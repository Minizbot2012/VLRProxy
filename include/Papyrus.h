#pragma once
#include <REX/W32/KERNEL32.h>
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
        vm->RegisterFunction("GetRealRace", "VLRace", GetRealRace);
        vm->RegisterFunction("Version", "VLRace", Version);
        return true;
    }
}  // namespace MPL::papyrus
