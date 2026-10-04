// sivmone: Fast Sila Virtual Machine implementation
// Copyright 2020 The evmone Authors.
// SPDX-License-Identifier: Apache-2.0
#pragma once

#include <sivmc/sivmc.h>
#include <array>

namespace sivmone::baseline
{
using CostTable = std::array<int16_t, 256>;

const CostTable& get_baseline_cost_table(sivmc_revision rev) noexcept;
}  // namespace sivmone::baseline
