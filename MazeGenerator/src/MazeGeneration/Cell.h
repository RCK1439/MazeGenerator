/*
* Ruan C. Keet (2022)
* Cell.h
*/

#pragma once

#include "Core/Types.h"

namespace maze
{
	/**
	* \brief This class represents a single cell in our grid.
	*/
	class Cell
	{
	public:
		static constexpr u8 NORTH_BIT = 0x8;
		static constexpr u8 EAST_BIT  = 0x4;
		static constexpr u8 SOUTH_BIT = 0x2;
		static constexpr u8 WEST_BIT  = 0x1;

		u8 x, y;
		u8 CellState = 0x00;

	public:
		/**
		* \brief Constructs a new cell with coordinates.
		* 
		* \param x:	The x-coordinate of the cell.
		* \param y:	The y-coordinate of the cell.
		*/
		explicit Cell(u8 x, u8 y) : x(x), y(y) {}

		/**
		* \brief Destroys the cell.
		*/
		~Cell() = default;

		/**
		* \brief Checks if the cell has been visited or not.
		* 
		* \return true, if the cell has been visited.
		*/
		inline bool IsVisited() const { return CellState & 0x10; }

		/**
		* \brief Marks the cell as visited.
		*/
		inline void Visit() { CellState |= 0x10; }

		/**
		* \brief Renders the cell to the window.
		* 
		* \param cellSize:	The size to draw the cell with.
		*/
		void OnRender(u8 cellSize) const;
	};
}