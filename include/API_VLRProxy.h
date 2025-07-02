#pragma once
namespace vlrp::API
{
    enum APIRes : int8_t
    {
        Failed = -1,
        Ok,
        Overriden
    };
    enum APIVersion : uint8_t
    {
        V1 = 1,
        Current = V1
    };
    class APIV1
    {
    public:
        [[nodiscard]] virtual const RE::TESRace *GetVLRace(const RE::TESRace *race) = 0;
        [[nodiscard]] virtual const RE::TESRace *GetVampireRace(const RE::TESRace *race) = 0;
        [[nodiscard]] virtual const RE::TESRace *GetRegularVL() = 0;
        [[nodiscard]] virtual APIRes RegisterRace(const RE::TESRace *lord_race, const RE::TESRace *human_vampire) = 0;
        [[nodiscard]] virtual bool IsVampireLord(const RE::TESRace *test_race) = 0;
        [[nodiscard]] virtual bool IsVampireLord(const RE::Actor *actor) = 0;
        [[nodiscard]] virtual bool IsRegisteredVL(const RE::TESRace *lord_race) = 0;
        [[nodiscard]] virtual bool IsRegisteredHV(const RE::TESRace *human_vampire) = 0;
    };
    using CurrentInterface = APIV1;
    typedef void *(*_RequestPluginAPI)(const APIVersion ver);
    [[nodiscard]] inline void *RequestPluginAPI(const APIVersion ver = APIVersion::Current)
    {
        auto pluginHandle = GetModuleHandleW(L"VLRProxy.dll");
        _RequestPluginAPI reqAPI = (_RequestPluginAPI)GetProcAddress(pluginHandle, "RequestPluginAPI");
        if (reqAPI)
        {
            return reqAPI(ver);
        }
        return nullptr;
    }
}