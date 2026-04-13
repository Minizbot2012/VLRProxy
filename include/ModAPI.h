#pragma once
#include <API_VLRProxy.h>
#include <RaceManager.h>
namespace MPL::API
{
    class Interface : public CurrentInterface
    {
    public:
        // API V1
        RE::TESRace* GetVLRace(RE::TESRace* race) override;
        RE::TESRace* GetVampireRace(RE::TESRace* race) override;
        RE::TESRace* GetRegularVL() override;
        bool IsVampireLord(RE::TESRace* test_race) override;
        bool IsVampireLord(RE::Actor* actor) override;
        bool IsRegisteredVL(RE::TESRace* lord_race) override;
        bool IsRegisteredHV(RE::TESRace* human_vampire) override;
    };
}  // namespace MPL::API
