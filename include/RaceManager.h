#pragma once
namespace vlrp::managers
{
    struct RaceData
    {
        const RE::TESRace *vampireRace;
        const RE::TESRace *vlRace;
    };
    class RaceManager : public ISingleton<RaceManager>
    {
    private:
        std::vector<RaceData> race_pairs;

    public:
        void Reset();
        void UpdateFormLists();
        void PushRaceData(RaceData &);
        auto GetVLRace(const RE::TESRace *) -> const RE::TESRace *;
        auto GetHumanRace(const RE::TESRace *rc) -> const RE::TESRace *;
        bool IsVampireLord(const RE::TESRace *);
        bool IsSupportedRace(const RE::TESRace *);
        bool IsSupportedVL(const RE::TESRace *);
        void SetDOMRace(RE::TESRace *);
    };
}