#pragma once
#include <filesystem>
#include <memory>
#include <glaze/glaze.hpp>
#include <RaceManager.h>
namespace vlrp::config
{
    struct RaceConfig
    {
        std::string VampireRace;
        std::string VLRace;
    };
    static std::string ConfigFolder = "./Data/SKSE/Plugins/VLRP/";
    void LoadConfigs();
}
