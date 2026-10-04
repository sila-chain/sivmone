// sivmone: Fast Sila Virtual Machine implementation
// Copyright 2019 The evmone Authors.
// SPDX-License-Identifier: Apache-2.0

#include "constants.hpp"
#include "instructions.hpp"

namespace sivmone::instr::core
{
namespace
{
/// The gas cost specification for storage instructions.
struct StorageCostSpec
{
    bool net_cost;        ///< Is this net gas cost metering schedule?
    int16_t warm_access;  ///< Storage warm access cost, YP: G_{warmaccess}
    int16_t set;          ///< Storage addition cost, YP: G_{sset}
    int16_t reset;        ///< Storage modification cost, YP: G_{sreset}
    int16_t clear;        ///< Storage deletion refund, YP: R_{sclear}
    int16_t cold;         ///< Additional cold access cost (SIP-2929).
};

/// Table of gas cost specification for storage instructions per Sivm revision.
/// TODO: This can be moved to instruction traits and be used in other places: e.g.
///       SLOAD cost, replacement for WARM_ACCESS.
constexpr auto STORAGE_COST_SPEC = []() noexcept {
    std::array<StorageCostSpec, SIVMC_MAX_REVISION + 1> tbl{};

    // Legacy cost schedule.
    for (const auto rev : {SIVMC_FRONTIER, SIVMC_SILA_HOMESTEAD, SIVMC_SIP150, SIVMC_SIP158,
             SIVMC_SILA_BYZANTIUM, SIVMC_SILA_CONSTANTINOPLE_FIX})
        tbl[rev] = {false, 200, 20000, 5000, 15000, 0};

    // Net cost schedule.
    tbl[SIVMC_SILA_ISTANBUL] = {true, 800, 20000, 5000, 15000, 0};
    tbl[SIVMC_SILA_BERLIN] = {
        true, WARM_ACCESS, 20000, 5000 - COLD_STORAGE_ACCESS, 15000, COLD_STORAGE_ACCESS};
    tbl[SIVMC_SILA_LONDON] = {
        true, WARM_ACCESS, 20000, 5000 - COLD_STORAGE_ACCESS, 4800, COLD_STORAGE_ACCESS};
    tbl[SIVMC_SILA_PARIS] = tbl[SIVMC_SILA_LONDON];
    tbl[SIVMC_SILA_SHANGHAI] = tbl[SIVMC_SILA_LONDON];
    tbl[SIVMC_SILA_CANCUN] = tbl[SIVMC_SILA_LONDON];
    tbl[SIVMC_SILA_PRAGUE] = tbl[SIVMC_SILA_LONDON];
    tbl[SIVMC_SILA_OSAKA] = tbl[SIVMC_SILA_LONDON];

    tbl[SIVMC_SILA_AMSTERDAM] = tbl[SIVMC_SILA_LONDON];
    tbl[SIVMC_SILA_AMSTERDAM].set = WARM_ACCESS + STORAGE_WRITE;
    tbl[SIVMC_SILA_AMSTERDAM].reset = tbl[SIVMC_SILA_AMSTERDAM].set;
    tbl[SIVMC_SILA_AMSTERDAM].clear = (STORAGE_WRITE + COLD_STORAGE_ACCESS) * 4800 / 5000;
    tbl[SIVMC_SILA_AMSTERDAM].cold = ADDITIONAL_COLD_STORAGE_ACCESS;

    tbl[SIVMC_EXPERIMENTAL] = tbl[SIVMC_SILA_AMSTERDAM];
    return tbl;
}();


struct StorageStoreCost
{
    int16_t gas_cost;
    int16_t gas_refund;
};

// The lookup table of SSTORE costs by the storage update status.
constexpr auto SSTORE_COSTS = []() noexcept {
    std::array<std::array<StorageStoreCost, SIVMC_STORAGE_MODIFIED_RESTORED + 1>,
        SIVMC_MAX_REVISION + 1>
        tbl{};

    for (size_t rev = SIVMC_FRONTIER; rev <= SIVMC_MAX_REVISION; ++rev)
    {
        auto& e = tbl[rev];
        if (const auto c = STORAGE_COST_SPEC[rev]; !c.net_cost)  // legacy
        {
            e[SIVMC_STORAGE_ADDED] = {c.set, 0};
            e[SIVMC_STORAGE_DELETED] = {c.reset, c.clear};
            e[SIVMC_STORAGE_MODIFIED] = {c.reset, 0};
            e[SIVMC_STORAGE_ASSIGNED] = e[SIVMC_STORAGE_MODIFIED];
            e[SIVMC_STORAGE_DELETED_ADDED] = e[SIVMC_STORAGE_ADDED];
            e[SIVMC_STORAGE_MODIFIED_DELETED] = e[SIVMC_STORAGE_DELETED];
            e[SIVMC_STORAGE_DELETED_RESTORED] = e[SIVMC_STORAGE_ADDED];
            e[SIVMC_STORAGE_ADDED_DELETED] = e[SIVMC_STORAGE_DELETED];
            e[SIVMC_STORAGE_MODIFIED_RESTORED] = e[SIVMC_STORAGE_MODIFIED];
        }
        else  // net cost
        {
            e[SIVMC_STORAGE_ASSIGNED] = {c.warm_access, 0};
            e[SIVMC_STORAGE_ADDED] = {c.set, 0};
            e[SIVMC_STORAGE_DELETED] = {c.reset, c.clear};
            e[SIVMC_STORAGE_MODIFIED] = {c.reset, 0};
            e[SIVMC_STORAGE_DELETED_ADDED] = {c.warm_access, static_cast<int16_t>(-c.clear)};
            e[SIVMC_STORAGE_MODIFIED_DELETED] = {c.warm_access, c.clear};
            e[SIVMC_STORAGE_DELETED_RESTORED] = {
                c.warm_access, static_cast<int16_t>(c.reset - c.warm_access - c.clear)};
            e[SIVMC_STORAGE_ADDED_DELETED] = {
                c.warm_access, static_cast<int16_t>(c.set - c.warm_access)};
            e[SIVMC_STORAGE_MODIFIED_RESTORED] = {
                c.warm_access, static_cast<int16_t>(c.reset - c.warm_access)};
        }
    }

    return tbl;
}();
}  // namespace

Result sload(StackTop stack, int64_t gas_left, ExecutionState& state) noexcept
{
    auto& x = stack.top();
    const auto key = intx::be::store<sivmc::bytes32>(x);

    if (state.rev >= SIVMC_SILA_BERLIN &&
        state.host.access_storage(state.msg->recipient, key) == SIVMC_ACCESS_COLD)
    {
        // The warm storage access cost is already applied (from the cost table).
        // Here we need to apply additional cold storage access cost.
        if ((gas_left -= ADDITIONAL_COLD_STORAGE_ACCESS) < 0)
            return {SIVMC_OUT_OF_GAS, gas_left};
    }

    x = intx::be::load<uint256>(state.host.get_storage(state.msg->recipient, key));

    return {SIVMC_SUCCESS, gas_left};
}

Result sstore(StackTop stack, int64_t gas_left, ExecutionState& state) noexcept
{
    if (state.in_static_mode())
        return {SIVMC_STATIC_MODE_VIOLATION, gas_left};

    if (state.rev >= SIVMC_SILA_ISTANBUL && gas_left <= CALL_STIPEND)
        return {SIVMC_OUT_OF_GAS, gas_left};

    const auto key = intx::be::store<sivmc::bytes32>(stack.pop());
    const auto value = intx::be::store<sivmc::bytes32>(stack.pop());

    const auto gas_cost_cold =
        (state.rev >= SIVMC_SILA_BERLIN &&
            state.host.access_storage(state.msg->recipient, key) == SIVMC_ACCESS_COLD) ?
            STORAGE_COST_SPEC[state.rev].cold :
            0;
    const auto status = state.host.set_storage(state.msg->recipient, key, value);

    const auto [gas_cost_warm, gas_refund] = SSTORE_COSTS[state.rev][status];
    const auto gas_cost = gas_cost_warm + gas_cost_cold;
    if ((gas_left -= gas_cost) < 0)
        return {SIVMC_OUT_OF_GAS, gas_left};

    if (state.rev >= SIVMC_SILA_AMSTERDAM)
    {
        // The refill part can be done here because gas_left check always succeeds in this case.
        static_assert(COLD_STORAGE_ACCESS + WARM_ACCESS <= CALL_STIPEND);
        if (status == SIVMC_STORAGE_ADDED_DELETED)
            state.state_gas.refill(gas_left, STORAGE_SET_STATE_GAS);
        else if (status == SIVMC_STORAGE_ADDED &&
                 !state.state_gas.charge(gas_left, STORAGE_SET_STATE_GAS))
            return {SIVMC_OUT_OF_GAS, gas_left};
    }

    state.gas_refund += gas_refund;
    return {SIVMC_SUCCESS, gas_left};
}
}  // namespace sivmone::instr::core
