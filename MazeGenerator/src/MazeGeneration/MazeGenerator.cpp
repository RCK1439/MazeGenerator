/*
* Ruan C. Keet (2022)
* MazeGenerator.cpp
*/

#pragma warning(push)
#pragma warning(disable: 28020)

#include "MazeGenerator.h"

#include <raylib.h>

namespace maze
{

    MazeGenerator::MazeGenerator()
    {
        ReconstructCellGrid();
    }

    MazeGenerator::MazeGenerator(uint8_t cellSize, uint16_t width, uint16_t height) :
        m_CellSize(cellSize), m_Width(width), m_Height(height)
    {
        ReconstructCellGrid();
    }

    void MazeGenerator::OnUpdate()
    {
        const size_t idx = static_cast<size_t>(m_CurrentX + m_CurrentY * m_Width);
        Cell& current = m_Cells[idx];
        if (!current.IsVisited())
        {
            current.Visit();
            m_NumVisited++;
        }

        if (IsFinish())
        {
            return;
        }

        std::array<Cell*, 4> validNeighbours = GetValidNeighbours(m_CurrentX, m_CurrentY);
        if (!validNeighbours[0] && !validNeighbours[1] && !validNeighbours[2] && !validNeighbours[3])
        {
            while (!m_Path.empty())
            {
                const Cell& cell = m_Path.top();

                const uint8_t sX = cell.x;
                const uint8_t sY = cell.y;

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
            index = static_cast<size_t>(GetRandomValue(0, 3));
        } while (!validNeighbours[index]);

        OpenWallBetween(current, *validNeighbours[index]);
        m_Path.push(current);

        m_CurrentX = validNeighbours[index]->x;
        m_CurrentY = validNeighbours[index]->y;
    }

    void MazeGenerator::OnRender() const
    {
        for (const Cell& cell : m_Cells)
        {
            cell.OnRender(m_CellSize);
        }
    }

    void MazeGenerator::SetCellSize(uint8_t cellSize)
    {
        m_CellSize = cellSize;
        ReconstructCellGrid();
    }

    void MazeGenerator::ReconstructCellGrid()
    {
        const int32_t screenWidth = GetScreenWidth();
        const int32_t screenHeight = GetScreenHeight();

        m_Width  = static_cast<uint16_t>(screenWidth / m_CellSize) + static_cast<uint16_t>(screenWidth % m_CellSize > 0);
        m_Height = static_cast<uint16_t>(screenHeight / m_CellSize) + static_cast<uint16_t>(screenHeight % m_CellSize > 0);

        const size_t size = static_cast<size_t>(m_Width) * static_cast<size_t>(m_Height);
        m_Cells.reserve(size);

        for (uint8_t y = 0; y < m_Height; y++)
        {
            for (uint8_t x = 0; x < m_Width; x++)
            {
                m_Cells.emplace_back(x, y);
            }
        }
    }

    bool MazeGenerator::IsValidNeighbour(uint16_t x, uint16_t y) const
    {
        if (x >= m_Width || y >= m_Height)
        {
            return false;
        }

        const size_t idx = static_cast<size_t>(x + y * m_Width);
        return !m_Cells[idx].IsVisited();
    }

    bool MazeGenerator::HasValidNeighbour(uint16_t x, uint16_t y) const
    {
        return IsValidNeighbour(x, y - 1) ||
               IsValidNeighbour(x + 1, y) ||
               IsValidNeighbour(x, y + 1) ||
               IsValidNeighbour(x + 1, y);
    }

    std::array<Cell*, 4> MazeGenerator::GetValidNeighbours(uint16_t x, uint16_t y)
    {
        std::array<Cell*, 4> neighbours = { nullptr, nullptr, nullptr, nullptr };

        if (IsValidNeighbour(x, y - 1)) neighbours[0] = &m_Cells[static_cast<size_t>(x + (y - 1) * m_Width)];
        if (IsValidNeighbour(x + 1, y)) neighbours[1] = &m_Cells[static_cast<size_t>((x + 1) + y * m_Width)];
        if (IsValidNeighbour(x, y + 1)) neighbours[2] = &m_Cells[static_cast<size_t>(x + (y + 1) * m_Width)];
        if (IsValidNeighbour(x - 1, y)) neighbours[3] = &m_Cells[static_cast<size_t>((x - 1) + y * m_Width)];

        return neighbours;
    }
}

#pragma warning(pop)	// MSVC Warning C28020