#include <Config.h>
#include <Hook.h>
#include <ModAPI.h>
#include <Papyrus.h>
#include <RaceManager.h>

void OnInit(SKSE::MessagingInterface::Message* a_msg)
{
    switch (a_msg->type)
    {
    case SKSE::MessagingInterface::kDataLoaded:
        vlrp::config::LoadConfigs();
        break;
#ifndef SKYRIM_AE
    case SKSE::MessagingInterface::kPostLoadGame:
        if (vlrp::managers::RaceManager::GetSingleton()->IsVampireLord(
                RE::PlayerCharacter::GetSingleton()->GetRace()))
        {
            vlrp::managers::RaceManager::GetSingleton()->SetDOMRace(
                RE::PlayerCharacter::GetSingleton()->GetRace());
        }
        break;
#endif
    default:
        break;
    }
}

extern "C" DLLEXPORT auto SKSEPlugin_Version = []() {
    SKSE::PluginVersionData v;
    v.PluginVersion({ 0, 7, 0, 0 });
    v.PluginName("VLRProxy");
    v.AuthorName("mini");
    v.UsesAddressLibrary();
    v.UsesNoStructs();

    return v;
}();

void SaveCallback(SKSE::SerializationInterface* a_intf)
{
    vlrp::managers::RaceManager::GetSingleton()->Save(a_intf);
}

void LoadCallback(SKSE::SerializationInterface* a_intf)
{
    vlrp::managers::RaceManager::GetSingleton()->Load(a_intf);
}

void RevertCallback(SKSE::SerializationInterface* a_intf)
{
    vlrp::managers::RaceManager::GetSingleton()->Revert(a_intf);
}

extern "C" DLLEXPORT bool SKSEAPI
SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
    SKSE::Init(a_skse);
    logger::info("Game version : {}", a_skse->RuntimeVersion().string());
    if (!SKSE::GetMessagingInterface()->RegisterListener(OnInit))
    {
        return false;
    }
    SKSE::GetPapyrusInterface()->Register(vlrp::papyrus::Bind);
    auto a_intf = SKSE::GetSerializationInterface();
    a_intf->SetUniqueID('VLRP');
    a_intf->SetLoadCallback(LoadCallback);
    a_intf->SetSaveCallback(SaveCallback);
    a_intf->SetRevertCallback(RevertCallback);
    vlrp::hook::TryInstall();
#ifndef SKYRIM_AE
    vlrp::managers::RaceManager::GetSingleton()->Register();
#endif
    return true;
}

extern "C" DLLEXPORT void* SKSEAPI RequestPluginAPI(vlrp::API::APIVersion ver)
{
    switch (ver)
    {
    case vlrp::API::APIVersion::V1:
        return vlrp::API::Interface::GetSingleton();
        break;
    }
    return nullptr;
}