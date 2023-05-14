#pragma once

#include "Core/Types.h"

namespace maze
{
	class Renderer
	{
	public:	
		static void Init();
		static void Shutdown();

		static void Begin();
		static void End();

		static void DrawTile(byte tileID, uint16 x, uint16 y, uint16 cellSize);

		static void DrawPerformanceMetrics();
	};
}