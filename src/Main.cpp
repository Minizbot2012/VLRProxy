#include <Config.h>
#include <Hook.h>
#include <ModAPI.h>
#include <Papyrus.h>
#include <RaceManager.h>

extern "C" DLLEXPORT auto SKSEPlugin_Version = []() {
    SKSE::PluginVersionData v;
    v.PluginVersion({ 0, 7, 0, 0 });
    v.PluginName("VLRProxy");
    v.AuthorName("mini");
    v.UsesAddressLibrary();
    v.UsesUpdatedStructs();

    return v;
}();

void SaveCallback(SKSE::SerializationInterface* a_intf)
{
    MPL::managers::RaceManager::GetSingleton()->Save(a_intf);
}

void LoadCallback(SKSE::SerializationInterface* a_intf)
{
    MPL::managers::RaceManager::GetSingleton()->Load(a_intf);
}

void RevertCallback(SKSE::SerializationInterface* a_intf)
{
    MPL::managers::RaceManager::GetSingleton()->Revert(a_intf);
}

extern "C" DLLEXPORT bool SKSEAPI
SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
    SKSE::Init(a_skse);
    logger::info("Game version : {}", a_skse->RuntimeVersion().string());
    SKSE::GetPapyrusInterface()->Register(MPL::papyrus::Bind);
    /*
    auto a_intf = SKSE::GetSerializationInterface();
    a_intf->SetUniqueID('VLRP');
    a_intf->SetLoadCallback(LoadCallback);
    a_intf->SetSaveCallback(SaveCallback);
    a_intf->SetRevertCallback(RevertCallback);
    */
    MPL::Hook::TryInstall();
    return true;
}

extern "C" DLLEXPORT void* SKSEAPI RequestPluginAPI(MPL::API::APIVersion ver)
{
    switch (ver)
    {
    case MPL::API::APIVersion::V1:
        return MPL::API::Interface::GetSingleton();
        break;
    }
    return nullptr;
}
