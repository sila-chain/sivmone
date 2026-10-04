// sivmone: Fast Sila Virtual Machine implementation
// Copyright 2025 The evmone Authors.
// SPDX-License-Identifier: Apache-2.0

#include <gtest/gtest.h>
#include <sivmc/hex.hpp>
#include <test/state/precompiles_internal.hpp>
#include <array>

TEST(bn254, ecpairing_null_pairs)
{
    // Any number of null pairs should pass the pairing check.
    for (const auto n : {0, 1, 2, 3, 4, 5})
    {
        sivmc::bytes input(192 * static_cast<size_t>(n), 0);
        std::array<uint8_t, 32> result{};
        const auto [status_code, output_size] = sivmone::state::ecpairing_execute(
            input.data(), input.size(), result.data(), result.size());
        EXPECT_EQ(status_code, SIVMC_SUCCESS);
        EXPECT_EQ(output_size, result.size());
        EXPECT_EQ(sivmc::hex({result.data(), result.size()}),
            "0000000000000000000000000000000000000000000000000000000000000001");
    }
}
