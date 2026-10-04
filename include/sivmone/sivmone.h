// sivmone: Fast Sila Virtual Machine implementation
// Copyright 2018-2019 The evmone Authors.
// SPDX-License-Identifier: Apache-2.0

#ifndef SIVMONE_H
#define SIVMONE_H

#include <sivmc/sivmc.h>
#include <sivmc/utils.h>

#if __cplusplus
extern "C" {
#endif

SIVMC_EXPORT struct sivmc_vm* sivmc_create_sivmone(void) SIVMC_NOEXCEPT;

#if __cplusplus
}
#endif

#endif  // SIVMONE_H
