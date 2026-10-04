// sivmone: Fast Sila Virtual Machine implementation
// Copyright 2019 The evmone Authors.
// SPDX-License-Identifier: Apache-2.0
#pragma once

#include <sivmc/sivmc.hpp>
#include <iosfwd>

namespace sivmone::tooling
{
int run(sivmc::VM& vm, sivmc_revision rev, int64_t gas, sivmc::bytes_view code,
    sivmc::bytes_view input, bool create, bool bench, std::ostream& out);
}  // namespace sivmone::tooling
