#pragma once
#include <RaceManager.h>
namespace MPL::papyrus
{
#define STATIC_ARGS RE::StaticFunctionTag*
    static auto LordRace(STATIC_ARGS, RE::TESRace* rc)
        -> RE::TESRace*
    {
        return MPL::Managers::RaceManager::GetSingleton()->GetLordRace(rc);
    }

    static auto VampireRace(STATIC_ARGS, RE::TESRace* rc)
        -> RE::TESRace*
    {
        return MPL::Managers::RaceManager::GetSingleton()->GetVampireRace(rc);
    }

    static auto IsVL(STATIC_ARGS, RE::TESRace* rc) -> bool
    {
        return MPL::Managers::RaceManager::GetSingleton()->IsVampireLord(rc);
    }

    static auto IsSupportedVampireRace(STATIC_ARGS,
        RE::TESRace* rc)
        -> bool
    {
        return MPL::Managers::RaceManager::GetSingleton()->IsSupportedRace(rc);
    }

    static auto IsSupportedVampireLord(STATIC_ARGS,
        RE::TESRace* rc)
        -> bool
    {
        return MPL::Managers::RaceManager::GetSingleton()->IsSupportedLord(rc);
    }

    static auto Version(STATIC_ARGS) -> uint32_t { return 1; }

    static auto OriginalVL(STATIC_ARGS) -> const RE::TESRace*
    {
        return MPL::Managers::RaceManager::GetSingleton()->GetOriginalLord();
    }
    static auto GetRealRace(STATIC_ARGS, RE::Actor* actor) -> const RE::TESRace* {
        return actor->GetRace();
    }
    static RE::TESRace* GetRace_ActorBase(RE::TESNPC* actor)
    {
        if(!actor)
            return nullptr;
        auto* RM = MPL::Managers::RaceManager::GetSingleton();
        if(RM->IsSupportedLord(actor->race)) {
            return RM->GetVampireRace(actor->race);
        }
        return actor->race;
    }
    static RE::TESRace* GetRace_Actor(RE::Actor* actor)
    {
        if(!actor)
            return nullptr;
        auto* RM = MPL::Managers::RaceManager::GetSingleton();
        if(RM->IsSupportedLord(actor->race)) {
            return RM->GetOriginalLord();
        }
        return actor->race;
    }
    static void SetRace(RE::Actor* actor, RE::TESRace* race)
    {
        if(!actor || !race)
            return;
        auto* RM = MPL::Managers::RaceManager::GetSingleton();
        if(RM->GetOriginalLord()->GetFormID() == race->GetFormID() && RM->IsSupportedRace(actor->GetRace())) {
            actor->SwitchRace(RM->GetLordRace(actor->GetRace()), actor->IsPlayerRef());
            //RM->AttachWings(actor);
            return;
        }
        //RM->DetachWings(actor);
        actor->SwitchRace(race, actor->IsPlayerRef());
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
        vm->RegisterFunction("GetRace", "ActorBase", GetRace_ActorBase);
        vm->RegisterFunction("GetRace", "Actor", GetRace_Actor);
        vm->RegisterFunction("SetRace", "Actor", SetRace);
        return true;
    }
}  // namespace MPL::papyrus
