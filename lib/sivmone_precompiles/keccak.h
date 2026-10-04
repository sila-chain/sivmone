/* silash: C/C++ implementation of Silash, the Sila Proof of Work algorithm.
 * Copyright 2018-2019 Pawel Bylica.
 * Licensed under the Apache License, Version 2.0.
 */

#pragma once

#include "hash_types.h"
#include <stddef.h>

#ifndef __cplusplus
#define noexcept  // Ignore noexcept in C code.
#endif

#ifdef __cplusplus
extern "C" {
#endif

union silash_hash256 silash_keccak256(const uint8_t* data, size_t size) noexcept;

#ifdef __cplusplus
}
#endif
