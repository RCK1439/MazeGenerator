/*
* Ruan C. Keet (2022)
* Renderer.h
*/

#pragma once

#include "Core/Types.h"

#include <raylib.h>
#include <string_view>

namespace maze
{
	/*
	* \brief Handles all drawing related logic for the application.
	*/
	class Renderer
	{
	public:
		/*
		* \brief Initializes the Renderer and loads all necessary files.
		*/
		static void Init();

		/*
		* \brief Unloads all used resources and shuts the Renderer down.
		*/
		static void Shutdown();

		/*
		* \brief Prepares the Renderer for drawing to the window.
		*/
		static void Begin();

		/*
		* \brief Finished drawing process.
		*/
		static void End();

		/*
		* \brief Draws the tile at the given coordinates.
		*
		* \param tileID:	The cell state of the cell.
		* \param x:			The x-coordinate of the cell.
		* \param y:			The y-coordinate of the cell.
		* \param cellSize:	The size of the cells in the field.
		*/ 
		static void DrawTile(u8 tileID, u16 x, u16 y, u16 cellSize);

		/*
		* \brief Draws the performance metrics of the 
		* application in the top-left corner.
		*/
		static void DrawPerformanceMetrics();

		/* 
		* \brief Draws text at the given position on the window.
		*
		* \param text:	The text to draw to the window.
		* \param x:		The x-coordinate to draw the text at.
		* \param y:		The y-coordinate to draw the text at.
		* \param color:	The colour to draw the text in.
		*/
		static void RenderText(std::string_view text, u16 x, u16 y, Color = WHITE);
	};
}