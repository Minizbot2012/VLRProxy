#pragma once
#include <RaceManager.h>
namespace vlrp::papyrus
{
#define STATIC_ARGS RE::StaticFunctionTag *
    [[nodiscard]] static auto GetVLRace(STATIC_ARGS, const RE::TESRace *rc)
        -> const RE::TESRace *
    {
        return vlrp::managers::RaceManager::GetSingleton()->GetVLRace(rc);
    }
    [[nodiscard]] static auto GetVampireRace(STATIC_ARGS, const RE::TESRace *rc)
        -> const RE::TESRace *
    {
        auto mgr = vlrp::managers::RaceManager::GetSingleton();
        if (!mgr->IsVampireLord(rc))
        {
            return rc;
        }
        else if (mgr->IsSupportedVL(rc))
        {
            return mgr->GetVampireRace(rc);
        }
        else
        {
            return nullptr;
        }
    }
    [[nodiscard]] static auto IsVL(STATIC_ARGS, const RE::TESRace *rc) -> bool
    {
        return vlrp::managers::RaceManager::GetSingleton()->IsVampireLord(rc);
    }
    [[nodiscard]] static auto IsSupportedVampireRace(STATIC_ARGS, const RE::TESRace *rc) -> bool
    {
        return vlrp::managers::RaceManager::GetSingleton()->IsSupportedRace(rc);
    }
    [[nodiscard]] static auto IsSupportedVampireLord(STATIC_ARGS, const RE::TESRace *rc) -> bool
    {
        return vlrp::managers::RaceManager::GetSingleton()->IsSupportedVL(rc);
    }
#undef STATIC_ARGS
    bool Bind(RE::BSScript::IVirtualMachine *vm)
    {
        vm->RegisterFunction("GetVLRace", "VLRace", GetVLRace);
        vm->RegisterFunction("GetVampireRace", "VLRace", GetVampireRace);
        vm->RegisterFunction("IsVL", "VLRace", IsVL);
        vm->RegisterFunction("IsSupportedVampireRace", "VLRace", IsSupportedVampireRace);
        vm->RegisterFunction("IsSupportedVampireLord", "VLRace", IsSupportedVampireLord);
        return true;
    }
}