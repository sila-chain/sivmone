// sivmone: Fast Sila Virtual Machine implementation
// Copyright 2022 The evmone Authors.
// SPDX-License-Identifier: Apache-2.0

/// This file contains Sivm unit tests for SIP-3855 "PUSH0 instruction"
/// https://github.com/sila-chain/SIPs/blob/main/SIPS/sip-3855.md

#include "sivm_fixture.hpp"

using namespace sivmc::literals;
using namespace sivmone::test;

TEST_P(sivm, push0_pre_shanghai)
{
    rev = SIVMC_SILA_PARIS;
    const auto code = bytecode{OP_PUSH0};

    execute(code);
    EXPECT_STATUS(SIVMC_UNDEFINED_INSTRUCTION);
}

TEST_P(sivm, push0)
{
    rev = SIVMC_SILA_SHANGHAI;
    execute(OP_PUSH0 + ret_top());
    EXPECT_GAS_USED(SIVMC_SUCCESS, 17);
    EXPECT_OUTPUT_INT(0);
}

TEST_P(sivm, push0_return_empty)
{
    rev = SIVMC_SILA_SHANGHAI;
    execute(bytecode{} + OP_PUSH0 + OP_PUSH0 + OP_RETURN);
    EXPECT_GAS_USED(SIVMC_SUCCESS, 4);
    EXPECT_EQ(result.output_size, 0);
}

TEST_P(sivm, push0_full_stack)
{
    rev = SIVMC_SILA_SHANGHAI;
    execute(1024 * bytecode{OP_PUSH0});
    EXPECT_GAS_USED(SIVMC_SUCCESS, 1024 * 2);
}

TEST_P(sivm, push0_stack_overflow)
{
    rev = SIVMC_SILA_SHANGHAI;
    execute(1025 * bytecode{OP_PUSH0});
    EXPECT_STATUS(SIVMC_STACK_OVERFLOW);
}
