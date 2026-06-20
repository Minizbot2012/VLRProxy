#pragma once
#include <RaceManager.h>
namespace MPL
{
    namespace Hook
    {
        bool HasArmorOrMorphRace(const RE::TESRace*, const RE::TESRace*);
        bool IsValidRace(const RE::TESObjectARMA*, RE::TESRace*);
        void TryInstall();
    }
    void TryInstall();
}  // namespace MPL
