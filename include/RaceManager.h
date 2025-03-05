#pragma once
namespace vlrp::managers
{
    enum HeadPartType : char
	{
		kNone,
		kArgonian,
		kElf,
		kDarkElf,
		kHighElf,
		kWoodElf,
		kHuman,
		kBreton,
		kImperial,
		kNord,
		kRedguard,
		kKhajiit,
		kOrc,
	};
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
        void PushRaceData(RaceData &);
        auto GetVLRace(const RE::TESRace *) -> const RE::TESRace *;
        auto GetHumanRace(const RE::TESRace *rc) -> const RE::TESRace *;
        bool IsVampireLord(const RE::TESRace *);
        bool IsSupportedRace(const RE::TESRace *);
        bool IsSupportedVL(const RE::TESRace *);
        void SetDOMRace(RE::TESRace *);
    };
}