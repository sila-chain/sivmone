# sivmone: Fast Sila Virtual Machine implementation
# Copyright 2020 The evmone Authors.
# SPDX-License-Identifier: Apache-2.0

add_library(sivmone-experimental STATIC)
add_library(sivmone::experimental ALIAS sivmone-experimental)
target_link_libraries(sivmone-experimental PRIVATE sivmone)
target_include_directories(sivmone-experimental PRIVATE ${sivmone_private_include_dir})
target_sources(
    sivmone-experimental PRIVATE
    jumpdest_analysis.hpp
    jumpdest_analysis.cpp
)
