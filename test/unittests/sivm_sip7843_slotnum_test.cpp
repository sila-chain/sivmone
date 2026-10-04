// sivmone: Fast Sila Virtual Machine implementation
// Copyright 2026 The evmone Authors.
// SPDX-License-Identifier: Apache-2.0

/// This file contains Sivm unit tests for SIP-7843: SLOTNUM opcode.
/// https://github.com/sila-chain/SIPs/blob/main/SIPS/sip-7843.md

#include "sivm_fixture.hpp"

using namespace sivmone::test;

TEST_P(sivm, slotnum_values)
{
    rev = SIVMC_SILA_AMSTERDAM;
    for (const auto slot_number : {0ull, 0x123456789abcdef0ull, 0xffffffffffffffffull})
    {
        host.tx_context.block_slot_number = slot_number;
        execute(OP_SLOTNUM + ret_top());
        EXPECT_STATUS(SIVMC_SUCCESS);
        EXPECT_OUTPUT_INT(slot_number);
    }
}

TEST_P(sivm, slotnum_gas_cost)
{
    rev = SIVMC_SILA_AMSTERDAM;
    host.tx_context.block_slot_number = 1;
    execute(bytecode{} + OP_SLOTNUM);
    EXPECT_STATUS(SIVMC_SUCCESS);
    EXPECT_EQ(gas_used, 2);
}

TEST_P(sivm, slotnum_undefined_before_amsterdam)
{
    // SLOTNUM (opcode 0x4b) is introduced in Amsterdam; undefined in earlier forks.
    for (const auto r : {SIVMC_FRONTIER, SIVMC_SILA_OSAKA})
    {
        rev = r;
        execute(bytecode{} + OP_SLOTNUM);
        EXPECT_EQ(result.status_code, SIVMC_UNDEFINED_INSTRUCTION) << "fork " << r;
    }
}
