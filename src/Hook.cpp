#include <Hook.h>
#include <RaceManager.h>
#include <cstdint>
namespace MPL
{
    namespace Hook
    {
        struct GetIsRace
        {
            static inline constexpr REL::VariantID relocation = REL::VariantID(0, 22173, 0);
            static inline constexpr REL::VariantOffset offset = REL::VariantOffset(0x0, 0x68, 0x0);
            static bool thunk(const RE::TESObjectREFR* actor, const RE::TESForm* race_form,
                [[maybe_unused]] void* unused, double& result)
            {
                if (actor && race_form->Is(RE::FormType::Race))
                {
                    auto rm = MPL::Managers::RaceManager::GetSingleton();
                    if (actor != nullptr && actor->IsPlayerRef() && race_form->Is(RE::FormType::Race) && race_form->As<RE::TESRace>() == rm->GetOriginalLord())
                    {
                        const auto npc = actor->As<RE::Actor>();
                        const auto race = race_form->As<RE::TESRace>();
                        if (race && npc && npc->GetActorRuntimeData().race != nullptr)
                        {
                            if (auto npc_race = npc->GetActorRuntimeData().race;
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
                return func(actor, race_form, unused, result);
            }
            static void post_hook() { logger::info("Installed hooks for GetIsRace"); };
            static inline REL::Relocation<decltype(thunk)> func;
        };

        struct GetIsRaceAddr
        {
            static inline constexpr REL::VariantID addr = REL::VariantID(0, 361561, 0);
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
            static inline constexpr REL::VariantID relocation = REL::VariantID(0, 22179, 0);
            static inline constexpr REL::VariantOffset offset = REL::VariantOffset(0x0, 0x66, 0x0);
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
            static inline constexpr REL::VariantID relocation = REL::VariantID(0, 23631, 0);
            static inline constexpr REL::VariantOffset offset = REL::VariantOffset(0x0, 0x10, 0x0);
            static long thunk(const RE::BGSListForm* frm, RE::TESForm* rc)
            {
                auto* race = rc->As<RE::TESRace>();
                auto* rm = MPL::Managers::RaceManager::GetSingleton();
                if (rm->IsSupportedLord(race) && !frm->HasForm(race))
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
            static inline constexpr REL::VariantID relocation = REL::VariantID(0, 17792, 0);
            static inline constexpr REL::VariantOffset offset = REL::VariantOffset(0x0, 0x4B, 0x0);
            static bool thunk(const RE::TESObjectARMA* armor_addon,
                RE::TESRace* race)
            {
                auto* rm = MPL::Managers::RaceManager::GetSingleton();
                if (race != nullptr && armor_addon != nullptr && rm->IsSupportedLord(race))
                {
                    auto* va_race = rm->GetVampireRace(race);
                    return func(armor_addon, va_race);
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
            static inline constexpr REL::VariantID relocation = REL::VariantID(0, 17794, 0);
            static inline constexpr REL::VariantOffset offset = REL::VariantOffset(0x0, 0x17, 0x0);
            static bool thunk(const RE::TESObjectARMA* armor_addon,
                RE::TESRace* race)
            {
                auto* rm = MPL::Managers::RaceManager::GetSingleton();
                if (race != nullptr && armor_addon != nullptr && rm->IsSupportedLord(race))
                {
                    auto* va_race = rm->GetVampireRace(race);
                    return func(armor_addon, va_race);
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
            static inline constexpr REL::VariantID relocation = REL::VariantID(0, 25362, 0);
            static inline constexpr REL::VariantOffset offset = REL::VariantOffset(0x0, 0x91, 0x0);
            static bool thunk(const RE::TESObjectARMA* armor_addon,
                RE::TESRace* race)
            {
                auto* rm = MPL::Managers::RaceManager::GetSingleton();
                if (race != nullptr && armor_addon != nullptr && rm->IsSupportedLord(race))
                {
                    auto* va_race = rm->GetVampireRace(race);
                    return func(armor_addon, va_race);
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
            static inline constexpr REL::VariantID relocation = REL::VariantID(0, 25362, 0);
            static inline constexpr REL::VariantOffset offset = REL::VariantOffset(0x0, 0x131, 0x0);
            static bool thunk(const RE::TESObjectARMA* armor_addon,
                RE::TESRace* race)
            {
                auto* rm = MPL::Managers::RaceManager::GetSingleton();
                if (race != nullptr && armor_addon != nullptr && rm->IsSupportedLord(race))
                {
                    auto* va_race = rm->GetVampireRace(race);
                    return func(armor_addon, va_race);
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
            static inline constexpr REL::VariantID relocation = REL::VariantID(0, 25363, 0);
            static inline constexpr REL::VariantOffset offset = REL::VariantOffset(0x0, 0x81, 0x0);
            static bool thunk(const RE::TESObjectARMA* armor_addon,
                RE::TESRace* race)
            {
                auto* rm = MPL::Managers::RaceManager::GetSingleton();
                if (race != nullptr && armor_addon != nullptr && rm->IsSupportedLord(race))
                {
                    auto* va_race = rm->GetVampireRace(race);
                    return func(armor_addon, va_race);
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

        struct IsValidRace_Hook6
        {
            static inline constexpr REL::VariantID relocation = REL::VariantID(0, 40114, 0);
            static inline constexpr REL::VariantOffset offset = REL::VariantOffset(0x0, 0x1a3, 0x0);
            static bool thunk(const RE::TESObjectARMA* armor_addon,
                RE::TESRace* race)
            {
                auto* rm = MPL::Managers::RaceManager::GetSingleton();
                if (race != nullptr && armor_addon != nullptr && rm->IsSupportedLord(race))
                {
                    auto* va_race = rm->GetVampireRace(race);
                    return func(armor_addon, va_race);
                }
                else
                {
                    return func(armor_addon, race);
                }
            }
            static void post_hook()
            {
                logger::info("Installed hook6 for TESObjectARMA::IsValid");
            };
            static inline REL::Relocation<decltype(thunk)> func;
        };

        struct Load3D_TESRace_IsTransformRace
        {
            static inline constexpr REL::VariantID relocation = REL::VariantID(0, 37177, 0);
            static inline constexpr REL::VariantOffset offset = REL::VariantOffset(0x0, 0x183, 0x0);
            static bool thunk(RE::TESRace* me)
            {
                auto* RM = MPL::Managers::RaceManager::GetSingleton();
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
            static inline constexpr REL::VariantID relocation = REL::VariantID(0, 37652, 0);
            static inline constexpr REL::VariantOffset offset = REL::VariantOffset(0x0, 0xfd6, 0x0);
            static bool thunk(RE::TESRace* me)
            {
                auto* RM = MPL::Managers::RaceManager::GetSingleton();
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
            static inline constexpr REL::VariantID relocation = REL::VariantID(0, 37925, 0);
            static inline constexpr REL::VariantOffset offset = REL::VariantOffset(0x0, 0xa4, 0x0);
            static bool thunk(RE::TESRace* me)
            {
                auto* RM = MPL::Managers::RaceManager::GetSingleton();
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
            static inline constexpr REL::VariantID relocation = REL::VariantID(0, 37925, 0);
            static inline constexpr REL::VariantOffset offset = REL::VariantOffset(0x0, 0x105, 0x0);
            static bool thunk(RE::TESRace* me)
            {
                auto* RM = MPL::Managers::RaceManager::GetSingleton();
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
            static inline constexpr REL::VariantID relocation = REL::VariantID(0, 52249, 0);
            static inline constexpr REL::VariantOffset offset = REL::VariantOffset(0x0, 0x1C1, 0x0);
            static RE::TESForm* thunk(uint32_t id)
            {
                auto* RM = MPL::Managers::RaceManager::GetSingleton();
                auto* plr = RE::PlayerCharacter::GetSingleton()->GetRace();
                if (RM->IsVampireLord(plr) && id == RE::DEFAULT_OBJECTS::kVampireRace)
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
            static inline constexpr REL::VariantID relocation = REL::VariantID(0, 52247, 0);
            static inline constexpr REL::VariantOffset offset = REL::VariantOffset(0x0, 0x13C, 0x0);
            static RE::TESForm* thunk(uint32_t id)
            {
                auto* RM = MPL::Managers::RaceManager::GetSingleton();
                auto* plr = RE::PlayerCharacter::GetSingleton()->GetRace();
                if (RM->IsVampireLord(plr) && id == RE::DEFAULT_OBJECTS::kVampireRace)
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
            static inline constexpr REL::VariantID relocation = REL::VariantID(0, 51532, 0);
            static inline constexpr REL::VariantOffset offset = REL::VariantOffset(0x0, 0x13E, 0x0);
            static RE::TESForm* thunk(uint32_t id)
            {
                auto* RM = MPL::Managers::RaceManager::GetSingleton();
                auto* plr = RE::PlayerCharacter::GetSingleton()->GetRace();
                if (RM->IsVampireLord(plr) && id == RE::DEFAULT_OBJECTS::kVampireRace)
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
            static inline constexpr REL::VariantID relocation = REL::VariantID(0, 54758, 0);
            static inline constexpr REL::VariantOffset offset = REL::VariantOffset(0x0, 0x23, 0x0);
            static void thunk(RE::Actor* act, RE::TESRace* rc, bool)
            {
                auto* RM = MPL::Managers::RaceManager::GetSingleton();
                if (RM->GetOriginalLord() == rc && act->IsPlayerRef())
                {
                    return func(act, RM->GetLordRace(act->GetActorRuntimeData().race), act->IsPlayerRef());
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

        struct Papyrus_Actor_EquipRobe
        {
            static inline constexpr REL::VariantID relocation = REL::VariantID(0, 54784, 0);
            static inline constexpr REL::VariantOffset offset = REL::VariantOffset(0x0, 0xeb3, 0x0);
            static void thunk(int64_t a1, uint32_t a2, RE::Actor* act, RE::TESForm* frm, float flt, uint8_t a5, char a6)
            {
                if (act->IsPlayerRef() && frm != nullptr && (frm == RE::TESForm::LookupByEditorID("DLC1ClothesVampireRoyalArmor") || frm == RE::TESForm::LookupByEditorID("DLC1ClothesVampireLordArmor"))) return;
                func(a1, a2, act, frm, flt, a5, a6);
            }
            static void post_hook()
            {
                logger::info("Installed hook for Papyrus_Actor::EquipItem");
            }
            static inline REL::Relocation<decltype(thunk)> func;
        };

        struct Papyrus_Actor_UnequipAll
        {
            static inline constexpr REL::VariantID relocation = REL::VariantID(0, 54784, 0);
            static inline constexpr REL::VariantOffset offset = REL::VariantOffset(0x0, 0x50a9, 0x0);
            static void thunk(int64_t a, int64_t b, RE::Actor* act)
            {
                auto* RM = MPL::Managers::RaceManager::GetSingleton();
                if (act->IsPlayerRef() && RM->IsSupportedLord(act->GetActorRuntimeData().race))
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
            static inline constexpr REL::VariantID relocation = REL::VariantID(0, 54784, 0);
            static inline constexpr REL::VariantOffset offset = REL::VariantOffset(0x0, 0x2245, 0x0);
            static RE::TESRace* thunk([[maybe_unused]] int64_t a, [[maybe_unused]] int64_t b, RE::Actor* act)
            {
                auto* RM = MPL::Managers::RaceManager::GetSingleton();
                if (RM->IsVampireLord(act->GetRace()) && act->IsPlayerRef())
                {
                    return const_cast<RE::TESRace*>(RM->GetOriginalLord());
                }
                else
                {
                    return act->GetActorRuntimeData().race;
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
            stl::install_hook<IsValidRace_Hook6>();
            stl::install_hook<Load3D_TESRace_IsTransformRace>();
            stl::install_hook<FinishLoadGame_TESRace_IsTransformRace>();
            stl::install_hook<SetRace_TESRace_IsTransformRace1>();
            stl::install_hook<SetRace_TESRace_IsTransformRace2>();
            stl::install_hook<CharacterMenu_GetFromDOM>();
            stl::install_hook<FavoritesMenu_GetFromDOM>();
            stl::install_hook<IMenu_GetFromDOM>();
            stl::install_hook<Papyrus_Actor_GetRace>();
            stl::install_hook<Papyrus_Actor_UnequipAll>();
            stl::install_hook<Papyrus_Actor_EquipRobe>();
            stl::install_hook<Papyrus_Actor_SetRace>();
        }
    }  // namespace Hook

    void TryInstall() { Hook::TryInstall(); }
}  // namespace MPL
