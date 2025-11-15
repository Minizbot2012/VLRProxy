#pragma once
namespace MPL::API
{
    enum struct APIRes : int8_t
    {
        Failed = -1,
        Ok,
        Overriden
    };

    enum struct APIVersion : uint8_t
    {
        V1 = 1,
        Current = V1
    };

    class APIV1
    {
    public:
        [[nodiscard]] virtual const RE::TESRace* GetVLRace(const RE::TESRace*) = 0;
        [[nodiscard]] virtual const RE::TESRace*
        GetVampireRace(const RE::TESRace*) = 0;
        [[nodiscard]] virtual const RE::TESRace* GetRegularVL() = 0;
        [[nodiscard]] virtual APIRes RegisterRace(const RE::TESRace*,
            const RE::TESRace*) = 0;
        [[nodiscard]] virtual bool IsVampireLord(const RE::TESRace*) = 0;
        [[nodiscard]] virtual bool IsVampireLord(const RE::Actor*) = 0;
        [[nodiscard]] virtual bool IsRegisteredVL(const RE::TESRace*) = 0;
        [[nodiscard]] virtual bool IsRegisteredHV(const RE::TESRace*) = 0;
        /*
        [[nodiscard]] virtual bool Transform(RE::Actor*, RE::TESRace*) = 0;
        [[nodiscard]] virtual bool Revert(RE::Actor*) = 0;
        */
    };

    using CurrentInterface = APIV1;
    typedef void* (*_RequestPluginAPI)(const APIVersion);

    [[nodiscard]] inline void* RequestPluginAPI(const APIVersion ver = APIVersion::Current)
    {
        auto pluginHandle = GetModuleHandle(L"VLRProxy.dll");
        _RequestPluginAPI reqAPI =
            (_RequestPluginAPI)GetProcAddress(pluginHandle, "RequestPluginAPI");
        if (reqAPI)
        {
            return reqAPI(ver);
        }
        return nullptr;
    }
}  // namespace MPL::API