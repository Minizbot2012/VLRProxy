#pragma once
#include <Externals/MMSF_API.h>
#include <string_view>
#include <vector>
#include <Overrides.h>

namespace MPL::Managers
{
    static const std::filesystem::path SLIDER_DIR("Meshes/actors/character/FaceGenMorphs");
    struct RaceData
    {
        RE::TESRace* vampireRace;
        RE::TESRace* vlRace;
        RE::TESRace* humanRace;
    };

    struct TransformStatus
    {
        RE::Actor* actor;
        RE::TESRace* race;
    };
    class RaceManager
    {
    private:
        RaceManager();
        ~RaceManager();
        std::vector<RaceData> race_pairs;
        std::vector<TransformStatus> transform_data;
        MPL::API::MMSF::Interface* MMSF;
        MPL::API::MMSF::IFormAllocator* alloc;
        std::atomic<bool> Ready{ false };
        static inline RE::BGSKeyword* VampireLordKeyword;
        RE::TESRace* OriginalVL;
        std::unordered_map<OverrideEntry, RE::TESRace*> override_table;

    public:
        inline static RaceManager* GetSingleton()
        {
            static RaceManager Instance;
            return std::addressof(Instance);
        }
        void InitMMSF();
        void InitLords();
        void AddOverride(RE::TESRace*, std::string_view, std::string_view, RE::TESRace*);
        RE::TESRace* Filter(RE::BSScript::ObjectTypeInfo*, RE::BSScript::IFunction*, RE::TESRace*);
        RE::TESRace* Filter(RE::BSScript::Internal::VirtualMachine*, RE::VMStackID, RE::TESRace*);
        void WaitForReadySignal();
        void PatchRMInis(std::filesystem::path);
        int PushRaceData(RaceData&);
        RE::TESRace* GetLordRace(RE::TESRace*);
        RE::TESRace* GetVampireRace(RE::TESRace*);
        RE::TESRace* GetOriginalLord();
        bool IsVampireLord(RE::TESRace*);
        bool IsSupportedRace(RE::TESRace*);
        bool IsSupportedLord(RE::TESRace*);
        void AttachWings(RE::Actor*);
        void DetachWings(RE::Actor*);
    };
}  // namespace MPL::Managers
