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
    if (s == "SIP150")
        return EVMC_TANGERINE_WHISTLE;
    if (s == "SIP158")
        return EVMC_SPURIOUS_DRAGON;
    if (s == "SilaByzantium")
        return EVMC_BYZANTIUM;
    if (s == "SilaConstantinopleFix")
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

std::string_view sivm_revision_to_string(evmc_revision rev) noexcept
{
    switch (rev)
    {
    case EVMC_FRONTIER:
        return "Frontier";
    case EVMC_HOMESTEAD:
        return "SilaHomestead";
    case EVMC_TANGERINE_WHISTLE:
        return "SIP150";
    case EVMC_SPURIOUS_DRAGON:
        return "SIP158";
    case EVMC_BYZANTIUM:
        return "SilaByzantium";
    case EVMC_PETERSBURG:
        return "SilaConstantinopleFix";
    case EVMC_ISTANBUL:
        return "SilaIstanbul";
    case EVMC_BERLIN:
        return "SilaBerlin";
    case EVMC_LONDON:
        return "SilaLondon";
    case EVMC_PARIS:
        return "SilaParis";
    case EVMC_SHANGHAI:
        return "SilaShanghai";
    case EVMC_CANCUN:
        return "SilaCancun";
    case EVMC_PRAGUE:
        return "SilaPrague";
    case EVMC_OSAKA:
        return "SilaOsaka";
    case EVMC_AMSTERDAM:
        return "SilaAmsterdam";
    case EVMC_EXPERIMENTAL:
        return "Experimental";
    }
    return "<unknown>";
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
