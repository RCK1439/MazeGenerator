
// Ruan C. Keet (2022)
// Cell.cpp

#include "Cell.h"

#include "Renderer/Renderer.h"

namespace maze
{
	void Cell::OnRender(u8 cellSize) const
	{
		Renderer::DrawTile(CellState, x * cellSize, y * cellSize, cellSize);
	}
}