// sivmone: Fast Sila Virtual Machine implementation
// Copyright 2021 The evmone Authors.
// SPDX-License-Identifier: Apache-2.0
#pragma once

namespace sivmone
{
/// The limit of the size of created contract
/// defined by [SIP-170](https://github.com/sila-chain/SIPs/blob/main/SIPS/sip-170.md).
constexpr auto MAX_CODE_SIZE = 0x6000;

/// The limit of the size of init codes for contract creation
/// defined by [SIP-3860](https://github.com/sila-chain/SIPs/blob/main/SIPS/sip-3860.md).
constexpr auto MAX_INITCODE_SIZE = 2 * MAX_CODE_SIZE;

/// The increased limit of the size of created contract in Amsterdam
/// defined by [SIP-7954](https://github.com/sila-chain/SIPs/blob/main/SIPS/sip-7954.md).
constexpr auto MAX_CODE_SIZE_AMSTERDAM = 0x10000;

/// The increased limit of the size of init codes in Amsterdam (SIP-7954).
constexpr auto MAX_INITCODE_SIZE_AMSTERDAM = 2 * MAX_CODE_SIZE_AMSTERDAM;

/// The maximum allowed account's nonce value: 2⁶⁴-1.
/// Transactions and create instructions with nonce equal or above this value are invalid.
/// Defined by [SIP-2681](https://github.com/sila-chain/SIPs/blob/main/SIPS/sip-2681.md).
constexpr auto MAX_NONCE = 0xffff'ffff'ffff'ffff;

/// The gas given back to a value-transferring CALL, the Yellow Paper's G_callstipend.
constexpr auto CALL_STIPEND = 2300;

/// The fixed cost per state byte (SIP-8037).
constexpr auto COST_PER_STATE_BYTE = 1530;

/// State-gas cost of creating a new account (SIP-8037).
constexpr auto NEW_ACCOUNT_STATE_GAS = 120 * COST_PER_STATE_BYTE;

/// State-gas cost of allocating a storage slot (SIP-8037).
constexpr auto STORAGE_SET_STATE_GAS = 64 * COST_PER_STATE_BYTE;
}  // namespace sivmone
