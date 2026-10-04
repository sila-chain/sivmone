// sivmone: Fast Sila Virtual Machine implementation
// Copyright 2018 The evmone Authors.
// SPDX-License-Identifier: Apache-2.0
#pragma once

#include <sivmc/sivmc.h>
#include <sivmc/utils.h>

namespace sivmone::advanced
{
struct AdvancedExecutionState;
struct AdvancedCodeAnalysis;

/// Execute the already analyzed code using the provided execution state.
SIVMC_EXPORT sivmc_result execute(
    AdvancedExecutionState& state, const AdvancedCodeAnalysis& analysis) noexcept;

/// SIVMC-compatible execute() function.
sivmc_result execute(sivmc_vm* vm, const sivmc_host_interface* host, sivmc_host_context* ctx,
    sivmc_revision rev, const sivmc_message* msg, const uint8_t* code, size_t code_size) noexcept;
}  // namespace sivmone::advanced
