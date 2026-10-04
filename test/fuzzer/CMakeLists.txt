# sivmone-fuzzer: LibFuzzer based testing tool for SIVMC-compatible Sivm implementations.
# Copyright 2019 The evmone Authors.
# SPDX-License-Identifier: Apache-2.0
include(ExternalProject)

get_target_property(type sivmone TYPE)
if(NOT type STREQUAL STATIC_LIBRARY)
    message(FATAL_ERROR "The sivmone must be built as static library")
endif()

if(COVERAGE)
    set(CMAKE_EXE_LINKER_FLAGS "-fsanitize=fuzzer")
else()
    string(REPLACE fuzzer-no-link fuzzer CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS})
endif()

add_executable(sivmone-fuzzer fuzzer.cpp)
target_link_libraries(sivmone-fuzzer PRIVATE sivmone sivmone::testutils sivmc::mocked_host)
