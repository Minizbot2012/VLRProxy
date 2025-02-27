#pragma once
#include "glaze/glaze.hpp"
#include <filesystem>
namespace vlrp::config
{
    struct RaceConfig {
        std::string VampireRace;
        std::string VLRace;
    };
    static std::string ConfigFolder = "./Data/SKSE/Plugins/VLRP/";
    void LoadConfigs();
}
