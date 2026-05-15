#include <API_VLRProxy.h>
#include <Hook.h>
#include <ModAPI.h>
#include <Papyrus.h>
#include <RaceManager.h>

MPL::API::Interface g_vlrpAPI;
void APIHandler(SKSE::MessagingInterface::Message* msg)
{
    switch (msg->type)
    {
    case MPL::API::VLRPMessage::kMessage_GetInterface:
        reinterpret_cast<MPL::API::VLRPMessage*>(msg->data)->API = &g_vlrpAPI;
        break;
    }
}

void MsgHandler(SKSE::MessagingInterface::Message* msg)
{
    auto sta = MPL::Managers::RaceManager::GetSingleton();
    switch (msg->type)
    {
    case SKSE::MessagingInterface::kDataLoaded:
        sta->InitLords();
        break;
    default:
        break;
    }
}

SKSEPluginInfo(
    .Version = REL::Version{ 1, 0, 0, 0 },
    .Name = "VLRProxy"sv,
    .Author = "Mini"sv,
    .SupportEmail = ""sv,
    .StructCompatibility = SKSE::StructCompatibility::Independent,
    .RuntimeCompatibility = SKSE::VersionIndependence::AddressLibrary);

SKSEPluginLoad(const SKSE::LoadInterface* a_skse)
{
    SKSE::Init(a_skse);
    logger::info("Game version : {}", a_skse->RuntimeVersion().string());
    SKSE::GetMessagingInterface()->RegisterListener(MsgHandler);
    if (!SKSE::GetMessagingInterface()->RegisterListener(nullptr, APIHandler))
    {
        logger::info("Failed to register handler of VLRProxy");
    }
    SKSE::GetPapyrusInterface()->Register(MPL::papyrus::Bind);
    MPL::Hook::TryInstall();
    return true;
}
