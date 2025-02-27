#pragma once
#include <RaceManager.h>
#include <Config.h>
namespace vlrp::papyrus
{
#define STATIC_ARGS RE::StaticFunctionTag *
    [[nodiscard]] static auto GetVLRace(STATIC_ARGS, const RE::TESRace *rc)
        -> const RE::TESRace *
    {
        return vlrp::managers::RaceManager::GetSingleton()->GetVLRace(rc);
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
        vm->RegisterFunction("IsVL", "VLRace", IsVL);
        vm->RegisterFunction("IsSupportedVampireRace", "VLRace", IsSupportedVampireRace);
        vm->RegisterFunction("IsSupportedVampireLord", "VLRace", IsSupportedVampireLord);
        return true;
    }
}