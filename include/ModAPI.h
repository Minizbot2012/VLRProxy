#pragma once
#include <RaceManager.h>
#include <VLRProxy_API.h>
#include <cstdint>
#include <winuser.h>

namespace MPL::API::VLRProxy
{
    class ModAPI : public Interface
    {
    private:
        ModAPI() = default;
        ~ModAPI() = default;

    public:
        static ModAPI* GetSingleton()
        {
            static ModAPI api;
            return &api;
        };
        std::string GetName() override { return "VLRP"; };
        uint8_t GetVersion() override { return 1; };
        void Initialize() override {};
        rfl::Generic::Object Save() override { return rfl::Generic::Object(); }
        void Load(rfl::Generic::Object) override{};
        void WaitForReadySignal() override;
        RE::TESRace* GetVLRace(RE::TESRace*) override;
        RE::TESRace* GetVampireRace(RE::TESRace*) override;
        RE::TESRace* GetRegularVL() override;
        bool IsVampireLord(RE::TESRace*) override;
        bool IsVampireLord(RE::Actor*) override;
        bool IsSupportedVL(RE::TESRace*) override;
        bool IsSupportedVH(RE::TESRace*) override;
        void Transform(RE::Actor*) override;
        void Revert(RE::Actor*) override;
    };
}  // namespace MPL::API::VLRProxy
