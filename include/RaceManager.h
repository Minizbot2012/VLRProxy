#pragma once
#include <cstdint>
#include <unordered_map>
#include <rfl/msgpack.hpp>
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
        std::unordered_map<std::string, uint32_t> race_formid_cache;
        bool conf_loaded;
        std::mutex _lock;
        void LoadConfig();

    public:
        RE::FormID baseID = 0x0;
        uint32_t offset = 0x0;
        int PushRaceData(RaceData&);
        auto GetLordRace(RE::TESRace*) -> RE::TESRace*;
        auto GetVampireRace(RE::TESRace*) -> RE::TESRace*;
        auto GetOriginalLord() -> RE::TESRace*;
        bool IsVampireLord(RE::TESRace*);
        bool IsSupportedRace(RE::TESRace*);
        bool IsSupportedLord(RE::TESRace*);
    };
}  // namespace MPL::managers
