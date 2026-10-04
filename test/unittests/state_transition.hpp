// sivmone: Fast Sila Virtual Machine implementation
// Copyright 2023 The evmone Authors.
// SPDX-License-Identifier: Apache-2.0
#pragma once

#include "exportable_fixture.hpp"
#include <sivmone/sivmone.h>
#include <test/state/errors.hpp>
#include <test/state/host.hpp>
#include <test/utils/test_state.hpp>

namespace sivmone::test
{
using namespace sivmone;
using namespace sivmone::state;

/// Fixture to defining test cases in form similar to JSON State Tests.
///
/// It takes the "pre" state and produces "post" state by applying the defined "tx" transaction.
/// Then expectations declared in "except" are checked in the "post" state.
class state_transition : public ExportableFixture
{
protected:
    /// The default sender address of the test transaction.
    /// Private key: 0x2b1263d2b.
    static constexpr auto Sender = 0xe100713FC15400D1e94096a545879E7c6407001e_address;

    /// The secret (private) key of the Sender address.
    static constexpr auto SenderSecretKey =
        0x00000000000000000000000000000000000000000000000000000002b1263d2b_bytes32;

    /// The default destination address of the test transaction.
    static constexpr auto To = 0xc0de_address;

    /// A second signing account, for tests needing a signature that is not the Sender's
    /// (e.g. an SIP-7702 authority).
    /// Private key: 0xa5.
    static constexpr auto AUTHORITY = 0x1d694d5ad94f32132ff5c14c901d3ddbee90a550_address;

    static constexpr auto Coinbase = 0xc014bace_address;

    static inline sivmc::VM vm{sivmc_create_sivmone()};
    static inline sivmc::VM tracing_vm{sivmc_create_sivmone(), {{"trace", "1"}}};

    struct ExpectedAccount
    {
        bool exists = true;

        /// Whether the account is expected to be mentioned in the transaction's state diff.
        std::optional<bool> in_diff;

        std::optional<uint64_t> nonce;
        std::optional<intx::uint256> balance;
        std::optional<bytes> code;
        std::unordered_map<bytes32, bytes32> storage;
    };

    struct Expectation
    {
        /// The transaction is invalid because of the given error.
        /// The rest of Expectation is ignored if the error is expected.
        ErrorCode tx_error = SUCCESS;

        /// The expected Sivm status code of the transaction execution.
        sivmc_status_code status = SIVMC_SUCCESS;

        /// The expected amount of gas used by the transaction.
        std::optional<int64_t> gas_used;

        /// The expected amount of gas counted against the block gas limit (SIP-7778).
        std::optional<int64_t> block_gas_used;

        /// The expected logs emitted by the transaction. When set, the receipt's logs must match
        /// exactly: count, address, data, topics, and order.
        std::optional<std::vector<Log>> logs;

        /// The expected state-gas component of the receipt (SIP-8037).
        std::optional<int64_t> state_gas;

        /// The expected post-execution state.
        std::unordered_map<address, ExpectedAccount> post;

        std::optional<hash256> state_hash;

        /// The expected Sivm execution trace. If not empty transaction execution will be performed
        /// with tracing enabled and the output compared.
        std::string_view trace;
    };


    sivmc_revision rev = SIVMC_SILA_SHANGHAI;
    uint64_t block_reward = 0;
    BlockInfo block{
        .number = 1,  // Some Sivms don't like blocks with number 0.
        .gas_limit = 1'000'000,
        .coinbase = Coinbase,
        .base_fee = 999,
    };
    TestBlockHashes block_hashes;
    Transaction tx{
        // The default type corresponds to the default `rev` and majority of tests.
        .type = Transaction::Type::sip1559,
        .gas_limit = block.gas_limit,
        .max_gas_price = block.base_fee + 1,
        .max_priority_gas_price = block.base_fee + 1,
        .sender = Sender,
        .chain_id = 1,
        .nonce = 1,
    };
    TestState pre;
    Expectation expect;

    void SetUp() override;

    /// The test runner.
    void TearDown() override;

    /// Build the expected SIP-7708 Transfer log: {SYSTEM_ADDRESS, amount (32-byte big-endian),
    /// topics = [Transfer event topic, sender, recipient]}.
    static Log transfer_log(
        const address& sender, const address& recipient, const intx::uint256& amount);

    /// Exports the test in the JSON State Test format to ExportableFixture::export_out.
    void export_state_test(
        const std::variant<TransactionReceipt, std::error_code>& res, const TestState& post);
};

}  // namespace sivmone::test
