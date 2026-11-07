#pragma once
#include <RaceManager.h>
#include <cstdint>
#include <string_view>

namespace MPL::papyrus
{
#define STATIC_ARGS RE::StaticFunctionTag*
    static RE::TESRace* LordRace(STATIC_ARGS, RE::TESRace* rc)
    {
        return MPL::Managers::RaceManager::GetSingleton()->GetLordRace(rc);
    }

    static RE::TESRace* VampireRace(STATIC_ARGS, RE::TESRace* rc)
    {
        return MPL::Managers::RaceManager::GetSingleton()->GetVampireRace(rc);
    }

    static bool IsVL(STATIC_ARGS, RE::TESRace* rc)
    {
        return MPL::Managers::RaceManager::GetSingleton()->IsVampireLord(rc);
    }

    static bool IsSupportedVampireRace(STATIC_ARGS,
        RE::TESRace* rc)
    {
        return MPL::Managers::RaceManager::GetSingleton()->IsSupportedRace(rc);
    }

    static bool IsSupportedVampireLord(STATIC_ARGS,
        RE::TESRace* rc)
    {
        return MPL::Managers::RaceManager::GetSingleton()->IsSupportedLord(rc);
    }

    static uint32_t Version(STATIC_ARGS) { return 1; }

    static RE::TESRace* OriginalVL(STATIC_ARGS)
    {
        return MPL::Managers::RaceManager::GetSingleton()->GetOriginalLord();
    }
    static RE::TESRace* GetRealRace(STATIC_ARGS, RE::Actor* actor)
    {
        return actor->GetActorRuntimeData().race;
    }
#undef STATIC_ARGS
    inline bool Bind(RE::BSScript::IVirtualMachine* a_vm)
    {
        a_vm->RegisterFunction("GetVLRace", "VLRace", LordRace);
        a_vm->RegisterFunction("GetVampireRace", "VLRace", VampireRace);
        a_vm->RegisterFunction("IsVL", "VLRace", IsVL);
        a_vm->RegisterFunction("IsSupportedVampireRace", "VLRace",
            IsSupportedVampireRace);
        a_vm->RegisterFunction("IsSupportedVampireLord", "VLRace",
            IsSupportedVampireLord);
        // New in 0.7.0+
        a_vm->RegisterFunction("OriginalVL", "VLRace", OriginalVL);
        a_vm->RegisterFunction("GetRealRace", "VLRace", GetRealRace);
        a_vm->RegisterFunction("Version", "VLRace", Version);
        return true;
    }
}  // namespace MPL::papyrus
