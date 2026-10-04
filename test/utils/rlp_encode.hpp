// sivmone: Fast Sila Virtual Machine implementation
// Copyright 2022 The evmone Authors.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <test/state/block.hpp>
#include <test/state/transaction.hpp>

namespace sivmone::state
{
/// Defines how to RLP-encode a Transaction.
[[nodiscard]] bytes rlp_encode(const Transaction& tx);

/// Defines how to RLP-encode a TransactionReceipt.
[[nodiscard]] bytes rlp_encode(const TransactionReceipt& receipt);

/// Defines how to RLP-encode a Log.
[[nodiscard]] bytes rlp_encode(const Log& log);

/// Defines how to RLP-encode an Authorization (SIP-7702).
[[nodiscard]] bytes rlp_encode(const Authorization& authorization);

/// Defines how to RLP-encode a Withdrawal.
[[nodiscard]] bytes rlp_encode(const Withdrawal& withdrawal);
}  // namespace sivmone::state
