#pragma once
#include <MMSF_API.h>
namespace MPL::Managers
{
    struct RaceData
    {
        RE::TESRace* vampireRace;
        RE::TESRace* vlRace;
    };


    class RaceManager : public REX::Singleton<RaceManager>
    {
    private:
        std::vector<RaceData> race_pairs;
        RE::TESRace* OriginalVL;
        MPL::API::ServiceMap* MMSF;
        bool conf_loaded;
        std::mutex _lock;

    public:
        void InitLords();
        int PushRaceData(RaceData&);
        auto GetLordRace(RE::TESRace*) -> RE::TESRace*;
        auto GetVampireRace(RE::TESRace*) -> RE::TESRace*;
        auto GetOriginalLord() -> RE::TESRace*;
        bool IsVampireLord(RE::TESRace*);
        bool IsSupportedRace(RE::TESRace*);
        bool IsSupportedLord(RE::TESRace*);
    };
}  // namespace MPL::managers
