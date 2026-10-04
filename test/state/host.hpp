// sivmone: Fast Sila Virtual Machine implementation
// Copyright 2022 The evmone Authors.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "state.hpp"
#include "state_view.hpp"
#include <sivmone/create_address.hpp>

namespace sivmone::state
{
using sivmc::uint256be;

class Host : public sivmc::Host
{
    sivmc_revision m_rev;
    sivmc::VM& m_vm;
    State& m_state;
    const BlockInfo& m_block;
    const BlockHashes& m_block_hashes;
    const Transaction& m_tx;
    std::vector<Log> m_logs;

public:
    Host(sivmc_revision rev, sivmc::VM& vm, State& state, const BlockInfo& block,
        const BlockHashes& block_hashes, const Transaction& tx) noexcept
      : m_rev{rev}, m_vm{vm}, m_state{state}, m_block{block}, m_block_hashes{block_hashes}, m_tx{tx}
    {}

    [[nodiscard]] std::vector<Log>&& take_logs() noexcept { return std::move(m_logs); }

    sivmc::Result call(const sivmc_message& msg) noexcept override;

    [[nodiscard]] bool account_exists(const address& addr) const noexcept override;

private:
    [[nodiscard]] bytes32 get_storage(
        const address& addr, const bytes32& key) const noexcept override;

    sivmc_storage_status set_storage(
        const address& addr, const bytes32& key, const bytes32& value) noexcept override;

    [[nodiscard]] sivmc::bytes32 get_transient_storage(
        const address& addr, const bytes32& key) const noexcept override;

    void set_transient_storage(
        const address& addr, const bytes32& key, const bytes32& value) noexcept override;

    [[nodiscard]] uint256be get_balance(const address& addr) const noexcept override;

    [[nodiscard]] uint64_t get_nonce(const address& addr) const noexcept override;

    [[nodiscard]] size_t get_code_size(const address& addr) const noexcept override;

    [[nodiscard]] bytes32 get_code_hash(const address& addr) const noexcept override;

    size_t copy_code(const address& addr, size_t code_offset, uint8_t* buffer_data,
        size_t buffer_size) const noexcept override;

    bool selfdestruct(const address& addr, const address& beneficiary) noexcept override;

    sivmc::Result create(const sivmc_message& msg) noexcept;

    [[nodiscard]] sivmc_tx_context get_tx_context() const noexcept override;

    [[nodiscard]] bytes32 get_block_hash(int64_t block_number) const noexcept override;

    void emit_log(const address& addr, const uint8_t* data, size_t data_size,
        const bytes32 topics[], size_t topics_count) noexcept override;

public:
    sivmc_access_status access_account(const address& addr) noexcept override;

private:
    sivmc_access_status access_storage(const address& addr, const bytes32& key) noexcept override;

    sivmc::Result execute_message(const sivmc_message& msg) noexcept;
};
}  // namespace sivmone::state
