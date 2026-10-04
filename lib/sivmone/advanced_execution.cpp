// sivmone: Fast Sila Virtual Machine implementation
// Copyright 2019 The evmone Authors.
// SPDX-License-Identifier: Apache-2.0

#include "advanced_execution.hpp"
#include "advanced_analysis.hpp"
#include <memory>

namespace sivmone::advanced
{
sivmc_result execute(AdvancedExecutionState& state, const AdvancedCodeAnalysis& analysis) noexcept
{
    state.analysis.advanced = &analysis;  // Allow accessing the analysis by instructions.

    const auto* instr = state.analysis.advanced->instrs.data();  // Get the first instruction.
    while (instr != nullptr)
        instr = instr->fn(instr, state);

    return make_execution_result(state, state.gas_left);
}

sivmc_result execute(sivmc_vm* /*unused*/, const sivmc_host_interface* host,
    sivmc_host_context* ctx, sivmc_revision rev, const sivmc_message* msg, const uint8_t* code,
    size_t code_size) noexcept
{
    const bytes_view container{code, code_size};
    const auto analysis = analyze(rev, container);
    auto state = std::make_unique<AdvancedExecutionState>(*msg, rev, *host, ctx, container);
    return execute(*state, analysis);
}
}  // namespace sivmone::advanced
