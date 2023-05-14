#pragma once

#include "Core/Types.h"

namespace maze
{
	class Cell
	{
	public:
		static constexpr const byte NORTH_BIT = 0x8;
		static constexpr const byte EAST_BIT  = 0x4;
		static constexpr const byte SOUTH_BIT = 0x2;
		static constexpr const byte WEST_BIT  = 0x1;

		byte CellState = 0;

	public:
		explicit Cell(uint16 x, uint16 y) : m_XCoord(x), m_YCoord(y) {}
		~Cell() = default;

		inline bool IsVisited() const { return CellState & 0x10; }
		inline void Visit() { CellState |= 0x10; }

		inline uint16 GetX() const { return m_XCoord; }
		inline uint16 GetY() const { return m_YCoord; }

		void OnRender(byte cellSize) const;

	private:
		uint16 m_XCoord;
		uint16 m_YCoord;
	};
}