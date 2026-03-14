#include "Config.h"
#include <RaceManager.h>
#include <SKSE/API.h>
#include <algorithm>
#include <format>
#include <string_view>
namespace MPL::Managers
{
    void Managers::RaceManager::LoadConfig()
    {
        if(this->conf_loaded) return;
        std::lock_guard _guard(this->_lock);
        if (!this->conf_loaded)
        {
            logger::info("Loading Configs");
            this->OriginalVL = RE::TESForm::LookupByEditorID<RE::TESRace>("DLC1VampireBeastRace");
            auto* dh = RE::TESDataHandler::GetSingleton();
            for (auto file : dh->files)
            {
                auto data_file = file->GetFilename();
                data_file.remove_suffix(4);
                auto file_name = std::format("VLRP/{}.json", data_file);
                RE::BSResourceNiBinaryStream fileStream(file_name);
                if (fileStream.good())
                {
                    if (fileStream.stream->totalSize > 0)
                    {
                        logger::info("Loading file {}", file_name);
                        std::string contents;
                        contents.resize(fileStream.stream->totalSize);
                        fileStream.read(contents.data(), fileStream.stream->totalSize);
                        auto gl = rfl::json::read<std::vector<MPL::Config::RaceConfig>>(contents);
                        for (auto itm : *gl)
                        {
                            if (!itm.VampireRace.IsNull() && itm.VampireRace->Is(RE::FormType::Race) && !itm.VLRace.IsNull() && itm.VLRace->Is(RE::FormType::Race))
                            {
                                auto vamprace = itm.VampireRace->As<RE::TESRace>();
                                auto vlrace = itm.VLRace->As<RE::TESRace>();
                                if (vamprace && vlrace)
                                {
                                    auto rd = Managers::RaceData{ vamprace, vlrace };
                                    this->PushRaceData(rd);
                                }
                            }
                        }
                    }
                }
            }
        }
        this->conf_loaded = true;
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
