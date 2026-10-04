// silash: C/C++ implementation of Silash, the Sila Proof of Work algorithm.
// Copyright 2018-2019 Pawel Bylica.
// Licensed under the Apache License, Version 2.0.

#pragma once

#include "keccak.h"

namespace silash
{
inline hash256 keccak256(const uint8_t* data, size_t size) noexcept
{
    return silash_keccak256(data, size);
}

}  // namespace silash
