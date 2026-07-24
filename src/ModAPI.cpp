#include <ModAPI.h>
namespace MPL::API::VLRProxy
{
    uint16_t ModAPI::GetVersion()
    {
        return 1;
    }

    RE::TESRace* ModAPI::GetVLRace(RE::TESRace* race)
    {
        if (race != nullptr)
        {
            return MPL::Managers::RaceManager::GetSingleton()->GetLordRace(race);
        }
        else
        {
            return nullptr;
        }
    }

    RE::TESRace* ModAPI::GetVampireRace(RE::TESRace* race)
    {
        if (race != nullptr)
        {
            return MPL::Managers::RaceManager::GetSingleton()->GetVampireRace(race);
        }
        else {
            return nullptr;
        }
    }

    RE::TESRace* ModAPI::GetRegularVL()
    {
        return MPL::Managers::RaceManager::GetSingleton()->GetOriginalLord();
    }

    bool ModAPI::IsVampireLord(RE::TESRace* test_race)
    {
        if (test_race != nullptr)
        {
            return MPL::Managers::RaceManager::GetSingleton()->IsVampireLord(
                test_race);
        }
        else
        {
            return false;
        }
    }

    bool ModAPI::IsVampireLord(RE::Actor* actor)
    {
        if (actor != nullptr && actor->GetActorRuntimeData().race != nullptr)
        {
            return MPL::Managers::RaceManager::GetSingleton()->IsVampireLord(
                actor->GetActorRuntimeData().race);
        }
        else
        {
            return false;
        }
    }

    bool ModAPI::IsRegisteredVL(RE::TESRace* lord_race)
    {
        if (lord_race != nullptr)
        {
            return MPL::Managers::RaceManager::GetSingleton()->IsSupportedLord(
                lord_race);
        }
        else
        {
            return false;
        }
    }

    bool ModAPI::IsRegisteredHV(RE::TESRace* human_vampire)
    {
        if (human_vampire != nullptr)
        {
            return MPL::Managers::RaceManager::GetSingleton()->IsSupportedRace(
                human_vampire);
        }
        else
        {
            return false;
        }
    }

    void ModAPI::TransformNPC(RE::Actor* actor)
    {
        MPL::Managers::RaceManager::GetSingleton()->AttachWings(actor);
    }
    void ModAPI::RevertNPC(RE::Actor* actor)
    {
        MPL::Managers::RaceManager::GetSingleton()->DetachWings(actor);
    }
}  // namespace MPL::API
