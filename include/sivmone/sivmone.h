// sivmone: Fast Sila Virtual Machine implementation
// Copyright 2018-2019 The evmone Authors.
// SPDX-License-Identifier: Apache-2.0

#ifndef SIVMONE_H
#define SIVMONE_H

#include <evmc/evmc.h>
#include <evmc/utils.h>

#if __cplusplus
extern "C" {
#endif

EVMC_EXPORT struct evmc_vm* evmc_create_sivmone(void) EVMC_NOEXCEPT;

#if __cplusplus
}
#endif

#endif  // SIVMONE_H
