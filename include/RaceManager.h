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

    struct ActorTransforms
    {
        std::string wingPath;
        RE::BGSArtObject* artObject;
    };

    class RaceManager
    {
    private:
        RaceManager();
        ~RaceManager();
        std::vector<RaceData> race_pairs;
        MPL::API::MMSF::Interface* MMSF;
        MPL::API::MMSF::IFormAllocator* alloc;
        RE::TESRace* OriginalVL;
        std::atomic<bool> Ready{ false };
        RE::BGSKeyword* VampireLordKeyword;
        //This whole thing is still WIP and might change, but this is included in 0.8.6 for concepting :)
        std::unordered_map<std::string, ActorTransforms> transforms = {
            { "Vrel", ActorTransforms{ .wingPath = "[Anton] mods\\Animated Dragon Wings\\Bloody Red Dragon Wings.nif", .artObject = nullptr} },
            { "Lord Harkon", ActorTransforms{ .wingPath = "[Anton] mods\\Animated Dragon Wings\\Evil Blue Dragon Wings.nif", .artObject = nullptr } }
        };

    public:
        inline static RaceManager* GetSingleton()
        {
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
}  // namespace MPL::Managers
