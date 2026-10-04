// sivmone: Fast Sila Virtual Machine implementation
// Copyright 2020 The evmone Authors.
// SPDX-License-Identifier: Apache-2.0

#include "baseline_instruction_table.hpp"
#include "instructions_traits.hpp"

namespace sivmone::baseline
{
namespace
{
consteval auto build_cost_tables() noexcept
{
    std::array<CostTable, SIVMC_MAX_REVISION + 1> tables{};
    for (size_t r = SIVMC_FRONTIER; r <= SIVMC_MAX_REVISION; ++r)
    {
        auto& table = tables[r];
        for (size_t op = 0; op < table.size(); ++op)
        {
            const auto& tr = instr::traits[op];
            const auto since = tr.since;
            table[op] = (since && r >= *since) ? instr::gas_costs[r][op] : instr::undefined;
        }
    }
    return tables;
}

constexpr auto COST_TABLES = build_cost_tables();
}  // namespace

const CostTable& get_baseline_cost_table(sivmc_revision rev) noexcept
{
    return COST_TABLES[rev];
}
}  // namespace sivmone::baseline
