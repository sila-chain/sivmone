// sivmone: Fast Sila Virtual Machine implementation
// Copyright 2025 The evmone Authors.
// SPDX-License-Identifier: Apache-2.0
#pragma once

#include <evmc/bytes.hpp>
#include <evmc/evmc.hpp>
#include <evmc/utils.h>

namespace sivmone
{
using evmc::bytes_view;

/// Prefix of code for delegated accounts
/// defined by [SIP-7702](https://github.com/sila-chain/SIPs/blob/main/SIPS/sip-7702.md)
constexpr uint8_t DELEGATION_MAGIC_BYTES[] = {0xef, 0x01, 0x00};
constexpr bytes_view DELEGATION_MAGIC{DELEGATION_MAGIC_BYTES, std::size(DELEGATION_MAGIC_BYTES)};

/// Check if code contains SIP-7702 delegation designator
constexpr bool is_code_delegated(bytes_view code) noexcept
{
    return code.starts_with(DELEGATION_MAGIC);
}

/// Get SIP-7702 delegate address from the code of addr, if it is delegated.
EVMC_EXPORT std::optional<evmc::address> get_delegate_address(
    const evmc::HostInterface& host, const evmc::address& addr) noexcept;
}  // namespace sivmone
