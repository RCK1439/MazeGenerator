#include "Cell.h"

#include "Renderer/Renderer.h"

namespace maze
{
    void Cell::OnRender(uint8_t cellSize) const
    {
        Renderer::DrawTile(state, x * static_cast<uint16_t>(cellSize), y * static_cast<uint16_t>(cellSize), static_cast<uint16_t>(cellSize));
    }
}