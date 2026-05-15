#pragma once
#include <SKSE/API.h>
#include <SKSE/Interfaces.h>
#include <cstdint>
namespace MPL::API
{
    enum struct VLRAPIRes : int8_t
    {
        Failed = -1,
        Ok,
        Overriden
    };

    enum struct VLAPIVersion : uint8_t
    {
        V1 = 1,
        Current = V1
    };

    class VLRP_APIV1
    {
    public:
        [[nodiscard]] virtual RE::TESRace* GetVLRace(RE::TESRace*) = 0;
        [[nodiscard]] virtual RE::TESRace* GetVampireRace(RE::TESRace*) = 0;
        [[nodiscard]] virtual RE::TESRace* GetRegularVL() = 0;
        [[nodiscard]] virtual bool IsVampireLord(RE::TESRace*) = 0;
        [[nodiscard]] virtual bool IsVampireLord(RE::Actor*) = 0;
        [[nodiscard]] virtual bool IsRegisteredVL(RE::TESRace*) = 0;
        [[nodiscard]] virtual bool IsRegisteredHV(RE::TESRace*) = 0;
    };

    using CurrentInterface = VLRP_APIV1;
    typedef void* (*_RequestPluginAPI)(const VLAPIVersion);

    struct VLRPMessage
    {
        enum message_type : uint32_t
        {
            kMessage_GetInterface = 'VLRP'
        };
        CurrentInterface* API;
    };

    [[nodiscard]] inline CurrentInterface* RequestVLAPI()
    {
        VLRPMessage message;
        SKSE::GetMessagingInterface()->Dispatch(VLRPMessage::kMessage_GetInterface, &message, sizeof(VLRPMessage), nullptr);
        if (message.API)
        {
            return message.API;
        }
        return nullptr;
    }
}  // namespace MPL::API
