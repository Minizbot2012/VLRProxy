#include <ModAPI.h>
namespace MPL::API
{
    RE::TESRace* Interface::GetVLRace(RE::TESRace* race)
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

    RE::TESRace* Interface::GetVampireRace(RE::TESRace* race)
    {
        if (race != nullptr)
        {
            return MPL::Managers::RaceManager::GetSingleton()->GetVampireRace(race);
        }
        else {
            return nullptr;
        }
    }

    RE::TESRace* Interface::GetRegularVL()
    {
        return MPL::Managers::RaceManager::GetSingleton()->GetOriginalLord();
    }

    bool Interface::IsVampireLord(RE::TESRace* test_race)
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

    bool Interface::IsVampireLord(RE::Actor* actor)
    {
        if (actor != nullptr && actor->race != nullptr)
        {
            return MPL::Managers::RaceManager::GetSingleton()->IsVampireLord(
                actor->race);
        }
        else
        {
            return false;
        }
    }

    bool Interface::IsRegisteredVL(RE::TESRace* lord_race)
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

    bool Interface::IsRegisteredHV(RE::TESRace* human_vampire)
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
}  // namespace MPL::API
