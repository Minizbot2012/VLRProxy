#include <RaceManager.h>
#include <Config.h>
#include <algorithm>
namespace vlrp::managers
{
    void RaceManager::Reset() {
        this->race_pairs.clear();
    }
    void RaceManager::PushRaceData(RaceData &rd) {
        logger::info("Adding Vampire Lord Race {} for vampire race {}", rd.vlRace->GetFormEditorID(), rd.vampireRace->GetFormEditorID());
        this->race_pairs.push_back(rd);
    }
    auto RaceManager::GetVLRace(const RE::TESRace *rc) -> const RE::TESRace *
    {
        auto it = std::find_if(this->race_pairs.begin(), this->race_pairs.end(), [&](auto rd)
                               { return rd.vampireRace == rc; });
        if (it != this->race_pairs.end())
        {
            return it->vlRace;
        }
        else
        {
            return RE::TESForm::LookupByID(0x0200283A)->As<RE::TESRace>();
        }
    }
    auto RaceManager::GetHumanRace(const RE::TESRace *rc) -> const RE::TESRace *
    {
        auto rdat = std::find_if(this->race_pairs.begin(), this->race_pairs.end(), [&](auto rn)
                                 { return rn.vlRace == rc; });
        return rdat->vampireRace;
    }
    bool RaceManager::IsVampireLord(const RE::TESRace *rc)
    {
        return std::find_if(this->race_pairs.begin(), this->race_pairs.end(), [&](auto rn)
                            { return rn.vlRace == rc; }) != this->race_pairs.end() ||
               rc->formID == 0x0200283A;
    }
    void RaceManager::SetDOMRace(RE::TESRace *race)
    {
        auto dom = RE::BGSDefaultObjectManager::GetSingleton();
        dom->objects[RE::DEFAULT_OBJECT::kVampireRace] = race;
    }
    bool RaceManager::IsSupportedRace(const RE::TESRace *race)
    {
        return std::find_if(this->race_pairs.begin(), this->race_pairs.end(), [&](auto rd)
                            { return race == rd.vampireRace; }) != this->race_pairs.end();
    }
    bool RaceManager::IsSupportedVL(const RE::TESRace *race)
    {
        return std::find_if(this->race_pairs.begin(), this->race_pairs.end(), [&](auto rd)
                            { return race == rd.vlRace; }) != this->race_pairs.end();
    }
}