#pragma once
#include <API_VLRProxy.h>
#include <Config.h>
#include <RaceManager.h>
namespace MPL::API
{
    class Interface : public CurrentInterface, public REX::Singleton<Interface>
    {
    public:
        // API V1
        const RE::TESRace* GetVLRace(const RE::TESRace* race) override;
        const RE::TESRace* GetVampireRace(const RE::TESRace* race) override;
        const RE::TESRace* GetRegularVL() override;
        APIRes RegisterRace(const RE::TESRace* lord_race,
            const RE::TESRace* human_vampire) override;
        bool IsVampireLord(const RE::TESRace* test_race) override;
        bool IsVampireLord(const RE::Actor* actor) override;
        bool IsRegisteredVL(const RE::TESRace* lord_race) override;
        bool IsRegisteredHV(const RE::TESRace* human_vampire) override;
    };
}  // namespace MPL::API
