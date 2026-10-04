// sivmone: Fast Sila Virtual Machine implementation
// Copyright 2024 The evmone Authors.
// SPDX-License-Identifier: Apache-2.0

#include <sivmone/baseline.hpp>
#include <gtest/gtest.h>
#include <test/utils/bytecode.hpp>

using namespace sivmone::test;

TEST(baseline_analysis, legacy)
{
    const auto code = push(1) + ret_top();
    const auto analysis = sivmone::baseline::analyze(code);

    EXPECT_EQ(analysis.code(), code);
    EXPECT_NE(analysis.code().data(), code.data()) << "copy should be made";
}
