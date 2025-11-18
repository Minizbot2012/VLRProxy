#pragma once
namespace MPL::config
{
    struct RaceConfig
    {
        RE::FormID VampireRace;
        RE::FormID VLRace;
    };
    static std::string ConfigFolder = "./Data/SKSE/Plugins/VLRP/";
}  // namespace MPL::config

namespace rfl
{
    template <>
    struct Reflector<RE::FormID>
    {
        using ReflType = std::string;
        static ReflType from(const RE::FormID& v)
        {
            auto frm = RE::TESForm::LookupByID(v);
            return std::format("{:06X}:{}", frm->GetLocalFormID(), frm->sourceFiles.array->front()->GetFilename());
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
                auto frm = RE::TESForm::LookupByEditorID(v);
                if (frm)
                {
                    return frm->GetFormID();
                }
                else {
                    return RE::FormID(0);
                }
            }
        }
    };
}  // namespace rfl