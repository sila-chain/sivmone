// sivmone: Fast Sila Virtual Machine implementation
// Copyright 2022 The evmone Authors.
// SPDX-License-Identifier: Apache-2.0

/// This file contains Sivm unit tests for SIP-3860 "Limit and meter initcode"
/// https://github.com/sila-chain/SIPs/blob/main/SIPS/sip-3860.md

#include "sivm_fixture.hpp"

using namespace sivmc::literals;
using namespace sivmone::test;

inline constexpr size_t initcode_size_limit = 0xc000;

TEST_P(sivm, create_initcode_limit)
{
    for (const auto& c : {create().input(0, calldataload(0)) + ret_top(),
             create2().input(0, calldataload(0)) + ret_top()})
    {
        for (const auto r : {SIVMC_SILA_PARIS, SIVMC_SILA_SHANGHAI})
        {
            rev = r;
            for (const auto s : {initcode_size_limit, initcode_size_limit + 1})
            {
                execute(c, sivmc::uint256be{s});
                if (rev >= SIVMC_SILA_SHANGHAI && s > initcode_size_limit)
                {
                    EXPECT_STATUS(SIVMC_OUT_OF_GAS);
                }
                else
                {
                    EXPECT_STATUS(SIVMC_SUCCESS);
                    ASSERT_EQ(result.output_size, 32);
                    EXPECT_NE(intx::be::unsafe::load<intx::uint256>(result.output_data), 0);
                }
            }
        }
    }
}

TEST_P(sivm, create_initcode_gas_cost)
{
    rev = SIVMC_SILA_SHANGHAI;
    const auto code = create().input(0, calldataload(0));
    execute(44300, code, sivmc::uint256be{initcode_size_limit});
    EXPECT_GAS_USED(SIVMC_SUCCESS, 44300);
    execute(44299, code, sivmc::uint256be{initcode_size_limit});
    EXPECT_STATUS(SIVMC_OUT_OF_GAS);
}

TEST_P(sivm, create2_initcode_gas_cost)
{
    rev = SIVMC_SILA_SHANGHAI;
    const auto code = create2().input(0, calldataload(0));
    execute(53519, code, sivmc::uint256be{initcode_size_limit});
    EXPECT_GAS_USED(SIVMC_SUCCESS, 53519);
    execute(53518, code, sivmc::uint256be{initcode_size_limit});
    EXPECT_STATUS(SIVMC_OUT_OF_GAS);
}
