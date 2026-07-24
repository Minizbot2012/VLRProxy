#pragma once
#include <SKSE/API.h>
#include <SKSE/Interfaces.h>
#include <cstdint>
namespace MPL::API::VLRProxy
{
    enum struct VLRAPIRes : int8_t
    {
        Failed = -1,
        Ok,
        Overriden
    };

    class Interface
    {
    public:
        virtual uint16_t GetVersion() = 0;
        virtual RE::TESRace* GetVLRace(RE::TESRace*) = 0;
        virtual RE::TESRace* GetVampireRace(RE::TESRace*) = 0;
        virtual RE::TESRace* GetRegularVL() = 0;
        virtual bool IsVampireLord(RE::TESRace*) = 0;
        virtual bool IsVampireLord(RE::Actor*) = 0;
        virtual bool IsRegisteredVL(RE::TESRace*) = 0;
        virtual bool IsRegisteredHV(RE::TESRace*) = 0;
        virtual void TransformNPC(RE::Actor*) = 0;
        virtual void RevertNPC(RE::Actor*) = 0;
    };

    static const char* sender = "VLRProxy";
    struct VLRPMessage
    {
        enum message_type : uint32_t
        {
            kMessage_GetInterface = 'VLRP'
        };
        Interface* API;
    };

    [[nodiscard]] inline Interface* RequestVLAPI()
    {
        VLRPMessage message;
        SKSE::GetMessagingInterface()->Dispatch(VLRPMessage::kMessage_GetInterface, &message, sizeof(VLRPMessage), sender);
        if (message.API)
        {
            return message.API;
        }
        return nullptr;
    }
}  // namespace MPL::API
