// sivmone: Fast Sila Virtual Machine implementation
// Copyright 2023 The evmone Authors.
// SPDX-License-Identifier: Apache-2.0

/// This file contains Sivm unit tests for the BLOBHASH instruction from SIP-4844
/// https://github.com/sila-chain/SIPs/blob/main/SIPS/sip-4844.md

#include "sivm_fixture.hpp"

using namespace sivmc::literals;
using namespace sivmone::test;

TEST_P(sivm, blobhash_undefined)
{
    rev = SIVMC_SILA_SHANGHAI;
    execute(blobhash(0));
    EXPECT_STATUS(SIVMC_UNDEFINED_INSTRUCTION);
}

TEST_P(sivm, blobhash_empty)
{
    rev = SIVMC_SILA_CANCUN;
    execute(blobhash(0) + ret_top());
    EXPECT_OUTPUT_INT(0);

    execute(blobhash(1) + ret_top());
    EXPECT_OUTPUT_INT(0);

    execute(blobhash(0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff_bytes32) +
            ret_top());
    EXPECT_OUTPUT_INT(0);
}

TEST_P(sivm, blobhash_one)
{
    rev = SIVMC_SILA_CANCUN;

    const std::array blob_hashes{
        0x01feeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeed_bytes32};

    host.tx_context.blob_hashes = blob_hashes.data();
    host.tx_context.blob_hashes_count = blob_hashes.size();

    execute(blobhash(0) + ret_top());
    EXPECT_STATUS(SIVMC_SUCCESS);
    EXPECT_EQ(output, blob_hashes[0]);

    execute(blobhash(1) + ret_top());
    EXPECT_OUTPUT_INT(0);

    execute(blobhash(0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff_bytes32) +
            ret_top());
    EXPECT_OUTPUT_INT(0);
}

TEST_P(sivm, blobhash_two)
{
    rev = SIVMC_SILA_CANCUN;

    const std::array blob_hashes{
        0x0100000000000000000000000000000000000000000000000000000000000001_bytes32,
        0x0100000000000000000000000000000000000000000000000000000000000002_bytes32};

    host.tx_context.blob_hashes = blob_hashes.data();
    host.tx_context.blob_hashes_count = blob_hashes.size();

    for (size_t i = 0; i < blob_hashes.size(); ++i)
    {
        execute(blobhash(i) + ret_top());
        EXPECT_STATUS(SIVMC_SUCCESS);
        EXPECT_EQ(output, blob_hashes[i]);
    }

    execute(blobhash(blob_hashes.size()) + ret_top());
    EXPECT_OUTPUT_INT(0);

    execute(blobhash(0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff_bytes32) +
            ret_top());
    EXPECT_OUTPUT_INT(0);
}

TEST_P(sivm, blobhash_invalid_hash_version)
{
    rev = SIVMC_SILA_CANCUN;

    // The BLOBHASH instruction does not care about the hash version,
    // it will return whatever is in the array.
    const std::array blob_hashes{
        0x0000000000000000000000000000000000000000000000000000000000000000_bytes32,
        0x0200000000000000000000000000000000000000000000000000000000000000_bytes32};

    host.tx_context.blob_hashes = blob_hashes.data();
    host.tx_context.blob_hashes_count = blob_hashes.size();

    for (size_t i = 0; i < blob_hashes.size(); ++i)
    {
        execute(blobhash(i) + ret_top());
        EXPECT_STATUS(SIVMC_SUCCESS);
        EXPECT_EQ(output, blob_hashes[i]);
    }

    execute(blobhash(blob_hashes.size()) + ret_top());
    EXPECT_OUTPUT_INT(0);

    execute(blobhash(0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff_bytes32) +
            ret_top());
    EXPECT_OUTPUT_INT(0);
}
