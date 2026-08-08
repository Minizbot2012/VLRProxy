#pragma once
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
        //Version of the interface
        virtual uint64_t GetVersion() = 0;
        //Not to be called on the main thread, will wait for Lord Initialization and return
        virtual void WaitForReadySignal() = 0;
        //Get the VL Race for a TESRace
        virtual RE::TESRace* GetVLRace(RE::TESRace*) = 0;
        //Get the Vampire Race for a VLTESRace
        virtual RE::TESRace* GetVampireRace(RE::TESRace*) = 0;
        //Simply returns the DLC1VampireBeastRace
        virtual RE::TESRace* GetRegularVL() = 0;
        //Is the race any VampireLord (including DLC1VampireBeastRace)
        virtual bool IsVampireLord(RE::TESRace*) = 0;
        //Same as above but for an actor
        virtual bool IsVampireLord(RE::Actor*) = 0;
        //Checks if the race is a supported VL, E.G. humanoid VL
        virtual bool IsSupportedVL(RE::TESRace*) = 0;
        //Checks if the race is a supported vampire race, E.G. this race has a vampire lord form
        virtual bool IsSupportedVH(RE::TESRace*) = 0;
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
