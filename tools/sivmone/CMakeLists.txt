# sivmone: Fast Sila Virtual Machine implementation
# Copyright 2025 The evmone Authors.
# SPDX-License-Identifier: Apache-2.0

hunter_add_package(CLI11)
find_package(CLI11 CONFIG REQUIRED)

add_executable(sivmone-cli main.cpp)
set_target_properties(sivmone-cli PROPERTIES OUTPUT_NAME sivmone)
target_link_libraries(sivmone-cli PRIVATE sivmone sivmone::testutils CLI11::CLI11)
