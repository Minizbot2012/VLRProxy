#include <Hook.h>
#include <RaceManager.h>
#include <Config.h>
#include <ClibUtil/editorID.hpp>
namespace vlrp
{
    namespace hook
    {
        struct GetIsRace
        {
            static inline constexpr REL::ID relocation = RELOCATION_ID(21691, 22173);
            static inline constexpr std::size_t offset = OFFSET(0x68, 0x68);
            static bool thunk(const RE::TESObjectREFR *obj, RE::TESForm *race_form, [[maybe_unused]] void *unused, double &result)
            {
                if (obj && race_form)
                {
                    const auto npc = obj->data.objectReference->As<RE::TESNPC>();
                    const auto race = race_form->As<RE::TESRace>();
                    if (npc && race)
                    {
                        if (race->formID == 0x200283A)
                        {
                            if (const auto npc_race = npc->race;
                                npc_race &&
                                vlrp::managers::RaceManager::GetSingleton()->IsVampireLord(npc_race))
                            {
                                result = 1.0;
                            }
                        }
                    }
                }
                if (result > 0)
                {
                    if (RE::GetStaticTLSData()->consoleMode)
                    {
                        RE::ConsoleLog::GetSingleton()->Print("GetIsRace >> %.2lf", result);
                    }
                    return true;
                }
                else
                {
                    return func(obj, race_form, unused, result);
                }
            }
            static void post_hook()
            {
                logger::info("Installed hooks for GetIsRace");
            };
            static inline REL::Relocation<decltype(thunk)> func;
        };
        struct IsValidHeadpart
        {
            static inline constexpr REL::ID relocation = RELOCATION_ID(23174, 23631);
            static inline constexpr std::size_t offset = OFFSET(0x10, 0x10);
            static long thunk(const RE::BGSListForm *frm, const RE::TESForm *rc)
            {
                if (rc->Is(RE::FormType::Race))
                {
                    auto race = rc->As<RE::TESRace>();
                    auto rm = vlrp::managers::RaceManager::GetSingleton();
                    if (rm->IsVampireLord(race) && rm->IsSupportedVL(race) && !frm->HasForm(race))
                    {
                        auto hr = rm->GetHumanRace(race);
                        auto ret = func(frm, hr);
                        logger::info("{} {} {} VL", editorID::get_editorID(frm), editorID::get_editorID(rc), std::to_string(ret));
                        return ret;
                    }
                }
                auto ret = func(frm, rc);
                logger::info("{} {} {} HR", editorID::get_editorID(frm), editorID::get_editorID(rc), std::to_string(ret));
                return ret;
            };
            static void post_hook()
            {
                logger::info("Installed hooks for BGSHeadPart::IsValid?");
            };
            static inline REL::Relocation<decltype(thunk)> func;
        };
        struct IsValidRace
        {
            static inline constexpr REL::ID relocation = RELOCATION_ID(17392, 17792);
            static inline constexpr std::size_t offset = OFFSET(0x4d, 0x4b);
            static bool thunk(const RE::TESObjectARMA *armor_addon, const RE::TESRace *race)
            {
                if (!armor_addon || !race)
                {
                    return func(armor_addon, race);
                }
                auto rm = vlrp::managers::RaceManager::GetSingleton();
                if (rm->IsSupportedVL(race) && rm->IsVampireLord(race))
                {
                    auto va_race = rm->GetHumanRace(race);
                    return func(armor_addon, va_race);
                }
                else
                {
                    return func(armor_addon, race);
                }
            }
            static void post_hook()
            {
                logger::info("Installed hooks for TESObjectARMA::IsValid");
            };
            static inline REL::Relocation<decltype(thunk)> func;
        };
        struct GetPcIsRace
        {
            static inline constexpr REL::ID relocation = RELOCATION_ID(21697, 22179);
            static inline constexpr std::size_t offset = OFFSET(0x66, 0x66);
            static bool thunk([[maybe_unused]] RE::TESObjectREFR *obj, RE::TESForm *race_form, [[maybe_unused]] void *unused, double &result)
            {
                return GetIsRace::thunk(RE::PlayerCharacter::GetSingleton(), race_form, unused, result);
            }
            static void post_hook()
            {
                logger::info("Installed hooks for GetPcIsRace");
            };
        };

        struct GetIsRaceAddr
        {
            static inline constexpr REL::ID addr = RELOCATION_ID(668606, 361561);
            static bool thunk([[maybe_unused]] RE::TESObjectREFR *obj, RE::TESForm *race_form, [[maybe_unused]] void *unused, double &result)
            {
                return GetIsRace::thunk(RE::PlayerCharacter::GetSingleton(), race_form, unused, result);
            }
            static void post_hook()
            {
                logger::info("Installed hooks for GetIsRace (CallAddr)");
            };
        };

        void TryInstall()
        {
            stl::install_hook<GetIsRace>();
            stl::install_hook<GetIsRaceAddr>();
            stl::install_hook<GetPcIsRace>();
            stl::install_hook<IsValidRace>();
            stl::install_hook<IsValidHeadpart>();
        }
    }

    void TryInstall()
    {
        hook::TryInstall();
    }
}