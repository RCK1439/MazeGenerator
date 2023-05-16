
// Ruan C. Keet (2022)
// MazeGenerator.cpp

#include "MazeGenerator.h"

#include "Core/Log.h"

#include <raylib.h>

#define ALL_INVALID 0xF

namespace maze
{

//	==================================================
//	Public Functions
//	==================================================

	MazeGenerator::MazeGenerator()
	{
		ConstructCellGrid();
	}

	MazeGenerator::MazeGenerator(byte cellSize, uint16 width, uint16 height) :
		m_CellSize(cellSize), m_Width(width), m_Height(height)
	{
		ConstructCellGrid();
	}

	void MazeGenerator::OnUpdate()
	{
		LOG(TextFormat("Current: [%d, %d]", m_CurrentX, m_CurrentY));
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

				uint16 sX = cell.GetX();
				uint16 sY = cell.GetY();

				if (HasValidNeighbour(sX, sY))
				{
					m_CurrentX = sX;
					m_CurrentY = sY;
					return;
				}

				m_Path.pop();
			}
		}

		const int upperBound = static_cast<int>(validNeighbours.size()) - 1;
		const size_t index   = static_cast<size_t>(GetRandomValue(0, upperBound));
		Cell& neighbour = *validNeighbours[index];

		OpenWallBetween(current, neighbour);
		m_Path.push(current);

		m_CurrentX = neighbour.GetX();
		m_CurrentY = neighbour.GetY();
	}

	void MazeGenerator::OnRender() const
	{
		for (uint16 y = 0; y < m_Height; y++)
			for (uint16 x = 0; x < m_Width; x++)
				m_Cells[y][x].OnRender(m_CellSize);
	}

	void MazeGenerator::OnResize(byte cellSize)
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
		for (uint16 y = 0; y < m_Height; y++)
		{
			m_Cells.emplace_back();
			m_Cells[y].reserve(m_Width);

			for (uint16 x = 0; x < m_Width; x++)
				m_Cells[y].emplace_back(x, y);
		}
	}

	bool MazeGenerator::IsValidNeighbour(uint16 x, uint16 y)
	{
		if (x >= m_Width || y >= m_Height)
			return false;

		return !m_Cells[y][x].IsVisited();
	}

	bool MazeGenerator::HasValidNeighbour(uint16 x, uint16 y)
	{
		return  IsValidNeighbour(x, y - 1) ||
				IsValidNeighbour(x + 1, y) ||
				IsValidNeighbour(x, y + 1) ||
				IsValidNeighbour(x + 1, y);
	}

	std::vector<Cell*> MazeGenerator::GetValidNeighbours(uint16 x, uint16 y)
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
		const int ax = static_cast<int>(a.GetX());
		const int ay = static_cast<int>(a.GetY());
		const int bx = static_cast<int>(b.GetX());
		const int by = static_cast<int>(b.GetY());

		const int xDir = bx - ax;
		const int yDir = by - ay;

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