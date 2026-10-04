// sivmone: Fast Sila Virtual Machine implementation
// Copyright 2023 The evmone Authors.
// SPDX-License-Identifier: Apache-2.0

#include <gtest/gtest.h>
#include <test/state/precompiles.hpp>

using namespace sivmc;
using namespace sivmone::state;

TEST(state_precompiles, is_precompile)
{
    for (int r = 0; r <= SIVMC_MAX_REVISION; ++r)
    {
        const auto rev = static_cast<sivmc_revision>(r);

        EXPECT_FALSE(is_precompile(rev, 0x00_address));

        // Frontier:
        EXPECT_TRUE(is_precompile(rev, 0x01_address));
        EXPECT_TRUE(is_precompile(rev, 0x02_address));
        EXPECT_TRUE(is_precompile(rev, 0x03_address));
        EXPECT_TRUE(is_precompile(rev, 0x04_address));

        // Byzantium:
        EXPECT_EQ(is_precompile(rev, 0x05_address), rev >= SIVMC_SILA_BYZANTIUM);
        EXPECT_EQ(is_precompile(rev, 0x06_address), rev >= SIVMC_SILA_BYZANTIUM);
        EXPECT_EQ(is_precompile(rev, 0x07_address), rev >= SIVMC_SILA_BYZANTIUM);
        EXPECT_EQ(is_precompile(rev, 0x08_address), rev >= SIVMC_SILA_BYZANTIUM);

        // Istanbul:
        EXPECT_EQ(is_precompile(rev, 0x09_address), rev >= SIVMC_SILA_ISTANBUL);

        // Cancun:
        EXPECT_EQ(is_precompile(rev, 0x0a_address), rev >= SIVMC_SILA_CANCUN);

        // Prague:
        EXPECT_EQ(is_precompile(rev, 0x0b_address), rev >= SIVMC_SILA_PRAGUE);
        EXPECT_EQ(is_precompile(rev, 0x0c_address), rev >= SIVMC_SILA_PRAGUE);
        EXPECT_EQ(is_precompile(rev, 0x0d_address), rev >= SIVMC_SILA_PRAGUE);
        EXPECT_EQ(is_precompile(rev, 0x0e_address), rev >= SIVMC_SILA_PRAGUE);
        EXPECT_EQ(is_precompile(rev, 0x0f_address), rev >= SIVMC_SILA_PRAGUE);
        EXPECT_EQ(is_precompile(rev, 0x10_address), rev >= SIVMC_SILA_PRAGUE);
        EXPECT_EQ(is_precompile(rev, 0x11_address), rev >= SIVMC_SILA_PRAGUE);

        // Osaka:
        EXPECT_EQ(is_precompile(rev, 0x0100_address), rev >= SIVMC_SILA_OSAKA);

        // Future?
        EXPECT_FALSE(is_precompile(rev, 0x12_address));
        EXPECT_FALSE(is_precompile(rev, 0x13_address));
        EXPECT_FALSE(is_precompile(rev, 0x14_address));
        EXPECT_FALSE(is_precompile(rev, 0x15_address));
        EXPECT_FALSE(is_precompile(rev, 0x16_address));
        EXPECT_FALSE(is_precompile(rev, 0x17_address));
    }
}
