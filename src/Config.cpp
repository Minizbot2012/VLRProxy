#include <Config.h>
#include <RaceManager.h>
#include <memory>
namespace vlrp::config
{
    void LoadConfigs()
    {
        auto rm = managers::RaceManager::GetSingleton();
        rm->Reset();
        if (!std::filesystem::exists(ConfigFolder))
        {
            logger::error("Config directory is missing");
            return;
        }
        auto mgr = managers::RaceManager::GetSingleton();
        std::vector<RaceConfig> pairs;
        for (auto ent : std::filesystem::directory_iterator(ConfigFolder))
        {
            if (ent.path().filename().extension() == ".json")
            {
                logger::info("Loading config file {}", ent.path().filename().string());
                pairs.clear();
                auto gl = glz::read_file_json(pairs, ent.path().string(), std::string{});
                for (auto itm : pairs)
                {
                    auto vamprace = RE::TESForm::LookupByEditorID(itm.VampireRace);
                    auto vlrace = RE::TESForm::LookupByEditorID(itm.VLRace);
                    if (vamprace && vlrace && vamprace->Is(RE::FormType::Race) && vlrace->Is(RE::FormType::Race))
                    {
                        auto rd = managers::RaceData{vamprace->As<RE::TESRace>(), vlrace->As<RE::TESRace>()};
                        mgr->PushRaceData(rd);
                    }
                }
            }
        }
    }
}