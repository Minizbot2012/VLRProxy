#pragma once
#include <algorithm>
#include <Config.h>
namespace vlrp::managers
{
    struct RaceData
    {
        const RE::TESRace *vampireRace;
        const RE::TESRace *vlRace;
    };

    struct TransformData
    {
        const RE::Actor *actor;
        const RE::TESRace *original_race;
        const RE::TESRace *transformed_race;
        const RE::TESForm *left_hand;
        const RE::TESForm *right_hand;
        const RE::TESForm *power;
    };

    class RaceManager : public REX::Singleton<RaceManager>, public RE::BSTEventSink<RE::TESSwitchRaceCompleteEvent>
    {
    private:
        std::vector<RaceData> race_pairs;
        std::vector<TransformData> transforms;
        const RE::TESRace *OriginalVL;
        std::mutex _lock;
    protected:
        RE::BSEventNotifyControl ProcessEvent(const RE::TESSwitchRaceCompleteEvent *a_event, RE::BSTEventSource<RE::TESSwitchRaceCompleteEvent> *) override;

    public:
        void Reset();
        static void Register();
        int PushRaceData(RaceData &);
        void Save(SKSE::SerializationInterface *inf);
        void Load(SKSE::SerializationInterface *inf);
        void Revert(SKSE::SerializationInterface *inf);
        auto GetVLRace(const RE::TESRace *) -> const RE::TESRace *;
        auto GetVampireRace(const RE::TESRace *) -> const RE::TESRace *;
        auto GetOriginalVL() -> const RE::TESRace *;
        bool IsVampireLord(const RE::TESRace *);
        bool IsSupportedRace(const RE::TESRace *);
        bool IsSupportedVL(const RE::TESRace *);
        void SetDOMRace(RE::TESRace *);
        bool TransformActor(RE::Actor *, RE::TESRace *);
        bool RevertActor(RE::Actor *);
    };
}