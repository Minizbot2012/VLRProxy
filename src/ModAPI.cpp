#include <ModAPI.h>
namespace vlrp::API
{
    const RE::TESRace *Interface::GetVLRace(const RE::TESRace *race)
    {
        if (race != nullptr)
        {
            return vlrp::managers::RaceManager::GetSingleton()->GetVLRace(race);
        }
        else
        {
            return nullptr;
        }
    }
    const RE::TESRace *Interface::GetVampireRace(const RE::TESRace *race)
    {
        return vlrp::managers::RaceManager::GetSingleton()->GetVampireRace(race);
    }
    const RE::TESRace *Interface::GetRegularVL()
    {
        return vlrp::managers::RaceManager::GetSingleton()->GetOriginalVL();
    }
    APIRes Interface::RegisterRace(const RE::TESRace *lord_race, const RE::TESRace *human_vampire)
    {
        if (lord_race != nullptr && human_vampire != nullptr)
        {
            auto insert = vlrp::managers::RaceData{
                human_vampire,
                lord_race};
            return (APIRes)vlrp::managers::RaceManager::GetSingleton()->PushRaceData(insert);
        }
        return APIRes::Failed;
    }
    bool Interface::IsVampireLord(const RE::TESRace *test_race)
    {
        if (test_race != nullptr)
        {
            return vlrp::managers::RaceManager::GetSingleton()->IsVampireLord(test_race);
        }
        else
        {
            return false;
        }
    }
    bool Interface::IsVampireLord(const RE::Actor *actor)
    {
        if (actor != nullptr && actor->race != nullptr)
        {
            return vlrp::managers::RaceManager::GetSingleton()->IsVampireLord(actor->race);
        }
        else
        {
            return false;
        }
    }
    bool Interface::IsRegisteredVL(const RE::TESRace *lord_race)
    {
        if (lord_race != nullptr)
        {
            return vlrp::managers::RaceManager::GetSingleton()->IsSupportedVL(lord_race);
        }
        else
        {
            return false;
        }
    }
    bool Interface::IsRegisteredHV(const RE::TESRace *human_vampire)
    {
        if (human_vampire != nullptr)
        {
            return vlrp::managers::RaceManager::GetSingleton()->IsSupportedRace(human_vampire);
        }
        else
        {
            return false;
        }
    }
}