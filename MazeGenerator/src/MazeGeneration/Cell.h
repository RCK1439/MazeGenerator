
// Ruan C. Keet (2022)
// Cell.h

#pragma once

#include "Core/Types.h"

namespace maze
{
	// This class represents a single cell (tile) in our maze.
	class Cell
	{
	public:
		static constexpr u8 NORTH_BIT = 0x8;
		static constexpr u8 EAST_BIT  = 0x4;
		static constexpr u8 SOUTH_BIT = 0x2;
		static constexpr u8 WEST_BIT  = 0x1;

		u8 CellState = 0;

	public:
		// Constructs a new cell with its coordinates.
		//
		// @param x:	The x-coordinate of the cell.
		// @param y:	The y-coordinate of the cell.
		explicit Cell(u16 x, u16 y) : x(x), y(y) {}

		// Destroys the cell.
		~Cell() = default;

		// Checks if the cell has been visited or not.
		//
		// @return true, if the cell has been visited.
		inline constexpr bool IsVisited() const { return CellState & 0x10; }

		// Visits the cell.
		inline void Visit() { CellState |= 0x10; }

		// Gets the x-coordinate of the cell.
		//
		// @return The x-coordinate.
		inline constexpr u16 GetX() const { return x; }

		// Gets the y-coordinate of the cell.
		//
		// @return The y-coordinate.
		inline constexpr u16 GetY() const { return y; }

		// Renders the cell to the window.
		//
		// @param cellSize:	The size of the cells in the maze.
		void OnRender(u8 cellSize) const;

	private:
		u16 x, y;
	};
}