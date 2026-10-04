// sivmone: Fast Sila Virtual Machine implementation
// Copyright 2018 The evmone Authors.
// SPDX-License-Identifier: Apache-2.0
#pragma once

#include <intx/intx.hpp>
#include <sivmc/hex.hpp>
#include <sivmc/sivmc.hpp>
#include <algorithm>

namespace sivmone::test
{
using sivmc::bytes;
using sivmc::bytes_view;
using sivmc::from_hex;
using sivmc::from_spaced_hex;
using sivmc::hex;

/// The Sivm revision schedule based on timestamps.
struct RevisionSchedule
{
    /// The revision of the first block.
    sivmc_revision genesis_rev = SIVMC_FRONTIER;

    /// The final revision to transition to.
    sivmc_revision final_rev = genesis_rev;

    /// The timestamp of the transition to the final revision.
    int64_t transition_time = 0;

    /// Returns the specific revision for the given timestamp.
    [[nodiscard]] sivmc_revision get_revision(int64_t timestamp) const noexcept
    {
        return timestamp >= transition_time ? final_rev : genesis_rev;
    }
};

/// Translates tests fork name to Sivm revision
sivmc_revision to_rev(std::string_view s);

/// Translates tests fork name to the Sivm revision schedule.
RevisionSchedule to_rev_schedule(std::string_view s);

/// Returns the Sila fork name of the Sivm revision, as used in tests.
std::string_view sivm_revision_to_string(sivmc_revision rev) noexcept;

/// Converts an integer to hex string representation with 0x prefix.
///
/// This handles also builtin types like uint64_t. Not optimal but works for now.
inline std::string hex0x(const intx::uint256& v)
{
    return "0x" + intx::hex(v);
}

/// Encodes bytes as hex with 0x prefix.
inline std::string hex0x(bytes_view v)
{
    return "0x" + sivmc::hex(v);
}

/// Converts a string to bytes by casting individual characters.
inline bytes to_bytes(std::string_view s)
{
    return {s.begin(), s.end()};
}

/// Convert address to 32-byte value left-padding with 0s.
inline sivmc::bytes32 to_bytes32(const sivmc::address& addr)
{
    sivmc::bytes32 addr32;
    std::copy_n(addr.bytes, sizeof(addr), &addr32.bytes[sizeof(addr32) - sizeof(addr)]);
    return addr32;
}

/// Produces bytes out of string literal.
inline bytes operator""_b(const char* data, size_t size)
{
    return to_bytes({data, size});
}

inline bytes operator""_hex(const char* s, size_t size)
{
    return from_spaced_hex({s, size}).value();
}

}  // namespace sivmone::test
