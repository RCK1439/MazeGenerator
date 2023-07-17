/*
* Ruan C. Keet (2022)
* Renderer.cpp
*/

#include "Renderer.h"

#include <array>

namespace maze
{
    static constexpr size_t NUM_TILES = 16;

    static constexpr s32 FONT_SIZE = 24;

    /**
    * \brief Data to be used by the renderer.
    */
    struct RendererData
    {
        Texture2D tileSheet;
        u32 tileSize;
        std::array<Rectangle, NUM_TILES> tiles;
        Font font;
    };

    static RendererData s_Data;

    void Renderer::Init()
    {
        s_Data.tileSheet = LoadTexture("res/textures/tiles.png");

        const u32 width  = s_Data.tileSheet.width;
        const u32 height = s_Data.tileSheet.height;

        s_Data.tileSize = height;

        for (u32 i = 0; i < NUM_TILES; ++i)
        {
            s_Data.tiles[i] =
            {
                (f32)(i * s_Data.tileSize),
                0.0f,
                (f32)(s_Data.tileSize),
                (f32)(s_Data.tileSize)
            };
        }

        s_Data.font = LoadFont("res/fonts/minecraft.ttf");
    }

    void Renderer::Shutdown()
    {
        UnloadTexture(s_Data.tileSheet);
    }

    void Renderer::Begin()
    {
        BeginDrawing();
        ClearBackground(BLACK);
    }

    void Renderer::End()
    {
        EndDrawing();
    }

    void Renderer::DrawTile(u8 tileID, u16 x, u16 y, u16 cellSize)
    {
        const f32 scale = ((f32)cellSize / (f32)s_Data.tileSize);

        const Rectangle dest =
        {
            (f32)x,
            (f32)y,
            s_Data.tileSize * scale,
            s_Data.tileSize * scale
        };

        DrawTexturePro(s_Data.tileSheet, s_Data.tiles[tileID & 0x0F], dest, { 0 }, 0.0f, WHITE);
    }

    void Renderer::DrawPerformanceMetrics()
    {
        const s32 fps = GetFPS();

        const f32 ft = GetFrameTime() * 1000.0f;

        RenderText(TextFormat("%d FPS", fps), 5, 5, WHITE);
        RenderText(TextFormat("%.2fms", ft), 5, 35, WHITE);
    }

    void Renderer::RenderText(std::string_view text, u16 x, u16 y, Color color)
    {
        constexpr Color borderColor = { 0, 0, 0, 64 };

        const Vector2 dimensions = MeasureTextEx(s_Data.font, text.data(), FONT_SIZE, 2);
        const Vector2 position   = { (f32)x, (f32)y };

        const Rectangle border = { position.x, position.y, dimensions.x, dimensions.y };

        DrawRectangleRec(border, borderColor);
        DrawTextPro(s_Data.font, text.data(), { (f32)x, (f32)y }, { 0 }, 0.0f, (f32)FONT_SIZE, 2, color);
    }

    const Font& Renderer::GetFont()
    {
        return s_Data.font;
    }
}