#include <Config.h>
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
extern "C" DLLEXPORT bool SKSEAPI
SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
    SKSE::Init(a_skse);
    logger::info("Game version : {}", a_skse->RuntimeVersion().string());
    SKSE::GetPapyrusInterface()->Register(MPL::papyrus::Bind);
    MPL::Hook::TryInstall();
    return true;
}

extern "C" DLLEXPORT void* SKSEAPI RequestPluginAPI(MPL::API::APIVersion ver)
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
