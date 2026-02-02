#include <RaceManager.h>
#include <algorithm>
namespace MPL::managers
{
    void managers::RaceManager::LoadConfig()
    {
        std::lock_guard _guard(this->_lock);
        if (!this->conf_loaded)
        {
            logger::info("Loading Configs");
            this->conf_loaded = true;
            this->OriginalVL = RE::TESForm::LookupByEditorID<RE::TESRace>("DLC1VampireBeastRace");
            if (!std::filesystem::exists(MPL::config::ConfigFolder))
            {
                logger::error("Config directory is missing");
                return;
            }
            for (auto ent : std::filesystem::directory_iterator(MPL::config::ConfigFolder))
            {
                if (ent.is_regular_file() && ent.path().filename().extension() == ".json")
                {
                    logger::info("Loading config file {}", ent.path().filename().string());
                    auto gl = rfl::json::load<std::vector<MPL::config::RaceConfig>>(ent.path().string());
                    if (gl)
                    {
                        for (auto itm : *gl)
                        {
                            if (itm.VampireRace != 0x0 && itm.VLRace != 0x0)
                            {
                                auto vamprace = RE::TESForm::LookupByID<RE::TESRace>(itm.VampireRace);
                                auto vlrace = RE::TESForm::LookupByID<RE::TESRace>(itm.VLRace);
                                if (vamprace && vlrace)
                                {
                                    auto rd = managers::RaceData{ vamprace, vlrace };
                                    this->PushRaceData(rd);
                                }
                            }
                        }
                    }
                    else
                    {
                        logger::warn("Config file {} has error {}", ent.path().filename().string(), gl.error().what());
                    }
                }
            }
        }
    }
    void RaceManager::Reset()
    {
        this->race_pairs.clear();
        this->conf_loaded = false;
        this->LoadConfig();
    }

    int RaceManager::PushRaceData(RaceData& rd)
    {
        auto vm =
            std::find_if(this->race_pairs.begin(), this->race_pairs.end(),
                [&](auto rdi) { return rdi.vampireRace == rd.vampireRace; });
        auto vlm = std::find_if(this->race_pairs.begin(), this->race_pairs.end(),
            [&](auto rdi) { return rdi.vlRace == rd.vlRace; });
        if (vm != this->race_pairs.end())
        {
            logger::info("Overriding vampire lord {} for vampire {}",
                rd.vlRace->GetFormEditorID(),
                rd.vampireRace->GetFormEditorID());
            vm->vlRace = rd.vlRace;
            return 1;
        }
        else if (vlm != this->race_pairs.end())
        {
            logger::info("Overriding vampire {} for vampire lord {}",
                rd.vampireRace->GetFormEditorID(),
                rd.vlRace->GetFormEditorID());
            vm->vampireRace = rd.vampireRace;
            return 1;
        }
        else
        {
            logger::info("Adding vampire lord {} for vampire {}",
                rd.vlRace->GetFormEditorID(),
                rd.vampireRace->GetFormEditorID());
            this->race_pairs.push_back(rd);
            return 0;
        }
        return -1;
    }
    auto RaceManager::GetLordRace(const RE::TESRace* rc) -> const RE::TESRace*
    {
        this->LoadConfig();
        auto it = std::find_if(this->race_pairs.begin(), this->race_pairs.end(),
            [&](auto rd) { return rd.vampireRace == rc; });
        if (it != this->race_pairs.end())
        {
            return it->vlRace;
        }
        else
        {
            return this->OriginalVL;
        }
    }

    auto RaceManager::GetVampireRace(const RE::TESRace* rc) -> const RE::TESRace*
    {
        this->LoadConfig();
        auto it = std::find_if(this->race_pairs.begin(), this->race_pairs.end(),
            [&](auto rd) { return rd.vlRace == rc; });
        if (it != this->race_pairs.end())
        {
            return it->vampireRace;
        }
        else
        {
            logger::info("DID NOT FIND HUMAN VAMPIRE RACE");
            return nullptr;
        }
    }

    auto RaceManager::GetOriginalLord() -> const RE::TESRace*
    {
        this->LoadConfig();
        return this->OriginalVL;
    }

    bool RaceManager::IsVampireLord(const RE::TESRace* rc)
    {
        this->LoadConfig();
        return std::find_if(this->race_pairs.begin(), this->race_pairs.end(),
                   [&](auto rn) { return rn.vlRace == rc; }) !=
                   this->race_pairs.end() ||
               rc == this->OriginalVL;
    }

    bool RaceManager::IsSupportedRace(const RE::TESRace* race)
    {
        this->LoadConfig();
        return std::find_if(this->race_pairs.begin(), this->race_pairs.end(),
                   [&](auto rd) { return race == rd.vampireRace; }) !=
               this->race_pairs.end();
    }

    bool RaceManager::IsSupportedLord(const RE::TESRace* race)
    {
        this->LoadConfig();
        return std::find_if(this->race_pairs.begin(), this->race_pairs.end(),
                   [&](auto rd) { return race == rd.vlRace; }) !=
               this->race_pairs.end();
    }
}  // namespace MPL::managers
