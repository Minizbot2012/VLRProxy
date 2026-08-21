#pragma once
#include <Externals/MMSF_API.h>
#include <unordered_map>
#include <vector>
namespace MPL::Managers
{
    static const std::filesystem::path SLIDER_DIR("Meshes/actors/character/FaceGenMorphs");
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

    class RaceManager
    {
    private:
        std::vector<RaceData> race_pairs;
        std::unordered_map<std::string, ActorTransforms> transforms = {
            {"Vrel", ActorTransforms { .wingPath = "[Anton] mods\\Animated Dragon Wings\\Bloody Red Dragon Wings.nif" }},
            {"Lord Harkon", ActorTransforms { .wingPath = "[Anton] mods\\Animated Dragon Wings\\Evil Blue Dragon Wings.nif" }}
        };
        MPL::API::MMSF::Interface* MMSF;
        RE::TESRace* OriginalVL;
        std::atomic<bool> Ready {false};
        ~RaceManager();
        RaceManager();
    public:
        inline static RaceManager* GetSingleton() {
            static RaceManager Instance;
            return std::addressof(Instance);
        }
        void InitMMSF();
        void InitLords();
        void WaitForReadySignal();
        void PatchRMInis(std::filesystem::path);
        int PushRaceData(RaceData&);
        auto GetLordRace(RE::TESRace*) -> RE::TESRace*;
        auto GetVampireRace(RE::TESRace*) -> RE::TESRace*;
        auto GetOriginalLord() -> RE::TESRace*;
        bool IsVampireLord(RE::TESRace*);
        bool IsSupportedRace(RE::TESRace*);
        bool IsSupportedLord(RE::TESRace*);
        void AttachWings(RE::Actor*);
        void DetachWings(RE::Actor*);
    };
}  // namespace MPL::managers
