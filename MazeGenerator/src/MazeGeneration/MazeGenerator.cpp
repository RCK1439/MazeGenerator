/*
* Ruan C. Keet (2022)
* MazeGenerator.cpp
*/

#pragma warning(push)
#pragma warning(disable: 28020)

#include "MazeGenerator.h"

#include "Core/Log.h"
#include "Core/Types.h"

#include <raylib.h>

namespace maze
{

	MazeGenerator::MazeGenerator()
	{
		ReconstructCellGrid();
	}

	MazeGenerator::MazeGenerator(u8 cellSize, u16 width, u16 height) :
		m_CellSize(cellSize), m_Width(width), m_Height(height)
	{
		ReconstructCellGrid();
	}

	void MazeGenerator::OnUpdate()
	{
		Cell& current = m_Cells[m_CurrentX + m_CurrentY * m_Width];
		if (!current.IsVisited())
		{
			current.Visit();
			m_NumVisited++;
		}

		if (IsFinish())
			return;

		LOG("Current: [%3hu, %3hu]\r", m_CurrentX, m_CurrentY);

		std::array<Cell*, 4> validNeighbours = GetValidNeighbours(m_CurrentX, m_CurrentY);
		if (!validNeighbours[0] && !validNeighbours[1] && !validNeighbours[2] && !validNeighbours[3])
		{
			while (!m_Path.empty())
			{
				const Cell& cell = m_Path.top();

				const u8 sX = cell.x;
				const u8 sY = cell.y;

				if (HasValidNeighbour(sX, sY))
				{
					m_CurrentX = sX;
					m_CurrentY = sY;
					return;
				}

				m_Path.pop();
			}
		}

		size_t index;
		do
		{
			index = (size_t)GetRandomValue(0, 3);
		} while (!validNeighbours[index]);

		OpenWallBetween(current, *validNeighbours[index]);
		m_Path.push(current);

		m_CurrentX = validNeighbours[index]->x;
		m_CurrentY = validNeighbours[index]->y;
	}

	void MazeGenerator::OnRender() const
	{
		for (Cell cell : m_Cells)
			cell.OnRender(m_CellSize);
	}

	void MazeGenerator::SetCellSize(u8 cellSize)
	{
		m_CellSize = cellSize;
		ReconstructCellGrid();
	}

	void MazeGenerator::ReconstructCellGrid()
	{
		const s32 screenWidth  = GetScreenWidth();
		const s32 screenHeight = GetScreenHeight();

		m_Width  = (screenWidth / m_CellSize) + (screenWidth % m_CellSize > 0);
		m_Height = (screenHeight / m_CellSize) + (screenHeight % m_CellSize > 0);

		m_Cells.reserve(m_Width * m_Height);

		for (u8 y = 0; y < m_Height; y++)
			for (u8 x = 0; x < m_Width; x++)
				m_Cells.emplace_back(x, y);
	}

	bool MazeGenerator::IsValidNeighbour(u16 x, u16 y)
	{
		if (x >= m_Width || y >= m_Height)
			return false;

		return !m_Cells[x + y * m_Width].IsVisited();
	}

	bool MazeGenerator::HasValidNeighbour(u16 x, u16 y)
	{
		return IsValidNeighbour(x, y - 1) ||
			   IsValidNeighbour(x + 1, y) ||
			   IsValidNeighbour(x, y + 1) ||
			   IsValidNeighbour(x + 1, y);
	}

	std::array<Cell*, 4> MazeGenerator::GetValidNeighbours(u16 x, u16 y)
	{
		std::array<Cell*, 4> neighbours = { nullptr, nullptr, nullptr, nullptr };

		if (IsValidNeighbour(x, y - 1)) neighbours[0] = &m_Cells[x + (y - 1) * m_Width];
		if (IsValidNeighbour(x + 1, y)) neighbours[1] = &m_Cells[(x + 1) + y * m_Width];
		if (IsValidNeighbour(x, y + 1)) neighbours[2] = &m_Cells[x + (y + 1) * m_Width];
		if (IsValidNeighbour(x - 1, y)) neighbours[3] = &m_Cells[(x - 1) + y * m_Width];

		return neighbours;
	}

	void MazeGenerator::OpenWallBetween(Cell& a, Cell& b)
	{
		const s8 xDir = b.x - a.x;
		const s8 yDir = b.y - a.y;

		if (xDir != 0)
		{
			a.CellState |= (xDir < 0 ? Cell::WEST_BIT : Cell::EAST_BIT);
			b.CellState |= (xDir < 0 ? Cell::EAST_BIT : Cell::WEST_BIT);
		}
		else if (yDir != 0)
		{
			a.CellState |= (yDir < 0 ? Cell::NORTH_BIT : Cell::SOUTH_BIT);
			b.CellState |= (yDir < 0 ? Cell::SOUTH_BIT : Cell::NORTH_BIT);
		}
	}
}

#pragma warning(pop)	// MSVC Warning C28020