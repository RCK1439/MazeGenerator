#pragma once

#include <raylib.h>
#include <cstdint>
#include <string_view>

namespace maze
{
    class Renderer
    {
    public:
        static void Init();
        static void Shutdown();

        static void Begin();
        static void End();

        static void DrawTile(uint8_t tileID, uint16_t x, uint16_t y, uint16_t cellSize);
        static void DrawPerformanceMetrics();
        static void RenderText(std::string_view text, uint16_t x, uint16_t y, Color = WHITE);

        static const Font& GetFont();
    };
}