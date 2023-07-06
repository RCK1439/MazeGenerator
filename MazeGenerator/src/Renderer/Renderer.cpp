/*
* Ruan C. Keet (2022)
* Renderer.cpp
*/

#include "Renderer.h"

#include <array>

namespace maze
{
	static constexpr size_t NUM_TILES = 16;

	/*
	* \brief Data to be used by the renderer.
	*/
	struct RendererData
	{
		Texture2D SpriteSheet;
		u32 TileSize;
		std::array<Rectangle, NUM_TILES> Tiles;
	};

	static RendererData s_Data;

	void Renderer::Init()
	{
		s_Data.SpriteSheet = LoadTexture("res/tiles.png");

		const u32 width  = s_Data.SpriteSheet.width;
		const u32 height = s_Data.SpriteSheet.height;
		s_Data.TileSize = height;

		for (u32 i = 0; i < NUM_TILES; ++i)
		{
			s_Data.Tiles[i] =
			{
				(f32)(i * s_Data.TileSize),
				0.0f,
				(f32)(s_Data.TileSize),
				(f32)(s_Data.TileSize)
			};
		}
	}

	void Renderer::Shutdown()
	{
		UnloadTexture(s_Data.SpriteSheet);
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
		const f32 scale = (f32)cellSize / (f32)s_Data.TileSize;
		const Rectangle dest =
		{
			(f32)x,
			(f32)y,
			s_Data.TileSize * scale,
			s_Data.TileSize * scale
		};

		DrawTexturePro(s_Data.SpriteSheet, s_Data.Tiles[tileID & 0x0F], dest, { 0.0f, 0.0f }, 0.0f, WHITE);
	}

	void Renderer::DrawPerformanceMetrics()
	{
		const s32 fps = GetFPS();
		const f32 ft  = GetFrameTime() * 1000.0f;

		DrawText(TextFormat("%d FPS", fps), 5, 5, 20, WHITE);
		DrawText(TextFormat("%.2fms", ft), 5, 25, 20, WHITE);
	}

	void Renderer::RenderText(std::string_view text, u16 x, u16 y, Color color)
	{
		DrawText(text.data(), x, y, 20, color);
	}
}