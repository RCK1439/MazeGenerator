
// Ruan C. Keet (2022)
// Renderer.h

#pragma once

#include "Core/Types.h"

namespace maze
{
	// This class handles all drawing-related functionality of our
	// application.
	class Renderer
	{
	public:
		// Initializes the Renderer and loads all necessary files.
		static void Init();

		// Unloads all used resources and shuts the Renderer down.
		static void Shutdown();

		// Prepares the Renderer for drawing to the window.
		static void Begin();

		// Finished drawing process.
		static void End();

		// Draws the tile at the given coordinates.
		//
		// @param tileID:	The cell state of the cell.
		// @param x:		The x-coordinate of the cell.
		// @param y:		The y-coordinate of the cell.
		// @param cellSize:	The size of the cells in the field.
		static void DrawTile(byte tileID, uint16 x, uint16 y, uint16 cellSize);

		// Draws the performance metrics of the application in the
		// top-left corner
		static void DrawPerformanceMetrics();
	};
}