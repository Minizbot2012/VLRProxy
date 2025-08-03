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
        for (auto ent : std::filesystem::directory_iterator(ConfigFolder))
        {
            if (ent.path().filename().extension() == ".json")
            {
                logger::info("Loading config file {}", ent.path().filename().string());
                auto gl = rfl::json::load<std::vector<RaceConfig>>(ent.path().string());
                if (gl)
                {
                    for (auto itm : *gl)
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
                else {
                    logger::warn("Config file {} has error {}", ent.path().filename().string(), gl.error().what());
                }
            }
        }
    }
}  // namespace vlrp::config