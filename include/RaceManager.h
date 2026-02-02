#pragma once
#include <Config.h>
namespace MPL::managers
{
    struct RaceData
    {
        const RE::TESRace* vampireRace;
        const RE::TESRace* vlRace;
    };

    struct TransformData
    {
        const RE::Actor* actor;
        const RE::TESRace* original_race;
        const RE::TESRace* transformed_race;
    };

    class RaceManager : public REX::Singleton<RaceManager>
    {
    private:
        std::vector<RaceData> race_pairs;
        std::vector<TransformData> transforms;
        const RE::TESRace* OriginalVL;
        std::mutex _lock;
        bool conf_loaded;
        void Reset();
        void LoadConfig();

    public:
        int PushRaceData(RaceData&);
        auto GetLordRace(const RE::TESRace*) -> const RE::TESRace*;
        auto GetVampireRace(const RE::TESRace*) -> const RE::TESRace*;
        auto GetOriginalLord() -> const RE::TESRace*;
        bool IsVampireLord(const RE::TESRace*);
        bool IsSupportedRace(const RE::TESRace*);
        bool IsSupportedLord(const RE::TESRace*);
    };
}  // namespace MPL::managers
