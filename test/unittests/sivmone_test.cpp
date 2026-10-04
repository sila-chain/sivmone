// sivmone: Fast Sila Virtual Machine implementation
// Copyright 2019-2020 The evmone Authors.
// SPDX-License-Identifier: Apache-2.0

#include <gtest/gtest.h>
#include <sivmc/sivmc.hpp>
#include <sivmone/sivmone.h>
#include <sivmone/vm.hpp>

TEST(sivmone, info)
{
    auto vm = sivmc::VM{sivmc_create_sivmone()};
    EXPECT_STREQ(vm.name(), "sivmone");
    EXPECT_STREQ(vm.version(), PROJECT_VERSION);
    EXPECT_TRUE(vm.is_abi_compatible());
}

TEST(sivmc, result_with_state_gas)
{
    const auto result = sivmc::Result{SIVMC_SUCCESS, 1, 2, {.left = 3, .spilled = 4}};
    EXPECT_EQ(result.status_code, SIVMC_SUCCESS);
    EXPECT_EQ(result.gas_left, 1);
    EXPECT_EQ(result.gas_refund, 2);
    EXPECT_EQ(result.state_gas.left, 3);
    EXPECT_EQ(result.state_gas.spilled, 4);
    EXPECT_EQ(result.output_data, nullptr);
    EXPECT_EQ(result.output_size, 0);

    const auto default_result = sivmc::Result{};
    EXPECT_EQ(default_result.state_gas.left, 0);
    EXPECT_EQ(default_result.state_gas.spilled, 0);

    const uint8_t output[] = {0x01};
    const auto output_result =
        sivmc::Result{SIVMC_REVERT, 1, 0, output, std::size(output), {.left = 5, .spilled = 6}};
    EXPECT_EQ(output_result.state_gas.left, 5);
    EXPECT_EQ(output_result.state_gas.spilled, 6);

    const auto failure_result = sivmc::Result{SIVMC_OUT_OF_GAS, {.left = 7}};
    EXPECT_EQ(failure_result.status_code, SIVMC_OUT_OF_GAS);
    EXPECT_EQ(failure_result.gas_left, 0);
    EXPECT_EQ(failure_result.gas_refund, 0);
    EXPECT_EQ(failure_result.state_gas.left, 7);
    EXPECT_EQ(failure_result.state_gas.spilled, 0);
}

TEST(sivmone, set_option_invalid)
{
    auto vm = sivmc_create_sivmone();
    ASSERT_NE(vm->set_option, nullptr);
    EXPECT_EQ(vm->set_option(vm, "", ""), SIVMC_SET_OPTION_INVALID_NAME);
    EXPECT_EQ(vm->set_option(vm, "o", ""), SIVMC_SET_OPTION_INVALID_NAME);
    EXPECT_EQ(vm->set_option(vm, "0", ""), SIVMC_SET_OPTION_INVALID_NAME);
    vm->destroy(vm);
}

TEST(sivmone, set_option_advanced)
{
    auto vm = sivmc::VM{sivmc_create_sivmone()};
    EXPECT_EQ(vm.set_option("advanced", ""), SIVMC_SET_OPTION_SUCCESS);

    // This will also enable Advanced.
    EXPECT_EQ(vm.set_option("advanced", "no"), SIVMC_SET_OPTION_SUCCESS);
}

TEST(sivmone, set_option_cgoto)
{
    sivmc::VM vm{sivmc_create_sivmone()};

#if SIVMONE_CGOTO_SUPPORTED
    EXPECT_EQ(vm.set_option("cgoto", ""), SIVMC_SET_OPTION_INVALID_VALUE);
    EXPECT_EQ(vm.set_option("cgoto", "yes"), SIVMC_SET_OPTION_INVALID_VALUE);
    EXPECT_EQ(vm.set_option("cgoto", "no"), SIVMC_SET_OPTION_SUCCESS);
#else
    EXPECT_EQ(vm.set_option("cgoto", "no"), SIVMC_SET_OPTION_INVALID_NAME);
#endif
}
