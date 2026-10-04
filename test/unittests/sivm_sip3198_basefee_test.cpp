// sivmone: Fast Sila Virtual Machine implementation
// Copyright 2021 The evmone Authors.
// SPDX-License-Identifier: Apache-2.0

/// This file contains Sivm unit tests for SIP-3198 "BASEFEE opcode"
/// https://github.com/sila-chain/SIPs/blob/main/SIPS/sip-3198.md

#include "sivm_fixture.hpp"

using namespace sivmone::test;

TEST_P(sivm, basefee_pre_london)
{
    rev = SIVMC_SILA_BERLIN;
    const auto code = bytecode{OP_BASEFEE};

    execute(code);
    EXPECT_STATUS(SIVMC_UNDEFINED_INSTRUCTION);
}

TEST_P(sivm, basefee_nominal_case)
{
    // https://github.com/sila-chain/SIPs/blob/main/SIPS/sip-3198.md#nominal-case
    rev = SIVMC_SILA_LONDON;
    host.tx_context.block_base_fee = sivmc::bytes32{7};

    execute(bytecode{} + OP_BASEFEE + OP_STOP);
    EXPECT_GAS_USED(SIVMC_SUCCESS, 2);

    execute(bytecode{} + OP_BASEFEE + ret_top());
    EXPECT_GAS_USED(SIVMC_SUCCESS, 17);
    EXPECT_OUTPUT_INT(7);
}
