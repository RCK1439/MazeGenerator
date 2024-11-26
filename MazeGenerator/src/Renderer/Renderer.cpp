#include "Renderer.h"

#include <external/glad.h>
#include <raymath.h>
#include <array>

namespace maze
{
    static constexpr size_t NUM_TILES = 16;
    static constexpr int32_t FONT_SIZE = 24;

    struct RendererData
    {
        Texture2D                        tileSheet;
        uint32_t                         tileSize;
        std::array<Rectangle, NUM_TILES> tiles;
        Font                             font;
    };

    static RendererData s_Data;

    void Renderer::Init()
    {
        s_Data.tileSheet = LoadTexture("res/textures/tiles.png");

        const uint32_t width = static_cast<uint32_t>(s_Data.tileSheet.width);
        const uint32_t height = static_cast<uint32_t>(s_Data.tileSheet.height);

        s_Data.tileSize = height;

        for (size_t i = 0; i < NUM_TILES; ++i)
        {
            s_Data.tiles[i] = Rectangle
            {
                static_cast<float>(i * s_Data.tileSize),
                0.0f,
                static_cast<float>(s_Data.tileSize),
                static_cast<float>(s_Data.tileSize)
            };
        }

        s_Data.font = LoadFont("res/fonts/minecraft.ttf");
    }

    void Renderer::Shutdown()
    {
        UnloadTexture(s_Data.tileSheet);
        UnloadFont(s_Data.font);
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
    
    void Renderer::DrawTile(uint8_t tileID, uint16_t x, uint16_t y, uint16_t cellSize)
    {
        const float scale = static_cast<float>(cellSize) / static_cast<float>(s_Data.tileSize);

        const Rectangle dest =
        {
            static_cast<float>(x),
            static_cast<float>(y),
            static_cast<float>(s_Data.tileSize) * scale,
            static_cast<float>(s_Data.tileSize) * scale
        };

        DrawTexturePro(s_Data.tileSheet, s_Data.tiles[tileID & 0x0F], dest, Vector2Zero(), 0.0f, WHITE);
    }

    void Renderer::DrawPerformanceMetrics()
    {
        const int32_t screenWidth = GetScreenWidth();
        const int32_t screenHeight = GetScreenHeight();
        const int32_t fps = GetFPS();

        const float ft = GetFrameTime() * 1000.0f;

        const char* gpu = reinterpret_cast<const char*>(glGetString(GL_RENDERER));

        RenderText(TextFormat("%d FPS", fps), 5, 5);
        RenderText(TextFormat("%.2fms", ft), 5, 35);
        RenderText(TextFormat("Dimensions: %hux%hu", screenWidth, screenHeight), 5, 65);
        RenderText(gpu, 5, 95);
    }

    void Renderer::RenderText(std::string_view text, uint16_t x, uint16_t y, Color color)
    {
        constexpr Color BORDER_COLOR = Color { 0, 0, 0, 64 };

        const Vector2 dimensions = MeasureTextEx(s_Data.font, text.data(), FONT_SIZE, 2);
        const Vector2 position = Vector2
        {
            static_cast<float>(x),
            static_cast<float>(y)
        };

        const Rectangle border = Rectangle
        {
            position.x,
            position.y,
            dimensions.x,
            dimensions.y
        };

        DrawRectangleRec(border, BORDER_COLOR);
        DrawTextPro(s_Data.font, text.data(), position, Vector2Zero(), 0.0f, static_cast<float>(FONT_SIZE), 2.0f, color);
    }

    const Font& Renderer::GetFont()
    {
        return s_Data.font;
    }
}