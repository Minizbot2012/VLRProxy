#pragma once
#include <RaceManager.h>
namespace vlrp::config
{
    struct RaceConfig
    {
        RE::FormID VampireRace;
        RE::FormID VLRace;
    };
    static std::string ConfigFolder = "./Data/SKSE/Plugins/VLRP/";
    void LoadConfigs();
}  // namespace vlrp::config

namespace rfl
{
    template <>
    struct Reflector<RE::FormID>
    {
        using ReflType = std::string;
        static ReflType from(const RE::FormID& v)
        {
            auto frm = RE::TESForm::LookupByID(v);
            return std::format("{:X}:{}", frm->GetLocalFormID(), frm->sourceFiles.array->front()->GetFilename());
        }
        static RE::FormID to(const ReflType& v)
        {
            auto loc = v.find(":");
            if (loc != std::string::npos)
            {
                auto lfid = strtoul(v.substr(0, loc).c_str(), nullptr, 16);
                auto file = v.substr(loc + 1);
                auto dh = RE::TESDataHandler::GetSingleton();
                return dh->LookupFormID(lfid, file);
            }
            else {
                return RE::TESForm::LookupByEditorID(v)->GetFormID();
            }
        }
    };
}  // namespace rfl