# sivmone: Fast Sila Virtual Machine implementation
# Copyright 2019 The evmone Authors.
# SPDX-License-Identifier: Apache-2.0

add_library(sivmone.testutils STATIC)
add_library(sivmone::testutils ALIAS sivmone.testutils)
target_link_libraries(
    sivmone.testutils
    PUBLIC sivmone::state evmc::evmc_cpp nlohmann_json::nlohmann_json
    PRIVATE evmc::mocked_host
)

target_sources(
    sivmone.testutils
    PRIVATE
    stdx/utility.hpp
    blob_schedule.hpp
    blob_schedule.cpp
    block_transition.hpp
    block_transition.cpp
    blockchaintest.hpp
    blockchaintest_loader.cpp
    blockchaintest_runner.cpp
    bytecode.hpp
    error_matching.hpp
    error_matching.cpp
    mpt.hpp
    mpt.cpp
    mpt_hash.hpp
    mpt_hash.cpp
    rlp.hpp
    rlp_encode.hpp
    rlp_encode.cpp
    run.hpp
    run.cpp
    statetest.hpp
    statetest_export.cpp
    statetest_loader.cpp
    statetest_logs_hash.cpp
    statetest_runner.cpp
    t8n.hpp
    t8n.cpp
    test_collector.hpp
    test_collector.cpp
    test_driver.hpp
    test_driver.cpp
    test_report.hpp
    test_report.cpp
    test_state.hpp
    test_state.cpp
    utils.hpp
    utils.cpp
)
