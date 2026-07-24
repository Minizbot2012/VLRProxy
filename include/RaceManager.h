#pragma once
#include "RE/R/ReferenceEffect.h"
#include <Externals/MMSF_API.h>
namespace MPL::Managers
{
    struct RaceData
    {
        RE::TESRace* vampireRace;
        RE::TESRace* vlRace;
        RE::TESRace* humanRace;
    };

    struct ActorTransforms {
        std::string wingPath;
        RE::BGSArtObject* artObject;
    };

    class RaceManager : public REX::Singleton<RaceManager>
    {
    private:
        std::vector<RaceData> race_pairs;
        std::unordered_map<std::string, ActorTransforms> transforms = {
            {"Vrel", ActorTransforms { .wingPath = "[Anton] mods\\Animated Dragon Wings\\Bloody Red Dragon Wings.nif" }}
        };
        MPL::API::MMSF::ServiceMap* MMSF;
        RE::TESRace* OriginalVL;
        bool lords_initialized;
        std::mutex _lock;

    public:
        void InitMMSF();
        void InitLords();
        int PushRaceData(RaceData&);
        auto GetLordRace(RE::TESRace*) -> RE::TESRace*;
        auto GetVampireRace(RE::TESRace*) -> RE::TESRace*;
        auto GetOriginalLord() -> RE::TESRace*;
        bool IsVampireLord(RE::TESRace*);
        bool IsSupportedRace(RE::TESRace*);
        bool IsSupportedLord(RE::TESRace*);
        void AttachWings(RE::Actor*);
        void DetachWings(RE::Actor*);
        void ClearTransforms();
    };
}  // namespace MPL::managers
