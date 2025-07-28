#pragma once
#include <RaceManager.h>
#include <glaze/glaze.hpp>
namespace vlrp::config
{
    struct RaceConfig
    {
        std::string VampireRace;
        std::string VLRace;
    };
    static std::string ConfigFolder = "./Data/SKSE/Plugins/VLRP/";
    void LoadConfigs();
}  // namespace vlrp::config