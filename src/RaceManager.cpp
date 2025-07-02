#include <RaceManager.h>
namespace vlrp::managers
{
    void RaceManager::Reset()
    {
        this->OriginalVL = RE::TESForm::LookupByEditorID("DLC1VampireBeastRace")->As<RE::TESRace>();
        this->race_pairs.clear();
    }

    int RaceManager::PushRaceData(RaceData &rd)
    {
        auto vm = std::find_if(this->race_pairs.begin(), this->race_pairs.end(), [&](auto rdi)
                               { return rdi.vampireRace == rd.vampireRace; });
        auto vlm = std::find_if(this->race_pairs.begin(), this->race_pairs.end(), [&](auto rdi)
                                { return rdi.vlRace == rd.vlRace; });
        if (vm != this->race_pairs.end())
        {
            logger::info("Overriding vampire lord {} for vampire {}", rd.vlRace->GetFormEditorID(), rd.vampireRace->GetFormEditorID());
            vm->vlRace = rd.vlRace;
            return 1;
        }
        else if (vlm != this->race_pairs.end())
        {
            logger::info("Overriding vampire {} for vampire lord {}", rd.vampireRace->GetFormEditorID(), rd.vlRace->GetFormEditorID());
            vm->vampireRace = rd.vampireRace;
            return 1;
        }
        else
        {
            logger::info("Adding vampire lord {} for vampire {}", rd.vlRace->GetFormEditorID(), rd.vampireRace->GetFormEditorID());
            this->race_pairs.push_back(rd);
            return 0;
        }
        return -1;
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
            return this->OriginalVL;
        }
    }
    auto RaceManager::GetVampireRace(const RE::TESRace *rc) -> const RE::TESRace *
    {
        auto it = std::find_if(this->race_pairs.begin(), this->race_pairs.end(), [&](auto rd)
                               { return rd.vlRace == rc; });
        if (it != this->race_pairs.end())
        {
            return it->vampireRace;
        }
        else
        {
            logger::info("DID NOT FIND HUMAN VAMPIRE RACE");
            return NULL;
        }
    }
    auto RaceManager::GetOriginalVL() -> const RE::TESRace *
    {
        return this->OriginalVL;
    }
    bool RaceManager::IsVampireLord(const RE::TESRace *rc)
    {
        return std::find_if(this->race_pairs.begin(), this->race_pairs.end(), [&](auto rn)
                            { return rn.vlRace == rc; }) != this->race_pairs.end() ||
               rc == this->OriginalVL;
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