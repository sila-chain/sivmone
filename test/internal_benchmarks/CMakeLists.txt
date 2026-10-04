# sivmone: Fast Sila Virtual Machine implementation
# Copyright 2019-2020 The evmone Authors.
# SPDX-License-Identifier: Apache-2.0

add_executable(
    sivmone-bench-internal
    modarith_bench.cpp
    find_jumpdest_bench.cpp
    memory_allocation.cpp
    lru_cache_bench.cpp
)

# TODO: Not sure why sivmone::precompiles must be here, but otherwise this don't link.
target_link_libraries(sivmone-bench-internal PRIVATE sivmone sivmone::precompiles benchmark::benchmark)
target_include_directories(sivmone-bench-internal PRIVATE ${sivmone_private_include_dir})
