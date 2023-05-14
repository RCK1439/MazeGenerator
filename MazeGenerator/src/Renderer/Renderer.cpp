#include "Renderer.h"

#include <raylib.h>

#define NUM_TILES 16

namespace maze
{
	struct RendererData
	{
		Texture2D Texture;
		uint32 TileSize;
		Rectangle Tiles[NUM_TILES];
	};

	static RendererData s_Data;

	void Renderer::Init()
	{
		s_Data.Texture = LoadTexture("res/tiles.png");

		const uint32 width  = s_Data.Texture.width;
		const uint32 height = s_Data.Texture.height;
		s_Data.TileSize = height;

		for (uint32 i = 0; i < NUM_TILES; ++i)
		{
			s_Data.Tiles[i] =
			{
				static_cast<float>(i * s_Data.TileSize),
				0.0f,
				static_cast<float>(s_Data.TileSize),
				static_cast<float>(s_Data.TileSize)
			};
		}
	}

	void Renderer::Shutdown()
	{
		UnloadTexture(s_Data.Texture);
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

	void Renderer::DrawTile(byte tileID, uint16 x, uint16 y, uint16 cellSize)
	{
		const float scale = static_cast<float>(cellSize) / static_cast<float>(s_Data.TileSize);
		const Rectangle dest =
		{
			static_cast<float>(x),
			static_cast<float>(y),
			s_Data.TileSize * scale,
			s_Data.TileSize * scale
		};

		DrawTexturePro(s_Data.Texture, s_Data.Tiles[tileID & 0xF], dest, { 0.0f, 0.0f }, 0.0f, WHITE);
	}

	void Renderer::DrawPerformanceMetrics()
	{
		const int fps  = GetFPS();
		const float ft = GetFrameTime() * 1000.0f;

		DrawText(TextFormat("%d FPS", fps), 5, 5, 20, WHITE);
		DrawText(TextFormat("%.2fms", ft), 5, 25, 20, WHITE);
	}
}