# sivmone: Fast Sila Virtual Machine implementation
# Copyright 2021 The evmone Authors.
# SPDX-License-Identifier: Apache-2.0

set(PREFIX ${PROJECT_NAME}/integration)

if(TARGET sivmone-cli)
    add_subdirectory(sivmone-cli)
endif()

add_subdirectory(export)
