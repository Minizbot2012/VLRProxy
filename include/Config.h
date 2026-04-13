#pragma once
#include <rfl.hpp>
#include <rfl/json.hpp>
namespace MPL::Config
{
    struct RaceConfig
    {
        RE::FormID VampireRace;
        RE::FormID VLRace;
    };
}  // namespace MPL::config

namespace rfl {
    template<>
    struct Reflector<RE::FormID>
    {
        using ReflType = std::string;
        static RE::FormID to(const ReflType& v)
        {
            if (v == "null")
            {
                return 0x0;
            }
            auto loc = v.find(":");
            if (loc != std::string::npos)
            {
                auto lfid = strtoul(v.substr(0, loc).c_str(), nullptr, 16);
                auto file = v.substr(loc + 1);
                auto* dh = RE::TESDataHandler::GetSingleton();
                return dh->LookupFormID(lfid, file);;
            }
            else
            {
                auto* frm = RE::TESForm::LookupByEditorID(v);
                if (frm)
                {
                    return frm->formID;
                }
                else
                {
                    return 0x0;
                }
            }
        }
    };
}
