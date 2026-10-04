// sivmone: Fast Sila Virtual Machine implementation
// Copyright 2023 The evmone Authors.
// SPDX-License-Identifier: Apache-2.0

#include "utils.hpp"

namespace sivmone::test
{

sivmc_revision to_rev(std::string_view s)
{
    if (s == "Frontier")
        return SIVMC_FRONTIER;
    if (s == "SilaHomestead")
        return SIVMC_SILA_HOMESTEAD;
    if (s == "SIP150")
        return SIVMC_SIP150;
    if (s == "SIP158")
        return SIVMC_SIP158;
    if (s == "SilaByzantium")
        return SIVMC_SILA_BYZANTIUM;
    if (s == "SilaConstantinopleFix")
        return SIVMC_SILA_CONSTANTINOPLE_FIX;
    if (s == "SilaIstanbul")
        return SIVMC_SILA_ISTANBUL;
    if (s == "SilaBerlin")
        return SIVMC_SILA_BERLIN;
    if (s == "SilaLondon")
        return SIVMC_SILA_LONDON;
    if (s == "SilaParis")
        return SIVMC_SILA_PARIS;
    if (s == "SilaShanghai")
        return SIVMC_SILA_SHANGHAI;
    if (s == "SilaCancun")
        return SIVMC_SILA_CANCUN;
    if (s == "SilaPrague")
        return SIVMC_SILA_PRAGUE;
    if (s == "SilaOsaka")
        return SIVMC_SILA_OSAKA;
    if (s == "SilaAmsterdam")
        return SIVMC_SILA_AMSTERDAM;
    if (s == "SilaOsakaToBPO1AtTime15k")
        return SIVMC_SILA_OSAKA;
    if (s == "BPO1ToBPO2AtTime15k")
        return SIVMC_SILA_OSAKA;
    if (s == "BPO2ToBPO3AtTime15k")
        return SIVMC_SILA_OSAKA;
    if (s == "BPO3ToBPO4AtTime15k")
        return SIVMC_SILA_OSAKA;
    if (s == "BPO2ToSilaAmsterdamAtTime15k")
        return SIVMC_SILA_OSAKA;
    if (s == "Experimental")
        return SIVMC_EXPERIMENTAL;
    throw std::invalid_argument{"unknown revision: " + std::string{s}};
}

std::string_view sivm_revision_to_string(sivmc_revision rev) noexcept
{
    switch (rev)
    {
    case SIVMC_FRONTIER:
        return "Frontier";
    case SIVMC_SILA_HOMESTEAD:
        return "SilaHomestead";
    case SIVMC_SIP150:
        return "SIP150";
    case SIVMC_SIP158:
        return "SIP158";
    case SIVMC_SILA_BYZANTIUM:
        return "SilaByzantium";
    case SIVMC_SILA_CONSTANTINOPLE_FIX:
        return "SilaConstantinopleFix";
    case SIVMC_SILA_ISTANBUL:
        return "SilaIstanbul";
    case SIVMC_SILA_BERLIN:
        return "SilaBerlin";
    case SIVMC_SILA_LONDON:
        return "SilaLondon";
    case SIVMC_SILA_PARIS:
        return "SilaParis";
    case SIVMC_SILA_SHANGHAI:
        return "SilaShanghai";
    case SIVMC_SILA_CANCUN:
        return "SilaCancun";
    case SIVMC_SILA_PRAGUE:
        return "SilaPrague";
    case SIVMC_SILA_OSAKA:
        return "SilaOsaka";
    case SIVMC_SILA_AMSTERDAM:
        return "SilaAmsterdam";
    case SIVMC_EXPERIMENTAL:
        return "Experimental";
    }
    return "<unknown>";
}

RevisionSchedule to_rev_schedule(std::string_view s)
{
    if (s == "SilaBerlinToSilaLondonAt5")
        return {SIVMC_SILA_BERLIN, SIVMC_SILA_LONDON, 5};
    if (s == "SilaParisToSilaShanghaiAtTime15k")
        return {SIVMC_SILA_PARIS, SIVMC_SILA_SHANGHAI, 15'000};
    if (s == "SilaShanghaiToSilaCancunAtTime15k")
        return {SIVMC_SILA_SHANGHAI, SIVMC_SILA_CANCUN, 15'000};
    if (s == "SilaCancunToSilaPragueAtTime15k")
        return {SIVMC_SILA_CANCUN, SIVMC_SILA_PRAGUE, 15'000};
    if (s == "SilaPragueToSilaOsakaAtTime15k")
        return {SIVMC_SILA_PRAGUE, SIVMC_SILA_OSAKA, 15'000};
    if (s == "BPO2ToSilaAmsterdamAtTime15k")
        return {SIVMC_SILA_OSAKA, SIVMC_SILA_AMSTERDAM, 15'000};

    const auto single_rev = to_rev(s);
    return {single_rev, single_rev, 0};
}

}  // namespace sivmone::test
