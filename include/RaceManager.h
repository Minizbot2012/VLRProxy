#pragma once
#include <Config.h>
namespace vlrp::managers
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

#ifdef SKYRIM_AE
    class RaceManager : public REX::Singleton<RaceManager>
#else
    class RaceManager : public REX::Singleton<RaceManager>, public RE::BSTEventSink<RE::TESSwitchRaceCompleteEvent>
#endif
    {
    private:
        std::vector<RaceData> race_pairs;
        std::vector<TransformData> transforms;
        const RE::TESRace* OriginalVL;
        std::mutex _lock;
        bool conf_loaded;
        void Reset();
        void LoadConfig();
#ifndef SKYRIM_AE
    protected:
        RE::BSEventNotifyControl
        ProcessEvent(const RE::TESSwitchRaceCompleteEvent* a_event,
            RE::BSTEventSource<RE::TESSwitchRaceCompleteEvent>*) override;
#endif
    public:
        int PushRaceData(RaceData&);
        void Save(SKSE::SerializationInterface* inf);
        void Load(SKSE::SerializationInterface* inf);
        void Revert(SKSE::SerializationInterface* inf);
        auto GetLordRace(const RE::TESRace*) -> const RE::TESRace*;
        auto GetVampireRace(const RE::TESRace*) -> const RE::TESRace*;
        auto GetOriginalLord() -> const RE::TESRace*;
        bool IsVampireLord(const RE::TESRace*);
        bool IsSupportedRace(const RE::TESRace*);
        bool IsSupportedLord(const RE::TESRace*);
        bool TransformActor(RE::Actor*, RE::TESRace*);
        bool RevertActor(RE::Actor*);
#ifndef SKYRIM_AE
        void Register();
        void SetDOMRace(RE::TESRace*);
#endif
    };
}  // namespace vlrp::managers