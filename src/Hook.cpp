#include <Hook.h>
namespace vlrp
{
    namespace hook
    {
        struct GetIsRace
        {
            static inline constexpr REL::ID relocation = RELOCATION_ID(21691, 22173);
            static inline constexpr std::size_t offset = OFFSET(0x68, 0x68);
            static bool thunk(const RE::TESObjectREFR *obj, const RE::TESForm *race_form, [[maybe_unused]] void *unused, double &result)
            {
                if (obj && race_form)
                {
                    const auto npc = obj->data.objectReference->As<RE::TESNPC>();
                    const auto race = race_form->As<RE::TESRace>();
                    if (npc && race)
                    {
                        auto rm = vlrp::managers::RaceManager::GetSingleton();
                        if (race == rm->GetOriginalVL())
                        {
                            if (const auto npc_race = npc->race;
                                npc_race &&
                                rm->IsVampireLord(npc_race))
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
                return func(obj, race_form, unused, result);
            }
            static void post_hook()
            {
                logger::info("Installed hooks for GetIsRace");
            };
            static inline REL::Relocation<decltype(thunk)> func;
        };
        struct GetIsRaceAddr
        {
            static inline constexpr REL::ID addr = RELOCATION_ID(668606, 361561);
            static bool thunk(RE::TESObjectREFR *obj, RE::TESForm *race_form, [[maybe_unused]] void *unused, double &result)
            {
                return GetIsRace::thunk(obj, race_form, unused, result);
            }
            static void post_hook()
            {
                logger::info("Installed hooks for GetIsRace (CallAddr)");
            };
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
        struct IsValidHeadpart
        {
            static inline constexpr REL::ID relocation = RELOCATION_ID(23174, 23631);
            static inline constexpr std::size_t offset = OFFSET(0x10, 0x10);
            static long thunk(const RE::BGSListForm *frm, const RE::TESForm *rc)
            {
                auto race = rc->As<RE::TESRace>();
                auto rm = vlrp::managers::RaceManager::GetSingleton();
                if (rm->IsVampireLord(race) && rm->IsSupportedVL(race) && !frm->HasForm(race))
                {
                    auto hr = rm->GetVampireRace(race);
                    return func(frm, hr);
                }
                return func(frm, rc);
            };
            static void post_hook()
            {
                logger::info("Installed hooks for BGSHeadPart::IsValid?");
            };
            static inline REL::Relocation<decltype(thunk)> func;
        };
        struct IsValidRace_Hook1
        {
            static inline constexpr REL::ID relocation = RELOCATION_ID(17392, 17792);
            static inline constexpr std::size_t offset = OFFSET(0x4d, 0x4b);
            static bool thunk(const RE::TESObjectARMA *armor_addon, const RE::TESRace *race)
            {
                auto rm = vlrp::managers::RaceManager::GetSingleton();
                if (race != NULL && armor_addon != NULL && rm->IsVampireLord(race) && rm->IsSupportedVL(race))
                {
                    auto va_race = rm->GetVampireRace(race);
                    return func(armor_addon, va_race) || (va_race->morphRace != NULL && func(armor_addon, va_race->morphRace));
                }
                else
                {
                    return func(armor_addon, race);
                }
            }
            static void post_hook()
            {
                logger::info("Installed hook1 for TESObjectARMA::IsValid");
            };
            static inline REL::Relocation<decltype(thunk)> func;
        };
        struct IsValidRace_Hook2
        {
            static inline constexpr REL::ID relocation = RELOCATION_ID(17394, 17794);
            static inline constexpr std::size_t offset = OFFSET(0x19, 0x17);
            static bool thunk(const RE::TESObjectARMA *armor_addon, const RE::TESRace *race)
            {
                return IsValidRace_Hook1::thunk(armor_addon, race);
            }
            static void post_hook()
            {
                logger::info("Installed hook2 for TESObjectARMA::IsValid");
            };
            static inline REL::Relocation<decltype(thunk)> func;
        };
        struct IsValidRace_Hook3
        {
            static inline constexpr REL::ID relocation = RELOCATION_ID(24842, 25362);
            static inline constexpr std::size_t offset = OFFSET(0x91, 0x91);
            static bool thunk(const RE::TESObjectARMA *armor_addon, const RE::TESRace *race)
            {
                return IsValidRace_Hook1::thunk(armor_addon, race);
            }
            static void post_hook()
            {
                logger::info("Installed hook3 for TESObjectARMA::IsValid");
            };
            static inline REL::Relocation<decltype(thunk)> func;
        };
        struct IsValidRace_Hook4
        {
            static inline constexpr REL::ID relocation = RELOCATION_ID(24883, 25363);
            static inline constexpr std::size_t offset = OFFSET(0x81, 0x81);
            static bool thunk(const RE::TESObjectARMA *armor_addon, const RE::TESRace *race)
            {
                return IsValidRace_Hook1::thunk(armor_addon, race);
            }
            static void post_hook()
            {
                logger::info("Installed hook4 for TESObjectARMA::IsValid");
            };
            static inline REL::Relocation<decltype(thunk)> func;
        };
        struct IsValidRace_Hook5
        {
            static inline constexpr REL::ID relocation = RELOCATION_ID(39044, 40114);
            static inline constexpr std::size_t offset = OFFSET(0x193, 0x1a3);
            static bool thunk(const RE::TESObjectARMA *armor_addon, const RE::TESRace *race)
            {
                return IsValidRace_Hook1::thunk(armor_addon, race);
            }
            static void post_hook()
            {
                logger::info("Installed hook5 for TESObjectARMA::IsValid");
            };
            static inline REL::Relocation<decltype(thunk)> func;
        };

        void TryInstall()
        {
            stl::install_hook<GetIsRace>();
            stl::install_hook<GetIsRaceAddr>();
            stl::install_hook<GetPcIsRace>();
            stl::install_hook<IsValidRace_Hook1>();
            stl::install_hook<IsValidRace_Hook2>();
            stl::install_hook<IsValidRace_Hook3>();
            stl::install_hook<IsValidRace_Hook4>();
            stl::install_hook<IsValidRace_Hook5>();
            stl::install_hook<IsValidHeadpart>();
        }
    }

    void TryInstall()
    {
        hook::TryInstall();
    }
}