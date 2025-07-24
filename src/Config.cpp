#include <Config.h>
#include <filesystem>
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
                if (!gl)
                {
                    for (auto itm : pairs)
                    {
                        auto vamprace =
                            RE::TESForm::LookupByEditorID<RE::TESRace>(itm.VampireRace);
                        auto vlrace = RE::TESForm::LookupByEditorID<RE::TESRace>(itm.VLRace);
                        if (vamprace && vlrace)
                        {
                            auto rd = managers::RaceData{ vamprace, vlrace };
                            mgr->PushRaceData(rd);
                        }
                    }
                }
            }
        }
    }
}  // namespace vlrp::config