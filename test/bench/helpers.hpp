// sivmone: Fast Sila Virtual Machine implementation
// Copyright 2019 The evmone Authors.
// SPDX-License-Identifier: Apache-2.0
#pragma once

#include "test/utils/utils.hpp"
#include <benchmark/benchmark.h>
#include <sivmc/mocked_host.hpp>
#include <sivmc/sivmc.hpp>
#include <sivmone/advanced_analysis.hpp>
#include <sivmone/advanced_execution.hpp>
#include <sivmone/baseline.hpp>
#include <sivmone/vm.hpp>
#include <source_location>

namespace sivmone::test
{
extern std::map<std::string_view, sivmc::VM> registered_vms;

/// Decorates a dynamically registered benchmark name with the location of the registration.
/// CodSpeed identifies benchmarks by the "source_file::name" URI, which the BENCHMARK() macro
/// adds automatically, but RegisterBenchmark() does not. Without CodSpeed this is a no-op.
/// The default argument is evaluated at the call site, naming the file registering the benchmark.
inline std::string bench_name(std::string name,
    [[maybe_unused]] const std::source_location loc = std::source_location::current())
{
#ifdef CODSPEED_ENABLED
    return codspeed::get_path_relative_to_workspace(loc.file_name()) + "::" + std::move(name);
#else
    return name;
#endif
}

constexpr auto default_revision = SIVMC_SILA_ISTANBUL;
constexpr auto default_gas_limit = std::numeric_limits<int64_t>::max();


template <typename ExecutionStateT, typename AnalysisT>
using ExecuteFn = sivmc::Result(sivmc::VM& vm, ExecutionStateT& exec_state, const AnalysisT&,
    const sivmc_message&, sivmc_revision, sivmc::Host&, bytes_view);

template <typename AnalysisT>
using AnalyseFn = AnalysisT(sivmc_revision, bytes_view);


struct FakeExecutionState
{};

struct FakeCodeAnalysis
{};

inline advanced::AdvancedCodeAnalysis advanced_analyse(sivmc_revision rev, bytes_view code)
{
    return advanced::analyze(rev, code);
}

inline baseline::CodeAnalysis baseline_analyse(sivmc_revision /*rev*/, bytes_view code)
{
    return baseline::analyze(code);
}

inline FakeCodeAnalysis sivmc_analyse(sivmc_revision /*rev*/, bytes_view /*code*/)
{
    return {};
}


inline sivmc::Result advanced_execute(sivmc::VM& /*vm*/,
    advanced::AdvancedExecutionState& exec_state, const advanced::AdvancedCodeAnalysis& analysis,
    const sivmc_message& msg, sivmc_revision rev, sivmc::Host& host, bytes_view code)
{
    exec_state.reset(msg, rev, host.get_interface(), host.to_context(), code);
    return sivmc::Result{execute(exec_state, analysis)};
}

inline sivmc::Result baseline_execute(sivmc::VM& c_vm, [[maybe_unused]] ExecutionState& exec_state,
    const baseline::CodeAnalysis& analysis, const sivmc_message& msg, sivmc_revision rev,
    sivmc::Host& host, [[maybe_unused]] bytes_view code)
{
    auto& vm = *static_cast<sivmone::VM*>(c_vm.get_raw_pointer());
    return sivmc::Result{
        baseline::execute(vm, host.get_interface(), host.to_context(), rev, msg, analysis)};
}

inline sivmc::Result sivmc_execute(sivmc::VM& vm, FakeExecutionState& /*exec_state*/,
    const FakeCodeAnalysis& /*analysis*/, const sivmc_message& msg, sivmc_revision rev,
    sivmc::Host& host, bytes_view code) noexcept
{
    return vm.execute(host, rev, msg, code.data(), code.size());
}


template <typename AnalysisT, AnalyseFn<AnalysisT> analyse_fn>
inline void bench_analyse(benchmark::State& state, sivmc_revision rev, bytes_view code) noexcept
{
    auto bytes_analysed = uint64_t{0};
    for (auto _ : state)
    {
        auto r = analyse_fn(rev, code);
        benchmark::DoNotOptimize(&r);
        bytes_analysed += code.size();
    }

    using benchmark::Counter;
    state.counters["size"] = Counter(static_cast<double>(code.size()));
    state.counters["rate"] = Counter(static_cast<double>(bytes_analysed), Counter::kIsRate);
}


template <typename ExecutionStateT, typename AnalysisT,
    ExecuteFn<ExecutionStateT, AnalysisT> execute_fn, AnalyseFn<AnalysisT> analyse_fn>
inline void bench_execute(benchmark::State& state, sivmc::VM& vm, bytes_view code, bytes_view input,
    bytes_view expected_output) noexcept
{
    constexpr auto rev = default_revision;
    constexpr auto gas_limit = default_gas_limit;

    const auto analysis = analyse_fn(rev, code);
    sivmc::MockedHost host;
    ExecutionStateT exec_state;
    sivmc_message msg{};
    msg.kind = SIVMC_CALL;
    msg.gas = gas_limit;
    msg.input_data = input.data();
    msg.input_size = input.size();


    {  // Test run.
        const auto r = execute_fn(vm, exec_state, analysis, msg, rev, host, code);
        if (r.status_code != SIVMC_SUCCESS)
        {
            state.SkipWithError(("failure: " + std::to_string(r.status_code)).c_str());
            return;
        }

        if (!expected_output.empty())
        {
            const auto output = bytes_view{r.output_data, r.output_size};
            if (output != expected_output)
            {
                state.SkipWithError(
                    ("got: " + hex(output) + "  expected: " + hex(expected_output)).c_str());
                return;
            }
        }
    }

    auto total_gas_used = int64_t{0};
    auto iteration_gas_used = int64_t{0};
    for (auto _ : state)
    {
        const auto r = execute_fn(vm, exec_state, analysis, msg, rev, host, code);
        iteration_gas_used = gas_limit - r.gas_left;
        total_gas_used += iteration_gas_used;
    }

    using benchmark::Counter;
    state.counters["gas_used"] = Counter(static_cast<double>(iteration_gas_used));
    state.counters["gas_rate"] = Counter(static_cast<double>(total_gas_used), Counter::kIsRate);
}


// TODO(C++23): use constexpr.
inline auto bench_advanced_execute = bench_execute<advanced::AdvancedExecutionState,
    advanced::AdvancedCodeAnalysis, advanced_execute, advanced_analyse>;

// TODO(C++23): use constexpr.
inline auto bench_baseline_execute =
    bench_execute<ExecutionState, baseline::CodeAnalysis, baseline_execute, baseline_analyse>;

inline void bench_sivmc_execute(benchmark::State& state, sivmc::VM& vm, bytes_view code,
    bytes_view input = {}, bytes_view expected_output = {})
{
    bench_execute<FakeExecutionState, FakeCodeAnalysis, sivmc_execute, sivmc_analyse>(
        state, vm, code, input, expected_output);
}

}  // namespace sivmone::test
