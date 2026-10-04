# sivmone: Fast Sila Virtual Machine implementation
# Copyright 2024 The evmone Authors.
# SPDX-License-Identifier: Apache-2.0

add_executable(sivmone-precompiles-bench)
target_compile_features(sivmone-precompiles-bench PRIVATE cxx_std_20)
target_include_directories(sivmone-precompiles-bench PRIVATE ..)
target_link_libraries(sivmone-precompiles-bench PRIVATE sivmone::state benchmark::benchmark intx::intx)
target_sources(
    sivmone-precompiles-bench PRIVATE
    precompiles_bench.cpp
)

add_test(NAME sivmone/sivmone-precompiles-bench COMMAND sivmone-precompiles-bench --benchmark_min_time=1x)
set_tests_properties(sivmone/sivmone-precompiles-bench PROPERTIES FAIL_REGULAR_EXPRESSION "ERROR")
