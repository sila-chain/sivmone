// sivmone: Fast Sila Virtual Machine implementation
// Copyright 2018 The evmone Authors.
// SPDX-License-Identifier: Apache-2.0

/// @file
/// SIVMC instance (class VM) and entry point of sivmone is defined here.

#include "vm.hpp"
#include "advanced_execution.hpp"
#include "baseline.hpp"
#include <sivmone/sivmone.h>
#include <cassert>
#include <iostream>

namespace sivmone
{
namespace
{
void destroy(sivmc_vm* vm) noexcept
{
    assert(vm != nullptr);
    delete static_cast<VM*>(vm);
}

sivmc_set_option_result set_option(sivmc_vm* c_vm, char const* c_name, char const* c_value) noexcept
{
    const auto name = (c_name != nullptr) ? std::string_view{c_name} : std::string_view{};
    const auto value = (c_value != nullptr) ? std::string_view{c_value} : std::string_view{};
    auto& vm = *static_cast<VM*>(c_vm);

    if (name == "advanced")
    {
        c_vm->execute = sivmone::advanced::execute;
        return SIVMC_SET_OPTION_SUCCESS;
    }
    else if (name == "cgoto")
    {
#if SIVMONE_CGOTO_SUPPORTED
        if (value == "no")
        {
            vm.cgoto = false;
            return SIVMC_SET_OPTION_SUCCESS;
        }
        return SIVMC_SET_OPTION_INVALID_VALUE;
#else
        return SIVMC_SET_OPTION_INVALID_NAME;
#endif
    }
    else if (name == "trace")
    {
        vm.add_tracer(create_instruction_tracer(std::clog));
        return SIVMC_SET_OPTION_SUCCESS;
    }
    else if (name == "histogram")
    {
        vm.add_tracer(create_histogram_tracer(std::clog));
        return SIVMC_SET_OPTION_SUCCESS;
    }
    else if (name == "opcode.count")
    {
        vm.add_tracer(create_instruction_counter(value));
    }
    return SIVMC_SET_OPTION_INVALID_NAME;
}

}  // namespace


VM::VM() noexcept
  : sivmc_vm{
        SIVMC_ABI_VERSION,
        "sivmone",
        PROJECT_VERSION,
        sivmone::destroy,
        sivmone::baseline::execute,
        sivmone::set_option,
    }
{
    m_execution_states.reserve(1025);
}

ExecutionState& VM::get_execution_state(size_t depth) noexcept
{
    // Vector already has the capacity for all possible depths,
    // so reallocation never happens (therefore: noexcept).
    // The ExecutionStates are lazily created because they pre-allocate Sivm memory and stack.
    assert(depth < m_execution_states.capacity());
    if (m_execution_states.size() <= depth)
        m_execution_states.resize(depth + 1);
    return m_execution_states[depth];
}

}  // namespace sivmone

extern "C" {
SIVMC_EXPORT sivmc_vm* sivmc_create_sivmone() noexcept
{
    return new sivmone::VM{};
}
}
