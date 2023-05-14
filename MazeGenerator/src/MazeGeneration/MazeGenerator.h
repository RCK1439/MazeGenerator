#pragma once

#include "Cell.h"

#include <stack>
#include <vector>
#include <memory>

namespace maze
{
	class MazeGenerator
	{
	public:
		MazeGenerator();
		MazeGenerator(byte cellSize, uint16 width, uint16 height);
		~MazeGenerator() = default;

		template<typename ... Args>
		inline static constexpr MazeGenerator Create(Args&& ... args)
		{
			return MazeGenerator(std::forward<Args>(args)...);
		}

		void OnUpdate();
		void OnRender() const;

		inline bool IsFinish() const { return m_NumVisited == (m_Width * m_Height); }
		inline float GetPercentageFinish() const { return (100.0f * m_NumVisited) / static_cast<float>(m_Width * m_Height); }

		inline void SetCellSize(byte cellSize) { m_CellSize = cellSize; }

		inline byte GetCellSize() const { return m_CellSize; }
		inline uint16 GetWidth() const { return m_Width; }
		inline uint16 GetHeight() const { return m_Height; }

		void OnResize(byte cellSize);

	private:
		void ConstructCellGrid();
		bool IsValidNeighbour(uint16 x, uint16 y);
		bool HasValidNeighbour(uint16 x, uint16 y);

	private:
		byte m_CellSize = 16;
		uint16 m_Width, m_Height;
		uint16 m_CurrentX = 0, m_CurrentY = 0;

		std::vector<std::vector<Cell>> m_Cells;
		std::stack<Cell> m_Path;

		uint32 m_NumVisited = 0;
	};
}