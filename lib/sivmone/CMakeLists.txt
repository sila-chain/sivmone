# sivmone: Fast Sila Virtual Machine implementation
# Copyright 2019 The evmone Authors.
# SPDX-License-Identifier: Apache-2.0

include(LibraryTools)

add_library(sivmone
    ${include_dir}/sivmone/sivmone.h
    advanced_analysis.cpp
    advanced_analysis.hpp
    advanced_execution.cpp
    advanced_execution.hpp
    advanced_instructions.cpp
    baseline.hpp
    baseline_analysis.cpp
    baseline_execution.cpp
    baseline_instruction_table.cpp
    baseline_instruction_table.hpp
    constants.hpp
    create_address.cpp
    create_address.hpp
    delegation.cpp
    delegation.hpp
    instructions.hpp
    instructions_calls.cpp
    instructions_opcodes.hpp
    instructions_storage.cpp
    instructions_traits.hpp
    instructions_xmacro.hpp
    lru_cache.hpp
    tracing.cpp
    tracing.hpp
    vm.cpp
    vm.hpp
)
target_compile_features(sivmone PUBLIC cxx_std_20)
target_link_libraries(sivmone PUBLIC sivmc::sivmc intx::intx PRIVATE sivmone::precompiles)

# Nested Sivm calls recurse ~2.6 KB of stack per frame, too much for Windows' 1 MB default.
target_link_options(sivmone INTERFACE $<$<CXX_COMPILER_ID:MSVC>:/STACK:8388608>)

target_include_directories(sivmone PUBLIC
    $<BUILD_INTERFACE:${include_dir}>$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

if(SIVMONE_X86_64_ARCH_LEVEL GREATER_EQUAL 2)
    # Add CPU architecture runtime check. The SIVMONE_X86_64_ARCH_LEVEL has a valid value.
    target_sources(sivmone PRIVATE cpu_check.cpp)
    set_source_files_properties(cpu_check.cpp PROPERTIES COMPILE_DEFINITIONS SIVMONE_X86_64_ARCH_LEVEL=${SIVMONE_X86_64_ARCH_LEVEL})
endif()

if(CABLE_COMPILER_GNULIKE)
    target_compile_options(
        sivmone PRIVATE
        -fno-exceptions
        $<$<CXX_COMPILER_ID:GNU>:-Wstack-usage=2900>
    )
    if(NOT SANITIZE MATCHES undefined)
        # RTTI can be disabled except for UBSan which checks vptr integrity.
        target_compile_options(sivmone PRIVATE -fno-rtti)
    endif()
endif()
set_target_properties(
    sivmone
    PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_SOVERSION}
)

if(NOT SANITIZE)
    # On Linux, check if all symbols in sivmone are resolved during linking.
    target_link_options(sivmone PRIVATE $<$<PLATFORM_ID:Linux>:LINKER:--no-undefined>)
endif()

set_source_files_properties(vm.cpp PROPERTIES COMPILE_DEFINITIONS PROJECT_VERSION="${PROJECT_VERSION}")

add_standalone_library(sivmone)
