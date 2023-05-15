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
		if (m_NumVisited == m_Width * m_Height)
			return;

		LOG(TextFormat("Current: [%d, %d]", m_CurrentX, m_CurrentY));

		// Mark current cell as visited
		m_Cells[m_CurrentY][m_CurrentX].Visit();

		// Randomly choose a valid neighbouring cell.
		bool validSelection = false;
		byte choice;
		byte invalids = 0;
		uint16 nX;
		uint16 nY;
		do
		{
			choice = GetRandomValue(0, 3);

			nX = m_CurrentX;
			nY = m_CurrentY;

			switch (choice)
			{
			case 0:	// West
				validSelection = IsValidNeighbour(nX - 1, nY);
				if (validSelection) nX -= 1;
				break;
			case 1:	// South
				validSelection = IsValidNeighbour(nX, nY + 1);
				if (validSelection) nY += 1;
				break;
			case 2:	// East
				validSelection = IsValidNeighbour(nX + 1, nY);
				if (validSelection) nX += 1;
				break;
			case 3:	// North
				validSelection = IsValidNeighbour(nX, nY - 1);
				if (validSelection) nY -= 1;
				break;
			}

			invalids |= (!validSelection & 0x1) << choice; 

		} while (!validSelection && invalids != ALL_INVALID);

		// Pop cells off of the stack if the algorithm gets stuck.
		if (invalids == ALL_INVALID)
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

		// Open border between current cell and neighbouring cell.
		switch (choice)
		{
		case 0:
			m_Cells[m_CurrentY][m_CurrentX].CellState |= Cell::WEST_BIT;
			m_Cells[nY][nX].CellState |= Cell::EAST_BIT;
			break;
		case 1:
			m_Cells[m_CurrentY][m_CurrentX].CellState |= Cell::SOUTH_BIT;
			m_Cells[nY][nX].CellState |= Cell::NORTH_BIT;
			break;
		case 2:
			m_Cells[m_CurrentY][m_CurrentX].CellState |= Cell::EAST_BIT;
			m_Cells[nY][nX].CellState |= Cell::WEST_BIT;
			break;
		case 3:
			m_Cells[m_CurrentY][m_CurrentX].CellState |= Cell::NORTH_BIT;
			m_Cells[nY][nX].CellState |= Cell::SOUTH_BIT;
			break;
		}

		// Push current cell to the stack.
		m_Path.push(m_Cells[m_CurrentY][m_CurrentX]);

		// Make neighbouring cell the current cell.
		m_CurrentX = nX;
		m_CurrentY = nY;

		m_NumVisited++;
	}

	void MazeGenerator::OnRender() const
	{
		for (uint16 y = 0; y < m_Height; y++)
			for (uint16 x = 0; x < m_Width; x++)
				m_Cells[y][x].OnRender(m_CellSize);
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

	void MazeGenerator::OnResize(byte cellSize)
	{
		m_CellSize = cellSize;
		ConstructCellGrid();
	}
}