#include <Hook.h>
namespace vlrp
{
    namespace hook
    {
        struct GetIsRace
        {
            static inline constexpr REL::ID relocation = RELOCATION_ID(21691, 22173);
            static inline constexpr std::size_t offset = OFFSET(0x68, 0x68);
            static bool thunk(const RE::TESObjectREFR* obj, const RE::TESForm* race_form,
                [[maybe_unused]] void* unused, double& result)
            {
                if (obj && race_form)
                {
                    const auto npc = obj->data.objectReference->As<RE::TESNPC>();
                    const auto race = race_form->As<RE::TESRace>();
                    if (npc && race)
                    {
                        auto rm = vlrp::managers::RaceManager::GetSingleton();
                        if (race == rm->GetOriginalLord())
                        {
                            if (const auto npc_race = npc->race;
                                npc_race && rm->IsVampireLord(npc_race))
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
            static void post_hook() { logger::info("Installed hooks for GetIsRace"); };
            static inline REL::Relocation<decltype(thunk)> func;
        };

        struct GetIsRaceAddr
        {
            static inline constexpr REL::ID addr = RELOCATION_ID(668606, 361561);
            static bool thunk(RE::TESObjectREFR* obj, RE::TESForm* race_form,
                [[maybe_unused]] void* unused, double& result)
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
            static bool thunk([[maybe_unused]] RE::TESObjectREFR* obj,
                RE::TESForm* race_form, [[maybe_unused]] void* unused,
                double& result)
            {
                return GetIsRace::thunk(RE::PlayerCharacter::GetSingleton(), race_form,
                    unused, result);
            }
            static void post_hook() { logger::info("Installed hooks for GetPcIsRace"); };
        };

        struct IsValidHeadpart
        {
            static inline constexpr REL::ID relocation = RELOCATION_ID(23174, 23631);
            static inline constexpr std::size_t offset = OFFSET(0x10, 0x10);
            static long thunk(const RE::BGSListForm* frm, const RE::TESForm* rc)
            {
                auto race = rc->As<RE::TESRace>();
                auto rm = vlrp::managers::RaceManager::GetSingleton();
                if (rm->IsVampireLord(race) && rm->IsSupportedLord(race) &&
                    !frm->HasForm(race))
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
            static bool thunk(const RE::TESObjectARMA* armor_addon,
                const RE::TESRace* race)
            {
                auto rm = vlrp::managers::RaceManager::GetSingleton();
                if (race != NULL && armor_addon != NULL && rm->IsVampireLord(race) &&
                    rm->IsSupportedLord(race))
                {
                    auto va_race = rm->GetVampireRace(race);
                    return func(armor_addon, va_race) ||
                           (va_race->morphRace != NULL &&
                               func(armor_addon, va_race->morphRace));
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
            static bool thunk(const RE::TESObjectARMA* armor_addon,
                const RE::TESRace* race)
            {
                auto rm = vlrp::managers::RaceManager::GetSingleton();
                if (race != NULL && armor_addon != NULL && rm->IsVampireLord(race) &&
                    rm->IsSupportedLord(race))
                {
                    auto va_race = rm->GetVampireRace(race);
                    return func(armor_addon, va_race) ||
                           (va_race->morphRace != NULL &&
                               func(armor_addon, va_race->morphRace));
                }
                else
                {
                    return func(armor_addon, race);
                }
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
            static bool thunk(const RE::TESObjectARMA* armor_addon,
                const RE::TESRace* race)
            {
                auto rm = vlrp::managers::RaceManager::GetSingleton();
                if (race != NULL && armor_addon != NULL && rm->IsVampireLord(race) &&
                    rm->IsSupportedLord(race))
                {
                    auto va_race = rm->GetVampireRace(race);
                    return func(armor_addon, va_race) ||
                           (va_race->morphRace != NULL &&
                               func(armor_addon, va_race->morphRace));
                }
                else
                {
                    return func(armor_addon, race);
                }
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
            static bool thunk(const RE::TESObjectARMA* armor_addon,
                const RE::TESRace* race)
            {
                auto rm = vlrp::managers::RaceManager::GetSingleton();
                if (race != NULL && armor_addon != NULL && rm->IsVampireLord(race) &&
                    rm->IsSupportedLord(race))
                {
                    auto va_race = rm->GetVampireRace(race);
                    return func(armor_addon, va_race) ||
                           (va_race->morphRace != NULL &&
                               func(armor_addon, va_race->morphRace));
                }
                else
                {
                    return func(armor_addon, race);
                }
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
            static bool thunk(const RE::TESObjectARMA* armor_addon,
                const RE::TESRace* race)
            {
                auto rm = vlrp::managers::RaceManager::GetSingleton();
                if (race != NULL && armor_addon != NULL && rm->IsVampireLord(race) &&
                    rm->IsSupportedLord(race))
                {
                    auto va_race = rm->GetVampireRace(race);
                    return func(armor_addon, va_race) ||
                           (va_race->morphRace != NULL &&
                               func(armor_addon, va_race->morphRace));
                }
                else
                {
                    return func(armor_addon, race);
                }
            }
            static void post_hook()
            {
                logger::info("Installed hook5 for TESObjectARMA::IsValid");
            };
            static inline REL::Relocation<decltype(thunk)> func;
        };

        struct Load3D_TESRace_IsTransformRace
        {
            static inline constexpr REL::ID relocation = RELOCATION_ID(36198, 37177);
            static inline constexpr std::size_t offset = OFFSET(0x17d, 0x183);
            static bool thunk(const RE::TESRace* me)
            {
                auto* RM = vlrp::managers::RaceManager::GetSingleton();
                if (RM->IsVampireLord(me))
                {
                    return true;
                }
                else
                {
                    return func(me);
                }
            }
            static void post_hook()
            {
                logger::info("Installed hook for TESRace::IsTransformationRace");
            };
            static inline REL::Relocation<decltype(thunk)> func;
        };

        struct FinishLoadGame_TESRace_IsTransformRace
        {
            static inline constexpr REL::ID relocation = RELOCATION_ID(36644, 37652);
            static inline constexpr std::size_t offset = OFFSET(0xde9, 0xfd6);
            static bool thunk(const RE::TESRace* me)
            {
                auto* RM = vlrp::managers::RaceManager::GetSingleton();
                if (RM->IsVampireLord(me))
                {
                    return true;
                }
                else
                {
                    return func(me);
                }
            }
            static void post_hook()
            {
                logger::info("Installed hook for TESRace::IsTransformationRace");
            };
            static inline REL::Relocation<decltype(thunk)> func;
        };

        struct SetRace_TESRace_IsTransformRace1
        {
            static inline constexpr REL::ID relocation = RELOCATION_ID(36901, 37925);
            static inline constexpr std::size_t offset = OFFSET(0xad, 0xa4);
            static bool thunk(const RE::TESRace* me)
            {
                auto* RM = vlrp::managers::RaceManager::GetSingleton();
                if (RM->IsVampireLord(me))
                {
                    return true;
                }
                else
                {
                    return func(me);
                }
            }
            static void post_hook()
            {
                logger::info("Installed hook for TESRace::SetRace");
            };
            static inline REL::Relocation<decltype(thunk)> func;
        };

        struct SetRace_TESRace_IsTransformRace2
        {
            static inline constexpr REL::ID relocation = RELOCATION_ID(36901, 37925);
            static inline constexpr std::size_t offset = OFFSET(0x10e, 0x105);
            static bool thunk(const RE::TESRace* me)
            {
                auto* RM = vlrp::managers::RaceManager::GetSingleton();
                if (RM->IsVampireLord(me))
                {
                    return true;
                }
                else
                {
                    return func(me);
                }
            }
            static void post_hook()
            {
                logger::info("Installed hook for TESRace::SetRace");
            };
            static inline REL::Relocation<decltype(thunk)> func;
        };
#ifdef SKYRIM_AE
        // In SE these are not function calls, in AE they are so use that fact, SE will continue to work through direct default object manager modifications
        // If you are reading this and know how to use Xybak, and have a way to get this to work via Xybak, PR's are open, until then these will be conditonally disabled on SE
        // However I might have an idea for something neatish that can abuse these sectors of code. However AE is gonna work though some new hook work.
        struct CharacterMenu_GetFromDOM
        {
            static inline constexpr REL::ID relocation = RELOCATION_ID(51400, 52249);
            static inline constexpr std::size_t offset = OFFSET(0x0, 0x1C1);
            static RE::TESForm* thunk(uint32_t id)
            {
                auto* RM = vlrp::managers::RaceManager::GetSingleton();
                auto* plr = RE::PlayerCharacter::GetSingleton()->GetRace();
                if (RM->IsVampireLord(plr))
                {
                    return plr;
                }
                else
                {
                    return func(id);
                }
            }
            static void post_hook()
            {
                logger::info("Installed hook for CharacterMenu::IsVampire");
            }
            static inline REL::Relocation<decltype(thunk)> func;
        };

        struct FavoritesMenu_GetFromDOM
        {
            static inline constexpr REL::ID relocation = RELOCATION_ID(51398, 52247);
            static inline constexpr std::size_t offset = OFFSET(0x0, 0x13C);
            static RE::TESForm* thunk(uint32_t id)
            {
                auto* RM = vlrp::managers::RaceManager::GetSingleton();
                auto* plr = RE::PlayerCharacter::GetSingleton()->GetRace();
                if (RM->IsVampireLord(plr))
                {
                    return plr;
                }
                else
                {
                    return func(id);
                }
            }
            static void post_hook()
            {
                logger::info("Installed hook for FavoritesMenu::IsVampire");
            }
            static inline REL::Relocation<decltype(thunk)> func;
        };

        struct IMenu_GetFromDOM
        {
            static inline constexpr REL::ID relocation = RELOCATION_ID(50638, 51532);
            static inline constexpr std::size_t offset = OFFSET(0x0, 0x13E);
            static RE::TESForm* thunk(uint32_t id)
            {
                auto* RM = vlrp::managers::RaceManager::GetSingleton();
                auto* plr = RE::PlayerCharacter::GetSingleton()->GetRace();
                if (RM->IsVampireLord(plr))
                {
                    return plr;
                }
                else
                {
                    return func(id);
                }
            }
            static void post_hook()
            {
                logger::info("Installed hook for IMenu::IsVampire");
            }
            static inline REL::Relocation<decltype(thunk)> func;
        };
#endif

        void TryInstall()
        {
            stl::install_hook<GetIsRace>();
            stl::install_hook<GetIsRaceAddr>();
            stl::install_hook<GetPcIsRace>();
            stl::install_hook<IsValidHeadpart>();
            stl::install_hook<IsValidRace_Hook1>();
            stl::install_hook<IsValidRace_Hook2>();
            stl::install_hook<IsValidRace_Hook3>();
            stl::install_hook<IsValidRace_Hook4>();
            stl::install_hook<IsValidRace_Hook5>();
            stl::install_hook<Load3D_TESRace_IsTransformRace>();
            stl::install_hook<FinishLoadGame_TESRace_IsTransformRace>();
            stl::install_hook<SetRace_TESRace_IsTransformRace1>();
            stl::install_hook<SetRace_TESRace_IsTransformRace2>();
#ifdef SKYRIM_AE
            stl::install_hook<CharacterMenu_GetFromDOM>();
            stl::install_hook<FavoritesMenu_GetFromDOM>();
            stl::install_hook<IMenu_GetFromDOM>();
#endif
        }
    }  // namespace hook

    void TryInstall() { hook::TryInstall(); }
}  // namespace vlrp