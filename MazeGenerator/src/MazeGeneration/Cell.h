#pragma once

#include <cstdint>

namespace maze
{
    class Cell
    {
    public:
        static constexpr uint8_t NORTH_BIT = 1 << 3;
        static constexpr uint8_t EAST_BIT  = 1 << 2;
        static constexpr uint8_t SOUTH_BIT = 1 << 1;
        static constexpr uint8_t WEST_BIT  = 1 << 0;

        uint8_t x, y;
        uint8_t state = 0x00;

    public:
        explicit Cell(uint8_t x, uint8_t y) : 
            x(x), y(y) {}
        ~Cell() = default;

        inline bool IsVisited() const
        {
            return state & 0x10;
        }

        inline void Visit()
        {
            state |= 0x10;
        }

        void OnRender(uint8_t cellSize) const;
    };
}