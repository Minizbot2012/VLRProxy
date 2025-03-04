#include <Hook.h>
#include <RaceManager.h>
#include <Config.h>
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
        };

        struct GetIsRaceAddr
        {
            static inline constexpr REL::ID addr = RELOCATION_ID(668606, 361561);
            static bool thunk([[maybe_unused]] RE::TESObjectREFR *obj, RE::TESForm *race_form, [[maybe_unused]] void *unused, double &result)
            {
                return GetIsRace::thunk(RE::PlayerCharacter::GetSingleton(), race_form, unused, result);
            }
        };

        void TryInstall()
        {
            stl::install_hook<GetIsRace>();
            logger::info("Installed hooks for GetIsRace");
            stl::install_hook<GetIsRaceAddr>();
            logger::info("Installed hook for GetIsRace Condition Lookup");
            stl::install_hook<GetPcIsRace>();
            logger::info("Installed hooks for GetPcIsRace");
            stl::install_hook<IsValidRace>();
            logger::info("Installed hooks for TESObjectARMA::IsValidRace");
        }
    }

    void TryInstall()
    {
        hook::TryInstall();
    }
}