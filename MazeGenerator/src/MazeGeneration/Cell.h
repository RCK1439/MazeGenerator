
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
		static constexpr const byte NORTH_BIT = 0x8;
		static constexpr const byte EAST_BIT  = 0x4;
		static constexpr const byte SOUTH_BIT = 0x2;
		static constexpr const byte WEST_BIT  = 0x1;

		byte CellState = 0;

	public:
		// Constructs a new cell with its coordinates.
		//
		// @param x:	The x-coordinate of the cell.
		// @param y:	The y-coordinate of the cell.
		explicit Cell(uint16 x, uint16 y) : m_XCoord(x), m_YCoord(y) {}

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
		inline constexpr uint16 GetX() const { return m_XCoord; }

		// Gets the y-coordinate of the cell.
		//
		// @return The y-coordinate.
		inline constexpr uint16 GetY() const { return m_YCoord; }

		// Renders the cell to the window.
		//
		// @param cellSize:	The size of the cells in the maze.
		void OnRender(byte cellSize) const;

	private:
		uint16 m_XCoord;
		uint16 m_YCoord;
	};
}