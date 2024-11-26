#pragma once

#include <cstdint>
#include <string_view>

namespace maze
{
    constexpr std::string_view MAZE_VERSION = "v1.3.1";

    constexpr uint8_t MAX_GENERATION_SPEED = 64;
    constexpr uint8_t MIN_GENERATION_SPEED = 1;

    constexpr uint8_t MAX_CELL_SIZE = 64;
    constexpr uint8_t MIN_CELL_SIZE = 8;

    constexpr uint32_t SCREEN_WIDTH  = 1536;
    constexpr uint32_t SCREEN_HEIGHT = 768;
}