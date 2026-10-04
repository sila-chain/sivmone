// sivmone: Fast Sila Virtual Machine implementation
// Copyright 2025 The evmone Authors.
// SPDX-License-Identifier: Apache-2.0
#include "delegation.hpp"
#include <cassert>

namespace sivmone
{
std::optional<sivmc::address> get_delegate_address(
    const sivmc::HostInterface& host, const sivmc::address& addr) noexcept
{
    // Load the code prefix up to the delegation designation size.
    // The HostInterface::copy_code() copies up to the addr's code size
    // and returns the number of bytes copied.
    uint8_t designation_buffer[std::size(DELEGATION_MAGIC) + sizeof(sivmc::address)];
    const auto size = host.copy_code(addr, 0, designation_buffer, std::size(designation_buffer));
    const bytes_view designation{designation_buffer, size};

    if (!is_code_delegated(designation))
        return {};

    // Copy the delegate address from the designation buffer.
    sivmc::address delegate_address;
    // Assume the designation with the valid magic has also valid length.
    assert(designation.size() == std::size(designation_buffer));
    std::ranges::copy(designation.substr(std::size(DELEGATION_MAGIC)), delegate_address.bytes);
    return delegate_address;
}
}  // namespace sivmone
