// sivmone: Fast Sila Virtual Machine implementation
// Copyright 2023 The evmone Authors.
// SPDX-License-Identifier: Apache-2.0
#pragma once

#include <sivmc/sivmc.h>

namespace sivmone::state
{
int64_t calculate_difficulty(int64_t parent_difficulty, bool parent_has_ommers,
    int64_t parent_timestamp, int64_t current_timestamp, int64_t block_number,
    sivmc_revision rev) noexcept;
}  // namespace sivmone::state
