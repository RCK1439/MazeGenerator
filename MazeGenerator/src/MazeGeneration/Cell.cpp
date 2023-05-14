#include "Cell.h"

#include "Renderer/Renderer.h"

namespace maze
{
	void Cell::OnRender(byte cellSize) const
	{
		Renderer::DrawTile(CellState, m_XCoord * cellSize, m_YCoord * cellSize, cellSize);
	}
}