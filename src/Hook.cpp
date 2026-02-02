#include <Hook.h>
#include <cstdint>
namespace MPL
{
    namespace Hook
    {
        struct GetIsRace
        {
            static inline constexpr REL::ID relocation = REL::ID(22173);
            static inline constexpr std::size_t offset = 0x68;
            static bool thunk(const RE::TESObjectREFR* obj, const RE::TESForm* race_form,
                [[maybe_unused]] void* unused, double& result)
            {
                if (obj && race_form)
                {
                    const auto npc = obj->data.objectReference->As<RE::TESNPC>();
                    const auto race = race_form->As<RE::TESRace>();
                    if (npc && race)
                    {
                        auto rm = MPL::managers::RaceManager::GetSingleton();
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
            static inline constexpr REL::ID addr = REL::ID(361561);
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
            static inline constexpr REL::ID relocation = REL::ID(22179);
            static inline constexpr std::size_t offset = 0x66;
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
            static inline constexpr REL::ID relocation = REL::ID(23631);
            static inline constexpr std::size_t offset = 0x10;
            static long thunk(const RE::BGSListForm* frm, const RE::TESForm* rc)
            {
                auto race = rc->As<RE::TESRace>();
                auto rm = MPL::managers::RaceManager::GetSingleton();
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
            static inline constexpr REL::ID relocation = REL::ID(17792);
            static inline constexpr std::size_t offset = 0x4b;
            static bool thunk(const RE::TESObjectARMA* armor_addon,
                const RE::TESRace* race)
            {
                auto rm = MPL::managers::RaceManager::GetSingleton();
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
            static inline constexpr REL::ID relocation = REL::ID(17794);
            static inline constexpr std::size_t offset = 0x17;
            static bool thunk(const RE::TESObjectARMA* armor_addon,
                const RE::TESRace* race)
            {
                auto rm = MPL::managers::RaceManager::GetSingleton();
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
            static inline constexpr REL::ID relocation = REL::ID(25362);
            static inline constexpr std::size_t offset = 0x91;
            static bool thunk(const RE::TESObjectARMA* armor_addon,
                const RE::TESRace* race)
            {
                auto rm = MPL::managers::RaceManager::GetSingleton();
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
            static inline constexpr REL::ID relocation = REL::ID(25363);
            static inline constexpr std::size_t offset = 0x81;
            static bool thunk(const RE::TESObjectARMA* armor_addon,
                const RE::TESRace* race)
            {
                auto rm = MPL::managers::RaceManager::GetSingleton();
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
            static inline constexpr REL::ID relocation = REL::ID(40114);
            static inline constexpr std::size_t offset = 0x1a3;
            static bool thunk(const RE::TESObjectARMA* armor_addon,
                const RE::TESRace* race)
            {
                auto rm = MPL::managers::RaceManager::GetSingleton();
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
            static inline constexpr REL::ID relocation = REL::ID(37177);
            static inline constexpr std::size_t offset = 0x183;
            static bool thunk(const RE::TESRace* me)
            {
                auto* RM = MPL::managers::RaceManager::GetSingleton();
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
            static inline constexpr REL::ID relocation = REL::ID(37652);
            static inline constexpr std::size_t offset = 0xfd6;
            static bool thunk(const RE::TESRace* me)
            {
                auto* RM = MPL::managers::RaceManager::GetSingleton();
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
            static inline constexpr REL::ID relocation = REL::ID(37925);
            static inline constexpr std::size_t offset = 0xa4;
            static bool thunk(const RE::TESRace* me)
            {
                auto* RM = MPL::managers::RaceManager::GetSingleton();
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
            static inline constexpr REL::ID relocation = REL::ID(37925);
            static inline constexpr std::size_t offset = 0x105;
            static bool thunk(const RE::TESRace* me)
            {
                auto* RM = MPL::managers::RaceManager::GetSingleton();
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
        struct CharacterMenu_GetFromDOM
        {
            static inline constexpr REL::ID relocation = REL::ID(52249);
            static inline constexpr std::size_t offset = 0x1C1;
            static RE::TESForm* thunk(uint32_t id)
            {
                auto* RM = MPL::managers::RaceManager::GetSingleton();
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
            static inline constexpr REL::ID relocation = REL::ID(52247);
            static inline constexpr std::size_t offset = 0x13C;
            static RE::TESForm* thunk(uint32_t id)
            {
                auto* RM = MPL::managers::RaceManager::GetSingleton();
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
            static inline constexpr REL::ID relocation = REL::ID(51532);
            static inline constexpr std::size_t offset = 0x13E;
            static RE::TESForm* thunk(uint32_t id)
            {
                auto* RM = MPL::managers::RaceManager::GetSingleton();
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

        struct Papyrus_Actor_SetRace
        {
            static inline constexpr REL::ID relocation = REL::ID(54758);
            static inline constexpr std::size_t offset = 0x23;
            static void thunk(RE::Actor* act, RE::TESRace* rc, bool)
            {
                auto* RM = MPL::managers::RaceManager::GetSingleton();
                if (RM->GetOriginalLord() == rc && act->IsPlayerRef())
                {
                    return func(act, const_cast<RE::TESRace*>(RM->GetLordRace(const_cast<const RE::TESRace*>(act->race))), act->IsPlayerRef());
                }
                else
                {
                    return func(act, rc, act->IsPlayerRef());
                }
            }
            static void post_hook()
            {
                logger::info("Installed hook for Papyrus_Actor::SetRace");
            }
            static inline REL::Relocation<decltype(thunk)> func;
        };

        struct Papyrus_Actor_UnequipAll
        {
            static inline constexpr REL::ID relocation = REL::ID(54784);
            static inline constexpr std::size_t offset = 0x50a9;
            static void thunk(int64_t a, int64_t b, RE::Actor* act)
            {
                auto* RM = MPL::managers::RaceManager::GetSingleton();
                if (act->IsPlayerRef() && RM->IsSupportedLord(act->race))
                {
                    return;
                }
                else
                {
                    return func(a, b, act);
                }
            }
            static void post_hook()
            {
                logger::info("Installed hook for Papyrus_Actor::UnequipAll");
            }
            static inline REL::Relocation<decltype(thunk)> func;
        };

        struct Papyrus_Actor_GetRace
        {
            static inline constexpr REL::ID relocation = REL::ID(54784);
            static inline constexpr std::size_t offset = 0x2245;
            static RE::TESRace* thunk([[maybe_unused]] int64_t a, [[maybe_unused]] int64_t b, RE::Actor* act)
            {
                auto* RM = MPL::managers::RaceManager::GetSingleton();
                if (RM->IsVampireLord(act->GetRace()) && act->IsPlayerRef())
                {
                    return const_cast<RE::TESRace*>(RM->GetOriginalLord());
                }
                else
                {
                    return act->race;
                }
            }
            static void post_hook()
            {
                logger::info("Installed hook for Papyrus_Actor::GetRace");
            }
            static inline REL::Relocation<decltype(thunk)> func;
        };

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
            stl::install_hook<CharacterMenu_GetFromDOM>();
            stl::install_hook<FavoritesMenu_GetFromDOM>();
            stl::install_hook<IMenu_GetFromDOM>();
            stl::install_hook<Papyrus_Actor_GetRace>();
            stl::install_hook<Papyrus_Actor_UnequipAll>();
            stl::install_hook<Papyrus_Actor_SetRace>();
        }
    }  // namespace Hook

    void TryInstall() { Hook::TryInstall(); }
}  // namespace MPL
