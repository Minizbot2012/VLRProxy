#pragma once
#include <VLRProxy_API.h>
#include <RaceManager.h>
#include <cstdint>
namespace MPL::API::VLRProxy
{
    class ModAPI : public Interface
    {
    public:
        uint64_t GetVersion() override;
        void WaitForReadySignal() override;
        RE::TESRace* GetVLRace(RE::TESRace*) override;
        RE::TESRace* GetVampireRace(RE::TESRace*) override;
        RE::TESRace* GetRegularVL() override;
        bool IsVampireLord(RE::TESRace*) override;
        bool IsVampireLord(RE::Actor*) override;
        bool IsSupportedVL(RE::TESRace*) override;
        bool IsSupportedVH(RE::TESRace*) override;
        void TransformNPC(RE::Actor*) override;
        void RevertNPC(RE::Actor*) override;
    };
}  // namespace MPL::API
