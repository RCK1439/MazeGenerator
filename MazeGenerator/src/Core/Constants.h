/*
* Ruan C. Keet (2022)
* Constants.h
*/

#pragma once

#include "Types.h"

namespace maze
{
    constexpr const char* MAZE_VERSION = "v1.3.1";

    constexpr u8 MAX_GENERATION_SPEED = 64;
    constexpr u8 MIN_GENERATION_SPEED = 1;

    constexpr u8 MAX_CELL_SIZE = 64;
    constexpr u8 MIN_CELL_SIZE = 8;

    constexpr u32 SCREEN_WIDTH  = 1536;
    constexpr u32 SCREEN_HEIGHT = 768;
}