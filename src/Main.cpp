#include <API_VLRProxy.h>
#include <Hook.h>
#include <ModAPI.h>
#include <Papyrus.h>
#include <RaceManager.h>
#include <memory>

extern "C" DLLEXPORT auto SKSEPlugin_Version = []() {
    SKSE::PluginVersionData v;
    v.PluginVersion({ 0, 7, 0, 0 });
    v.PluginName("VLRProxy");
    v.AuthorName("mini");
    v.UsesAddressLibrary();
    v.UsesUpdatedStructs();

    return v;
}();

void* RequestPluginAPI(MPL::API::APIVersion ver)
{
    static MPL::API::Interface API;
    switch (ver)
    {
    case MPL::API::APIVersion::V1:
        return std::addressof(API);
        break;
    }
    return nullptr;
}

void MsgHandler(SKSE::MessagingInterface::Message* msg)
{
    auto sta = MPL::Managers::RaceManager::GetSingleton();
    switch (msg->type)
    {
    case SKSE::MessagingInterface::kDataLoaded:
        sta->InitLords();
        break;
    case MPL::API::VLRPMessage::kMessage_GetInterface:
        ((MPL::API::VLRPMessage*)(msg->data))->GetAPI = RequestPluginAPI;
        break;
    default:
        break;
    }
}

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
    SKSE::Init(a_skse);
    logger::info("Game version : {}", a_skse->RuntimeVersion().string());
    SKSE::GetMessagingInterface()->RegisterListener(MsgHandler);
    SKSE::GetPapyrusInterface()->Register(MPL::papyrus::Bind);
    MPL::Hook::TryInstall();
    return true;
}
