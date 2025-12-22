#pragma once
#include <RaceManager.h>
namespace MPL::papyrus
{
#define STATIC_ARGS RE::StaticFunctionTag*
    [[nodiscard]] static auto LordRace(STATIC_ARGS, const RE::TESRace* rc)
        -> const RE::TESRace*
    {
        return MPL::managers::RaceManager::GetSingleton()->GetLordRace(rc);
    }

    [[nodiscard]] static auto VampireRace(STATIC_ARGS, const RE::TESRace* rc)
        -> const RE::TESRace*
    {
        auto mgr = MPL::managers::RaceManager::GetSingleton();
        if (!mgr->IsVampireLord(rc))
        {
            return rc;
        }
        else if (mgr->IsSupportedLord(rc))
        {
            return mgr->GetVampireRace(rc);
        }
        else
        {
            return nullptr;
        }
    }

    [[nodiscard]] static auto IsVL(STATIC_ARGS, const RE::TESRace* rc) -> bool
    {
        return MPL::managers::RaceManager::GetSingleton()->IsVampireLord(rc);
    }

    [[nodiscard]] static auto IsSupportedVampireRace(STATIC_ARGS,
        const RE::TESRace* rc)
        -> bool
    {
        return MPL::managers::RaceManager::GetSingleton()->IsSupportedRace(rc);
    }

    [[nodiscard]] static auto IsSupportedVampireLord(STATIC_ARGS,
        const RE::TESRace* rc)
        -> bool
    {
        return MPL::managers::RaceManager::GetSingleton()->IsSupportedLord(rc);
    }

    [[nodiscard]] static auto Version(STATIC_ARGS) -> uint32_t { return 1; }

    [[nodiscard]] static auto OriginalVL(STATIC_ARGS) -> const RE::TESRace*
    {
        return MPL::managers::RaceManager::GetSingleton()->GetOriginalLord();
    }
    [[nodiscard]] static auto GetRealRace(STATIC_ARGS, RE::Actor* actor) -> const RE::TESRace* {
        return actor->race;
    }

#undef STATIC_ARGS
    inline bool Bind(RE::BSScript::IVirtualMachine* vm)
    {
        vm->RegisterFunction("GetVLRace", "VLRace", LordRace);
        vm->RegisterFunction("GetVampireRace", "VLRace", VampireRace);
        vm->RegisterFunction("IsVL", "VLRace", IsVL);
        vm->RegisterFunction("IsSupportedVampireRace", "VLRace",
            IsSupportedVampireRace);
        vm->RegisterFunction("IsSupportedVampireLord", "VLRace",
            IsSupportedVampireLord);
        // New in 0.7.0+
        vm->RegisterFunction("OriginalVL", "VLRace", OriginalVL);
        vm->RegisterFunction("Version", "VLRace", Version);
        vm->RegisterFunction("GetRealRace", "VLRace", GetRealRace);
        return true;
    }
}  // namespace MPL::papyrus
