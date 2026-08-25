#include <Hook.h>
#include <ModAPI.h>
#include <Papyrus.h>
#include <Plugin.h>
#include <RaceManager.h>
#include <VLRProxy_API.h>
MPL::API::VLRProxy::ModAPI g_vlrpAPI;
void APIHandler(SKSE::MessagingInterface::Message* msg)
{
    switch (msg->type)
    {
    case MPL::API::VLRProxy::VLRPMessage::kMessage_GetInterface:
        reinterpret_cast<MPL::API::VLRProxy::VLRPMessage*>(msg->data)->API = &g_vlrpAPI;
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
    .Version = REL::Version{ MPL::Plugin::MAJOR, MPL::Plugin::MINOR, MPL::Plugin::PATCH, 0 },
    .Name = "VLRProxy",
    .Author = "Mini"sv,
    .SupportEmail = ""sv,
    .StructCompatibility = SKSE::StructCompatibility::Independent,
    .RuntimeCompatibility = SKSE::VersionIndependence::AddressLibrary
);

SKSEPluginLoad(const SKSE::LoadInterface* a_skse)
{
    SKSE::Init(a_skse);
    logger::info("Game version : {}", a_skse->RuntimeVersion().string());
    SKSE::GetMessagingInterface()->RegisterListener(MsgHandler);
    SKSE::GetMessagingInterface()->RegisterListener(nullptr, APIHandler);
    SKSE::GetPapyrusInterface()->Register(MPL::papyrus::Bind);
    MPL::Hooks::Install();
    return true;
}
