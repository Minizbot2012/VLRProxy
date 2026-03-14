#pragma once
#include <SKSE/API.h>
#include <SKSE/Interfaces.h>
#include <cstdint>
namespace MPL::API
{
    enum struct APIRes : int8_t
    {
        Failed = -1,
        Ok,
        Overriden
    };

    enum struct APIVersion : uint8_t
    {
        V1 = 1,
        Current = V1
    };

    struct APIV1
    {
    public:
        [[nodiscard]] virtual const RE::TESRace* GetVLRace(const RE::TESRace*) = 0;
        [[nodiscard]] virtual const RE::TESRace* GetVampireRace(const RE::TESRace*) = 0;
        [[nodiscard]] virtual const RE::TESRace* GetRegularVL() = 0;
        [[nodiscard]] virtual APIRes RegisterRace(const RE::TESRace*, const RE::TESRace*) = 0;
        [[nodiscard]] virtual bool IsVampireLord(const RE::TESRace*) = 0;
        [[nodiscard]] virtual bool IsVampireLord(const RE::Actor*) = 0;
        [[nodiscard]] virtual bool IsRegisteredVL(const RE::TESRace*) = 0;
        [[nodiscard]] virtual bool IsRegisteredHV(const RE::TESRace*) = 0;
    };

    using CurrentInterface = APIV1;
    typedef void* (*_RequestPluginAPI)(const APIVersion);

    struct VLRPMessage
    {
        enum message_type : uint32_t
        {
            kMessage_GetInterface = 0x1a208e02
        };
        _RequestPluginAPI GetAPI;
    };

    [[nodiscard]] inline void* RequestPluginAPI(const APIVersion ver = APIVersion::Current)
    {
        VLRPMessage message;
        SKSE::GetMessagingInterface()->Dispatch('VLRP', (void*)&message, sizeof(VLRPMessage), "VLRProxy");
        if (message.GetAPI)
        {
            return message.GetAPI(ver);
        }
        return nullptr;
    }
}  // namespace MPL::API
