# sivmone: Fast Sila Virtual Machine implementation
# Copyright 2023 The evmone Authors.
# SPDX-License-Identifier: Apache-2.0

include(blst)

add_library(sivmone_precompiles STATIC)
add_library(sivmone::precompiles ALIAS sivmone_precompiles)
target_link_libraries(sivmone_precompiles PUBLIC evmc::evmc_cpp intx::intx PRIVATE blst::blst)
target_include_directories(sivmone_precompiles INTERFACE ..)
target_sources(
    sivmone_precompiles PRIVATE
    blake2b.hpp
    blake2b.cpp
    bls.hpp
    bls.cpp
    bn254.hpp
    bn254.cpp
    ecc.hpp
    hash_types.h
    keccak.h
    keccak.hpp
    keccak.c
    pairing/bn254/fields.hpp
    pairing/bn254/pairing.cpp
    pairing/bn254/utils.hpp
    pairing/field_template.hpp
    modarith.hpp
    modexp.hpp
    modexp.cpp
    mulmod.hpp
    mulmod.cpp
    ripemd160.hpp
    ripemd160.cpp
    secp256k1.hpp
    secp256k1.cpp
    secp256r1.hpp
    secp256r1.cpp
    sha256.hpp
    sha256.cpp
    kzg.hpp
    kzg.cpp
    kzg_precomputed_lines.hpp
    kzg_precomputed_lines.cpp
)

# Enable optimizations in the cryptography code also in Debug builds, otherwise it is very slow.
target_compile_options(
    sivmone_precompiles PRIVATE
    $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:GNU,Clang,AppleClang>>:-O1>
)
