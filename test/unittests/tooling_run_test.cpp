// sivmone: Fast Sila Virtual Machine implementation
// Copyright 2026 The evmone Authors.
// SPDX-License-Identifier: Apache-2.0

#include <gmock/gmock.h>
#include <sivmone/sivmone.h>
#include <test/utils/bytecode.hpp>
#include <test/utils/run.hpp>
#include <sstream>

using namespace sivmone;
using namespace sivmone::test;
using namespace sivmone::tooling;
using namespace testing;

TEST(tooling_run, execute)
{
    sivmc::VM vm{sivmc_create_sivmone()};
    const auto code = push(1);
    std::ostringstream out;
    const auto rc = run(vm, SIVMC_SILA_OSAKA, 100, code, {}, false, false, out);
    EXPECT_EQ(rc, 0);
    EXPECT_THAT(out.str(), HasSubstr("Executing"));
    EXPECT_THAT(out.str(), HasSubstr("SilaOsaka"));
    EXPECT_THAT(out.str(), HasSubstr("Result:   success"));
    EXPECT_THAT(out.str(), HasSubstr("Gas used: 3"));
}

TEST(tooling_run, create)
{
    sivmc::VM vm{sivmc_create_sivmone()};
    const auto code = mstore(0, 0x5f) + ret(31, 1);
    std::ostringstream out;
    const auto rc = run(vm, SIVMC_SILA_OSAKA, 100, code, {}, true, false, out);
    EXPECT_EQ(rc, 0);
    EXPECT_THAT(out.str(), HasSubstr("Creating"));
    EXPECT_THAT(out.str(), HasSubstr("SilaOsaka"));
    EXPECT_THAT(out.str(), HasSubstr("Result:   success"));
    EXPECT_THAT(out.str(), HasSubstr("Gas used: 2"));
}
