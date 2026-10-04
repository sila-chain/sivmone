// sivmone: Fast Sila Virtual Machine implementation
// Copyright 2021 The evmone Authors.
// SPDX-License-Identifier: Apache-2.0
#pragma once

#include <intx/intx.hpp>
#include <sivmc/bytes.hpp>
#include <sivmc/sivmc.h>
#include <sivmc/utils.h>
#include <memory>
#include <ostream>
#include <string_view>

namespace sivmone
{
using sivmc::bytes_view;
class ExecutionState;

class Tracer
{
    friend class VM;  // Has access the m_next_tracer to traverse the list forward.
    std::unique_ptr<Tracer> m_next_tracer;

public:
    virtual ~Tracer() = default;

    void notify_execution_start(  // NOLINT(misc-no-recursion)
        sivmc_revision rev, const sivmc_message& msg, bytes_view code) noexcept
    {
        on_execution_start(rev, msg, code);
        if (m_next_tracer)
            m_next_tracer->notify_execution_start(rev, msg, code);
    }

    void notify_execution_end(const sivmc_result& result) noexcept  // NOLINT(misc-no-recursion)
    {
        on_execution_end(result);
        if (m_next_tracer)
            m_next_tracer->notify_execution_end(result);
    }

    void notify_instruction_start(  // NOLINT(misc-no-recursion)
        uint32_t pc, intx::uint256* stack_top, int stack_height, int64_t gas,
        const ExecutionState& state) noexcept
    {
        on_instruction_start(pc, stack_top, stack_height, gas, state);
        if (m_next_tracer)
            m_next_tracer->notify_instruction_start(pc, stack_top, stack_height, gas, state);
    }

private:
    virtual void on_execution_start(
        sivmc_revision rev, const sivmc_message& msg, bytes_view code) noexcept = 0;
    virtual void on_instruction_start(uint32_t pc, const intx::uint256* stack_top, int stack_height,
        int64_t gas, const ExecutionState& state) noexcept = 0;
    virtual void on_execution_end(const sivmc_result& result) noexcept = 0;
};

/// Creates the "histogram" tracer which counts occurrences of individual opcodes during execution
/// and reports this data in CSV format.
///
/// @param out  Report output stream.
/// @return     Histogram tracer object.
SIVMC_EXPORT std::unique_ptr<Tracer> create_histogram_tracer(std::ostream& out);

SIVMC_EXPORT std::unique_ptr<Tracer> create_instruction_counter(std::string_view out_file_path);

SIVMC_EXPORT std::unique_ptr<Tracer> create_instruction_tracer(std::ostream& out);

}  // namespace sivmone
