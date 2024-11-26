#pragma once

#include "Cell.h"

#include <raylib.h>
#include <array>
#include <cstdint>
#include <stack>
#include <vector>

namespace maze
{
    class MazeGenerator
    {
    public:
        MazeGenerator();
        MazeGenerator(uint8_t cellSize, uint16_t width, uint16_t height);
        ~MazeGenerator() = default;

        template<typename ... Args>
        inline static constexpr MazeGenerator Create(Args&& ... args)
        {
            return MazeGenerator(std::forward<Args>(args)...);
        }

        void OnUpdate();
        void OnRender() const;

        inline bool IsFinish() const
        {
            return m_NumVisited == static_cast<uint32_t>(m_Width * m_Height);
        }

        inline float GetPercentageFinish() const
        {
            return (100.0f * static_cast<float>(m_NumVisited)) / static_cast<float>(m_Width * m_Height);
        }

        inline uint32_t GetNumVisited() const
        {
            return m_NumVisited;
        }

        void SetCellSize(uint8_t cellSize);
        
        inline uint8_t GetCellSize() const
        {
            return m_CellSize;
        }

        inline uint16_t GetWidth() const
        {
            return m_Width;
        }

        inline uint16_t GetHeight() const
        {
            return m_Height;
        }

        inline Vector2 GetCurrentPosition() const
        {
            return Vector2
            {
                static_cast<float>(m_CurrentX * static_cast<uint16_t>(m_CellSize)) - static_cast<float>(m_CellSize / 2),
                static_cast<float>(m_CurrentY * static_cast<uint16_t>(m_CellSize)) - static_cast<float>(m_CellSize / 2)
            };
        }

    private:
        void ReconstructCellGrid();

        bool IsValidNeighbour(uint16_t x, uint16_t y) const;
        bool HasValidNeighbour(uint16_t x, uint16_t y) const;

        std::array<Cell*, 4> GetValidNeighbours(uint16_t x, uint16_t y);

        inline void OpenWallBetween(Cell& a, Cell& b)
        {
            const int8_t xDir = b.x - a.x;
            const int8_t yDir = b.y - a.y;

            const int8_t xlt = xDir < 0;
            const int8_t ylt = yDir < 0;
            const int8_t xgt = xDir > 0;
            const int8_t ygt = yDir > 0;

            a.state |= xlt * Cell::WEST_BIT + xgt * Cell::EAST_BIT;
            a.state |= ylt * Cell::NORTH_BIT + ygt * Cell::SOUTH_BIT;

            b.state |= xlt * Cell::EAST_BIT + xgt * Cell::WEST_BIT;
            b.state |= ylt * Cell::SOUTH_BIT + ygt * Cell::NORTH_BIT;
        }

    private:
        uint8_t           m_CellSize = 16;

        uint16_t          m_Width;
        uint16_t          m_Height;
        uint16_t          m_CurrentX = 0;
        uint16_t          m_CurrentY = 0;

        uint32_t          m_NumVisited = 0;

        std::vector<Cell> m_Cells;
        std::stack<Cell>  m_Path;
    };
}