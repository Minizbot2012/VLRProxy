#pragma once
#include <RaceManager.h>
namespace vlrp::papyrus
{
#define STATIC_ARGS RE::StaticFunctionTag*
    [[nodiscard]] static auto LordRace(STATIC_ARGS, const RE::TESRace* rc)
        -> const RE::TESRace*
    {
        return vlrp::managers::RaceManager::GetSingleton()->GetVLRace(rc);
    }
    [[nodiscard]] static auto VampireRace(STATIC_ARGS, const RE::TESRace* rc)
        -> const RE::TESRace*
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
    [[nodiscard]] static auto IsVL(STATIC_ARGS, const RE::TESRace* rc) -> bool
    {
        return vlrp::managers::RaceManager::GetSingleton()->IsVampireLord(rc);
    }
    [[nodiscard]] static auto IsSupportedVampireRace(STATIC_ARGS,
        const RE::TESRace* rc)
        -> bool
    {
        return vlrp::managers::RaceManager::GetSingleton()->IsSupportedRace(rc);
    }
    [[nodiscard]] static auto IsSupportedVampireLord(STATIC_ARGS,
        const RE::TESRace* rc)
        -> bool
    {
        return vlrp::managers::RaceManager::GetSingleton()->IsSupportedVL(rc);
    }
    [[nodiscard]] static auto TransformActor(STATIC_ARGS, RE::Actor* actor,
        RE::TESRace* race) -> bool
    {
        return vlrp::managers::RaceManager::GetSingleton()->TransformActor(actor,
            race);
    }
    [[nodiscard]] static auto RevertActor(STATIC_ARGS, RE::Actor* actor) -> bool
    {
        return vlrp::managers::RaceManager::GetSingleton()->RevertActor(actor);
    }
    [[nodiscard]] static auto Version(STATIC_ARGS) -> uint32_t { return 1; }
    [[nodiscard]] static auto OriginalVL(STATIC_ARGS) -> const RE::TESRace*
    {
        return vlrp::managers::RaceManager::GetSingleton()->GetOriginalVL();
    }

#undef STATIC_ARGS
    bool Bind(RE::BSScript::IVirtualMachine* vm)
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
        // These provide per-actor transform tracking
        vm->RegisterFunction("TransformActor", "VLRace", TransformActor);
        vm->RegisterFunction("RevertActor", "VLRace", RevertActor);
        return true;
    }
}  // namespace vlrp::papyrus