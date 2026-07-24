#include "Hooking.h"
#include <Hook.h>
#include <RaceManager.h>
#include <cstdint>
namespace MPL::Hooks
{
    struct GetIsRace
    {
        static inline constexpr REL::VariantID relocation = REL::VariantID(0, 22173, 0);
        static inline constexpr REL::VariantOffset offset = REL::VariantOffset(0x0, 0x68, 0x0);
        static bool thunk(const RE::TESObjectREFR* actor, const RE::TESForm* race_form, [[maybe_unused]] void* unused, double& result)
        {
            if (actor && race_form)
            {
                auto rm = MPL::Managers::RaceManager::GetSingleton();
                if (actor->IsPlayerRef() && race_form->Is(RE::FormType::Race) && race_form->As<RE::TESRace>() == rm->GetOriginalLord())
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

    struct TESNPC_IsValidHeadpart
    {
        static inline constexpr REL::VariantID relocation = REL::VariantID(0, 24707, 0);
        static inline constexpr REL::VariantOffset offset = REL::VariantOffset(0x0, 0x1F, 0x0);
        static bool thunk(const RE::BGSHeadPart* frm, RE::TESForm* rc)
        {
            auto* race = rc->As<RE::TESRace>();
            auto* rm = MPL::Managers::RaceManager::GetSingleton();
            if (rm->IsSupportedLord(race))
            {
                auto hr = rm->GetVampireRace(race);
                return func(frm, hr);
            }
            return func(frm, rc);
        };
        static void post_hook()
        {
            logger::info("Installed hooks for TESNPC::IsValidHeadpart");
        };
        static inline REL::Relocation<decltype(thunk)> func;
    };

    struct PlayerChar_IsValidHeadpart
    {
        static inline constexpr REL::VariantID relocation = REL::VariantID(0, 52369, 0);
        static inline constexpr REL::VariantOffset offset = REL::VariantOffset(0x0, 0xDE, 0x0);
        static bool thunk(const RE::BGSHeadPart* frm, RE::TESForm* rc)
        {
            auto* race = rc->As<RE::TESRace>();
            auto* rm = MPL::Managers::RaceManager::GetSingleton();
            if (rm->IsSupportedLord(race))
            {
                auto hr = rm->GetVampireRace(race);
                return func(frm, hr);
            }
            return func(frm, rc);
        };
        static void post_hook()
        {
            logger::info("Installed hooks for PlayerCharacter::IsValidHeadpart");
        };
        static inline REL::Relocation<decltype(thunk)> func;
    };

    struct RSM_IsValidHeadpart
    {
        static inline constexpr REL::VariantID relocation = REL::VariantID(0, 52409, 0);
        static inline constexpr REL::VariantOffset offset = REL::VariantOffset(0x0, 0x3AC, 0x0);
        static bool thunk(const RE::BGSHeadPart* frm, RE::TESForm* rc)
        {
            auto* race = rc->As<RE::TESRace>();
            auto* rm = MPL::Managers::RaceManager::GetSingleton();
            if (rm->IsSupportedLord(race))
            {
                auto hr = rm->GetVampireRace(race);
                return func(frm, hr);
            }
            return func(frm, rc);
        };
        static void post_hook()
        {
            logger::info("Installed hooks for RSM::IsValidHeadpart");
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

    struct Actor_SetRace
    {
        static inline constexpr REL::VariantID relocation = REL::VariantID(0, 54758, 0);
        static inline constexpr REL::VariantOffset offset = REL::VariantOffset(0x0, 0x23, 0x0);
        static void thunk(RE::Actor* a_this, RE::TESRace* a_race, bool a_player)
        {
            auto* RM = MPL::Managers::RaceManager::GetSingleton();
            if (RM->GetOriginalLord() == a_race)
            {
                return func(a_this, RM->GetLordRace(a_this->GetActorRuntimeData().race), a_player);
            }
            return func(a_this, a_race, a_player);
        }
        static inline REL::Relocation<decltype(thunk)> func {REL::ID(37925)};
    };

    struct Papyrus_Actor_EquipRobe
    {
        static inline constexpr REL::VariantID relocation = REL::VariantID(0, 54784, 0);
        static inline constexpr REL::VariantOffset offset = REL::VariantOffset(0x0, 0xeb3, 0x0);
        static void thunk(int64_t a1, uint32_t a2, RE::Actor* act, RE::TESForm* frm, float flt, uint8_t a5, char a6)
        {
            if (frm != nullptr && (frm == RE::TESForm::LookupByEditorID("DLC1ClothesVampireRoyalArmor") || frm == RE::TESForm::LookupByEditorID("DLC1ClothesVampireLordArmor"))) return;
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
            if (RM->IsSupportedLord(act->GetActorRuntimeData().race))
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
            if (RM->IsVampireLord(act->GetRace()))
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

    struct PlayerCharacter_Load3D
    {
        using Target = RE::PlayerCharacter;
        static inline VariantIndex index = VariantIndex(0x6A);
        static inline RE::NiAVObject* thunk(Target* actor, bool bg)
        {
            auto RM = Managers::RaceManager::GetSingleton();
            auto niNode = func(actor, bg);
            if (RM->IsSupportedLord(actor->GetRace()))
            {
                RM->AttachWings(actor);
            }
            else
            {
                RM->DetachWings(actor);
            }
            return niNode;
        }
        static void post_hook()
        {
            logger::info("Installed hook for PlayerCharacter::Load3D");
        }
        static inline REL::Relocation<decltype(thunk)> func;
    };

    struct Actor_Load3D
    {
        using Target = RE::Actor;
        static inline VariantIndex index = VariantIndex(0x6A);
        static inline RE::NiAVObject* thunk(Target* actor, bool bg)
        {
            auto RM = Managers::RaceManager::GetSingleton();
            auto niNode = func(actor, bg);
            if (RM->IsSupportedLord(actor->GetRace()))
            {
                RM->AttachWings(actor);
            }
            else
            {
                RM->DetachWings(actor);
            }
            return niNode;
        }
        static void post_hook()
        {
            logger::info("Installed hook for Actor::Load3D");
        }
        static inline REL::Relocation<decltype(thunk)> func;
    };

    void Install()
    {
        stl::install_hook<GetIsRace>();
        stl::install_hook<GetIsRaceAddr>();
        stl::install_hook<GetPcIsRace>();
        stl::install_hook<TESNPC_IsValidHeadpart>();
        stl::install_hook<PlayerChar_IsValidHeadpart>();
        stl::install_hook<RSM_IsValidHeadpart>();
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
        stl::install_hook<Actor_SetRace>();
        stl::install_hook<Actor_Load3D>();
        stl::install_hook<PlayerCharacter_Load3D>();
    }
}  // namespace MPL::Hooks
