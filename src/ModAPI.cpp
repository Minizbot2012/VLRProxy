#include <ModAPI.h>
namespace MPL::API
{
    const RE::TESRace* Interface::GetVLRace(const RE::TESRace* race)
    {
        if (race != nullptr)
        {
            return MPL::managers::RaceManager::GetSingleton()->GetLordRace(race);
        }
        else
        {
            return nullptr;
        }
    }

    const RE::TESRace* Interface::GetVampireRace(const RE::TESRace* race)
    {
        if (race != nullptr)
        {
            return MPL::managers::RaceManager::GetSingleton()->GetVampireRace(race);
        }
        else {
            return nullptr;
        }
    }

    const RE::TESRace* Interface::GetRegularVL()
    {
        return MPL::managers::RaceManager::GetSingleton()->GetOriginalLord();
    }

    APIRes Interface::RegisterRace(const RE::TESRace* lord_race,
        const RE::TESRace* human_vampire)
    {
        if (lord_race != nullptr && human_vampire != nullptr)
        {
            auto insert = MPL::managers::RaceData{ human_vampire, lord_race };
            return (APIRes)MPL::managers::RaceManager::GetSingleton()->PushRaceData(
                insert);
        }
        return APIRes::Failed;
    }

    bool Interface::IsVampireLord(const RE::TESRace* test_race)
    {
        if (test_race != nullptr)
        {
            return MPL::managers::RaceManager::GetSingleton()->IsVampireLord(
                test_race);
        }
        else
        {
            return false;
        }
    }

    bool Interface::IsVampireLord(const RE::Actor* actor)
    {
        if (actor != nullptr && actor->race != nullptr)
        {
            return MPL::managers::RaceManager::GetSingleton()->IsVampireLord(
                actor->race);
        }
        else
        {
            return false;
        }
    }

    bool Interface::IsRegisteredVL(const RE::TESRace* lord_race)
    {
        if (lord_race != nullptr)
        {
            return MPL::managers::RaceManager::GetSingleton()->IsSupportedLord(
                lord_race);
        }
        else
        {
            return false;
        }
    }

    bool Interface::IsRegisteredHV(const RE::TESRace* human_vampire)
    {
        if (human_vampire != nullptr)
        {
            return MPL::managers::RaceManager::GetSingleton()->IsSupportedRace(
                human_vampire);
        }
        else
        {
            return false;
        }
    }
    /*
    bool Interface::Transform(RE::Actor* actor,
        RE::TESRace* optional_race = nullptr)
    {
        return MPL::managers::RaceManager::GetSingleton()->TransformActor(
            actor, optional_race);
    }

    bool Interface::Revert(RE::Actor* actor)
    {
        return MPL::managers::RaceManager::GetSingleton()->RevertActor(actor);
    }
    */
}  // namespace MPL::API