// sivmone: Fast Sila Virtual Machine implementation
// Copyright 2019 The evmone Authors.
// SPDX-License-Identifier: Apache-2.0
#pragma once

#include <evmc/evmc.hpp>
#include <iosfwd>

namespace sivmone::tooling
{
int run(evmc::VM& vm, evmc_revision rev, int64_t gas, evmc::bytes_view code, evmc::bytes_view input,
    bool create, bool bench, std::ostream& out);
}  // namespace sivmone::tooling
