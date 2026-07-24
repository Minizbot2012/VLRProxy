#pragma once
#include <VLRProxy_API.h>
#include <RaceManager.h>
#include <cstdint>
namespace MPL::API::VLRProxy
{
    class ModAPI : public Interface
    {
    public:
        uint16_t GetVersion() override;
        RE::TESRace* GetVLRace(RE::TESRace*) override;
        RE::TESRace* GetVampireRace(RE::TESRace*) override;
        RE::TESRace* GetRegularVL() override;
        bool IsVampireLord(RE::TESRace*) override;
        bool IsVampireLord(RE::Actor*) override;
        bool IsRegisteredVL(RE::TESRace*) override;
        bool IsRegisteredHV(RE::TESRace*) override;
        void TransformNPC(RE::Actor*) override;
        void RevertNPC(RE::Actor*) override;
    };
}  // namespace MPL::API
