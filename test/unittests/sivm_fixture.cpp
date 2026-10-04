// sivmone: Fast Sila Virtual Machine implementation
// Copyright 2019-2020 The evmone Authors.
// SPDX-License-Identifier: Apache-2.0

#include "sivm_fixture.hpp"
#include <sivmone/sivmone.h>

namespace sivmone::test
{
namespace
{
sivmc::VM advanced_vm{sivmc_create_sivmone(), {{"advanced", ""}}};
sivmc::VM baseline_vm{sivmc_create_sivmone()};
sivmc::VM bnocgoto_vm{sivmc_create_sivmone(), {{"cgoto", "no"}}};

const char* print_vm_name(const testing::TestParamInfo<sivmc::VM*>& info) noexcept
{
    if (info.param == &advanced_vm)
        return "advanced";
    if (info.param == &baseline_vm)
        return "baseline";
    if (info.param == &bnocgoto_vm)
        return "bnocgoto";
    return "unknown";
}
}  // namespace

INSTANTIATE_TEST_SUITE_P(
    sivmone, sivm, testing::Values(&advanced_vm, &baseline_vm, &bnocgoto_vm), print_vm_name);

bool sivm::is_advanced() noexcept
{
    return GetParam() == &advanced_vm;
}
}  // namespace sivmone::test
