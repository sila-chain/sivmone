// evmone: Fast Ethereum Virtual Machine implementation
// Copyright 2023 The evmone Authors.
// SPDX-License-Identifier: Apache-2.0

#include "utils.hpp"

namespace evmone::test
{

evmc_revision to_rev(std::string_view s)
{
    if (s == "Frontier")
        return EVMC_FRONTIER;
    if (s == "SilaHomestead")
        return EVMC_HOMESTEAD;
    if (s == "TangerineWhistle" || s == "SIP150")
        return EVMC_TANGERINE_WHISTLE;
    if (s == "SpuriousDragon" || s == "SIP158")
        return EVMC_SPURIOUS_DRAGON;
    if (s == "SilaByzantium")
        return EVMC_BYZANTIUM;
    if (s == "Petersburg" || s == "SilaConstantinopleFix")
        return EVMC_PETERSBURG;
    if (s == "SilaIstanbul")
        return EVMC_ISTANBUL;
    if (s == "SilaBerlin")
        return EVMC_BERLIN;
    if (s == "SilaLondon" || s == "ArrowGlacier")
        return EVMC_LONDON;
    if (s == "SilaParis" || s == "Merge")
        return EVMC_PARIS;
    if (s == "SilaShanghai")
        return EVMC_SHANGHAI;
    if (s == "SilaCancun")
        return EVMC_CANCUN;
    if (s == "SilaPrague")
        return EVMC_PRAGUE;
    if (s == "SilaOsaka")
        return EVMC_OSAKA;
    if (s == "SilaAmsterdam")
        return EVMC_AMSTERDAM;
    if (s == "SilaOsakaToBPO1AtTime15k")
        return EVMC_OSAKA;
    if (s == "BPO1ToBPO2AtTime15k")
        return EVMC_OSAKA;
    if (s == "BPO2ToBPO3AtTime15k")
        return EVMC_OSAKA;
    if (s == "BPO3ToBPO4AtTime15k")
        return EVMC_OSAKA;
    if (s == "BPO2ToSilaAmsterdamAtTime15k")
        return EVMC_OSAKA;
    if (s == "Experimental")
        return EVMC_EXPERIMENTAL;
    throw std::invalid_argument{"unknown revision: " + std::string{s}};
}

RevisionSchedule to_rev_schedule(std::string_view s)
{
    if (s == "SilaBerlinToSilaLondonAt5")
        return {EVMC_BERLIN, EVMC_LONDON, 5};
    if (s == "SilaParisToSilaShanghaiAtTime15k")
        return {EVMC_PARIS, EVMC_SHANGHAI, 15'000};
    if (s == "SilaShanghaiToSilaCancunAtTime15k")
        return {EVMC_SHANGHAI, EVMC_CANCUN, 15'000};
    if (s == "SilaCancunToSilaPragueAtTime15k")
        return {EVMC_CANCUN, EVMC_PRAGUE, 15'000};
    if (s == "SilaPragueToSilaOsakaAtTime15k")
        return {EVMC_PRAGUE, EVMC_OSAKA, 15'000};
    if (s == "BPO2ToSilaAmsterdamAtTime15k")
        return {EVMC_OSAKA, EVMC_AMSTERDAM, 15'000};

    const auto single_rev = to_rev(s);
    return {single_rev, single_rev, 0};
}

}  // namespace evmone::test
