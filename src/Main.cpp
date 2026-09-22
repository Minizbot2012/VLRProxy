#include <Externals/MMSF_API.h>
#include <Hook.h>
#include <ModAPI.h>
#include <Papyrus.h>
#include <Plugin.h>
#include <RaceManager.h>
#include <VLRProxy_API.h>
#include <winnt.h>

void MsgHandler(SKSE::MessagingInterface::Message* msg)
{
    auto sta = MPL::Managers::RaceManager::GetSingleton();
    switch (msg->type)
    {
    case MPL::API::MMSF::MMSFMessage::kMessage_MMSFServicesReg:
        static_cast<MPL::API::MMSF::MMSFMessage*>(msg->data)->API->RegisterService(MPL::API::VLRProxy::ModAPI::GetSingleton());
        break;
    case MPL::API::MMSF::MMSFMessage::kMessage_MMSFServicesReady:
        sta->InitLords();
        break;
    default:
        break;
    }
}

SKSEPluginInfo(
        .Version = REL::Version{ MPL::Plugin::MAJOR, MPL::Plugin::MINOR, MPL::Plugin::PATCH, 0 },
    .Name = MPL::Plugin::PROJECT,
    .Author = "Mini"sv,
    .SupportEmail = ""sv,
    .StructCompatibility = SKSE::StructCompatibility::Independent,
    .RuntimeCompatibility = SKSE::VersionIndependence::AddressLibrary);

SKSEPluginLoad(const SKSE::LoadInterface* a_skse)
{
    SKSE::InitInfo info;
    info.trampoline = true;
    info.trampolineSize = 20 * 14;
    SKSE::Init(a_skse, info);
    logger::info("Game version : {}", a_skse->RuntimeVersion().string());
    SKSE::GetMessagingInterface()->RegisterListener(MsgHandler);
    SKSE::GetMessagingInterface()->RegisterListener("MMSF", MsgHandler);
    SKSE::GetPapyrusInterface()->Register(MPL::papyrus::Bind);
    MPL::Hooks::Install();
    return true;
}
