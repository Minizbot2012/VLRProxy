#pragma once
#include <RaceManager.h>

namespace MPL::Hooks
{
#pragma pack(push, 1)
    struct Assembly
    {
        uint8_t opCode{ 0xFF };
        uint8_t dism{ 0x25 };
        uint32_t disp{ 0x0 };
        uint64_t loc;
    };
    struct RelJump
    {
        uint8_t opCode{ 0xE9 };
        int32_t relLoc;
    };
#pragma pack(pop)
    void Install();
    void Install_late();
}  // namespace MPL::Hooks
