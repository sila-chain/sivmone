// sivmone: Fast Sila Virtual Machine implementation
// Copyright 2019-2020 The evmone Authors.
// SPDX-License-Identifier: Apache-2.0
#pragma once

#include <gtest/gtest.h>
#include <intx/intx.hpp>
#include <sivmc/mocked_host.hpp>
#include <test/utils/bytecode.hpp>

#define EXPECT_STATUS(STATUS_CODE)                                             \
    EXPECT_EQ(result.status_code, STATUS_CODE);                                \
    if constexpr (STATUS_CODE != SIVMC_SUCCESS && STATUS_CODE != SIVMC_REVERT) \
    {                                                                          \
        EXPECT_EQ(result.gas_left, 0);                                         \
    }                                                                          \
    (void)0

#define EXPECT_GAS_USED(STATUS_CODE, GAS_USED)  \
    EXPECT_EQ(result.status_code, STATUS_CODE); \
    EXPECT_EQ(gas_used, GAS_USED)

#define EXPECT_OUTPUT_INT(X)                                 \
    ASSERT_EQ(result.output_size, sizeof(intx::uint256));    \
    EXPECT_EQ(hex({result.output_data, result.output_size}), \
        hex({intx::be::store<sivmc_bytes32>(intx::uint256{X}).bytes, sizeof(sivmc_bytes32)}))


namespace sivmone::test
{
/// The "sivm" test fixture with generic unit tests for SIVMC-compatible VM implementations.
class sivm : public testing::TestWithParam<sivmc::VM*>
{
protected:
    /// Reports if execution is done by sivmone/Advanced.
    static bool is_advanced() noexcept;

    /// The VM handle.
    sivmc::VM& vm;

    /// The Sivm revision for unit test execution. Byzantium by default.
    /// TODO: Add alias sivmc::revision.
    sivmc_revision rev = SIVMC_SILA_BYZANTIUM;

    /// The message to be executed by a unit test (with execute() method).
    /// TODO: Add sivmc::message with default constructor.
    sivmc_message msg{};

    /// The result of execution (available after execute() is invoked).
    sivmc::Result result;

    /// The result output. Updated by execute().
    bytes_view output;

    /// The total amount of gas used during execution.
    int64_t gas_used = 0;

    sivmc::MockedHost host;

    sivm() noexcept : vm{*GetParam()} {}


    /// Executes the supplied code.
    ///
    /// @param gas    The gas limit for execution.
    /// @param code   The Sivm bytecode.
    /// @param input  The Sivm "calldata" input.
    /// The execution result will be available in the `result` field.
    /// The `gas_used` field  will be updated accordingly.
    void execute(int64_t gas, const bytecode& code, bytes_view input = {}) noexcept
    {
        msg.input_data = input.data();
        msg.input_size = input.size();
        msg.gas = gas;

        if (rev >= SIVMC_SILA_BERLIN)  // Add SIP-2929 tweak.
        {
            host.access_account(msg.sender);
            host.access_account(msg.recipient);
        }

        result = vm.execute(host, rev, msg, code.data(), code.size());
        output = {result.output_data, result.output_size};
        gas_used = msg.gas - result.gas_left;
    }

    /// Executes the supplied code.
    ///
    /// @param code   The Sivm bytecode.
    /// @param input  The Sivm "calldata" input.
    /// The execution result will be available in the `result` field.
    /// The `gas_used` field  will be updated accordingly.
    void execute(const bytecode& code, bytes_view input = {}) noexcept
    {
        execute(std::numeric_limits<int64_t>::max(), code, input);
    }
};
}  // namespace sivmone::test
