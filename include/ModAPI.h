#pragma once
#include <API_VLRProxy.h>
#include <RaceManager.h>
#include <Config.h>
namespace vlrp::API
{
    class Interface : public CurrentInterface, public ISingleton<Interface>
    {
    public:
        //API V1
        const RE::TESRace *GetVLRace(const RE::TESRace *race) override;
        const RE::TESRace *GetVampireRace(const RE::TESRace *race) override;
        const RE::TESRace *GetRegularVL() override;
        APIRes RegisterRace(const RE::TESRace *lord_race, const RE::TESRace *human_vampire) override;
        bool IsVampireLord(const RE::TESRace *test_race) override;
        bool IsVampireLord(const RE::Actor *actor) override;
        bool IsRegisteredVL(const RE::TESRace *lord_race) override;
        bool IsRegisteredHV(const RE::TESRace *human_vampire) override;
    };
}