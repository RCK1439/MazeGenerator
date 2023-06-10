
// Ruan C. Keet (2022)
// MazeGenerator.cpp

#include "MazeGenerator.h"

#include "Core/Log.h"

#include <raylib.h>

namespace maze
{

//	==================================================
//	Public Functions
//	==================================================

	MazeGenerator::MazeGenerator()
	{
		ConstructCellGrid();
	}

	MazeGenerator::MazeGenerator(u8 cellSize, u16 width, u16 height) :
		m_CellSize(cellSize), m_Width(width), m_Height(height)
	{
		ConstructCellGrid();
	}

	void MazeGenerator::OnUpdate()
	{
		LOG("Current: [{}, {}]", m_CurrentX, m_CurrentY);
		Cell& current = m_Cells[m_CurrentY][m_CurrentX];

		if (!current.IsVisited())
		{
			current.Visit();
			m_NumVisited++;
		}

		if (IsFinish())
			return;

		std::vector<Cell*> validNeighbours = GetValidNeighbours(m_CurrentX, m_CurrentY);
		if (validNeighbours.empty())
		{
			while (!m_Path.empty())
			{
				const Cell& cell = m_Path.top();

				const u16 sX = cell.GetX();
				const u16 sY = cell.GetY();

				if (HasValidNeighbour(sX, sY))
				{
					m_CurrentX = sX;
					m_CurrentY = sY;
					return;
				}

				m_Path.pop();
			}
		}

		const s32 upperBound = (s32)validNeighbours.size() - 1;
		const size_t index   = (size_t)GetRandomValue(0, upperBound);
		Cell& neighbour = *validNeighbours[index];

		OpenWallBetween(current, neighbour);
		m_Path.push(current);

		m_CurrentX = neighbour.GetX();
		m_CurrentY = neighbour.GetY();
	}

	void MazeGenerator::OnRender() const
	{
		for (u16 y = 0; y < m_Height; y++)
			for (u16 x = 0; x < m_Width; x++)
				m_Cells[y][x].OnRender(m_CellSize);
	}

	void MazeGenerator::OnResize(u8 cellSize)
	{
		m_CellSize = cellSize;
		ConstructCellGrid();
	}

//	==================================================
//	Private Functions
//	==================================================

	void MazeGenerator::ConstructCellGrid()
	{
		m_Width  = GetScreenWidth() / m_CellSize;
		m_Height = GetScreenHeight() / m_CellSize;

		m_Cells.reserve(m_Height);
		for (u16 y = 0; y < m_Height; y++)
		{
			m_Cells.emplace_back();
			m_Cells[y].reserve(m_Width);

			for (u16 x = 0; x < m_Width; x++)
				m_Cells[y].emplace_back(x, y);
		}
	}

	bool MazeGenerator::IsValidNeighbour(u16 x, u16 y)
	{
		if (x >= m_Width || y >= m_Height)
			return false;

		return !m_Cells[y][x].IsVisited();
	}

	bool MazeGenerator::HasValidNeighbour(u16 x, u16 y)
	{
		return  IsValidNeighbour(x, y - 1) ||
				IsValidNeighbour(x + 1, y) ||
				IsValidNeighbour(x, y + 1) ||
				IsValidNeighbour(x + 1, y);
	}

	std::vector<Cell*> MazeGenerator::GetValidNeighbours(u16 x, u16 y)
	{
		std::vector<Cell*> neighbours;

		if (IsValidNeighbour(x, y - 1)) neighbours.push_back(&m_Cells[y - 1][x]);
		if (IsValidNeighbour(x + 1, y)) neighbours.push_back(&m_Cells[y][x + 1]);
		if (IsValidNeighbour(x, y + 1)) neighbours.push_back(&m_Cells[y + 1][x]);
		if (IsValidNeighbour(x - 1, y)) neighbours.push_back(&m_Cells[y][x - 1]);

		return neighbours;
	}

	void MazeGenerator::OpenWallBetween(Cell& a, Cell& b)
	{
		const s32 ax = (s32)a.GetX();
		const s32 ay = (s32)a.GetY();
		const s32 bx = (s32)b.GetX();
		const s32 by = (s32)b.GetY();

		const s32 xDir = bx - ax;
		const s32 yDir = by - ay;

		if (xDir != 0)
		{
			a.CellState |= (xDir < 0 ? Cell::WEST_BIT : Cell::EAST_BIT);
			b.CellState |= (xDir < 0 ? Cell::EAST_BIT : Cell::WEST_BIT);
		}

		if (yDir != 0)
		{
			a.CellState |= (yDir < 0 ? Cell::NORTH_BIT : Cell::SOUTH_BIT);
			b.CellState |= (yDir < 0 ? Cell::SOUTH_BIT : Cell::NORTH_BIT);
		}
	}
}