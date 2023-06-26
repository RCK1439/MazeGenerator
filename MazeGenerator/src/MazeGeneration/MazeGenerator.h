
// Ruan C. Keet (2022)
// MazeGenerator.h

#pragma once

#include "Cell.h"

#include <array>
#include <stack>
#include <vector>

namespace maze
{
	class MazeGenerator
	{
	public:
		// Constructs a new MazeGenerator.
		MazeGenerator();

		// Constructs a new MazeGenerator with pre-specified values.
		//
		// @param cellSize:	The size of the cells in the maze.
		// @param width:	The width of the cell field.
		// @param height:	The height of the cell field.
		MazeGenerator(u8 cellSize, u16 width, u16 height);

		// Destroys the MazeGenerator.
		~MazeGenerator() = default;

		// Creates a new MazeGenerator on the stack.
		//
		// @param args:	Arguments passed through to the constructor of MazeGenerator.
		template<typename ... Args>
		inline static constexpr MazeGenerator Create(Args&& ... args)
		{
			return MazeGenerator(std::forward<Args>(args)...);
		}

		// Called once per frame to update the logic of the MazeGenerator.
		// In this case a single call will result in a single 'step' of the algorithm.
		void OnUpdate();

		// Renders the maze to the window.
		void OnRender() const;

		// Checks if the generator is finished generating the maze.
		//
		// @return true, if the generator has finished generating.
		inline constexpr bool IsFinish() const { return m_NumVisited == (m_Width * m_Height); }

		// Gets the ratio of visited cells to total cells in percentage form.
		//
		// @return The percentage of visited cells in total amount of cells.
		inline constexpr float GetPercentageFinish() const { return (100.0f * m_NumVisited) / (f32)(m_Width * m_Height); }

		// Gets the number of cells already
		// visited by the algorithm.
		//
		// @return The number of visited cells.
		inline constexpr u32 GetNumVisited() const { return m_NumVisited; }

		// Sets the size of the cells.
		//
		// @param cellSize:	The size to give the cells.
		inline void SetCellSize(u8 cellSize) { m_CellSize = cellSize; }

		// Gets the size of the cells.
		//
		// @return The current size of the cells.
		inline constexpr u8 GetCellSize() const { return m_CellSize; }

		// Gets the width of the field.
		//
		// @return The width of the field.
		inline constexpr u16 GetWidth() const { return m_Width; }

		// Gets the height of the field.
		//
		// @return The height of the field.
		inline constexpr u16 GetHeight() const { return m_Height; }
		
		// Logic to run when the field gets resized.
		//
		// @param cellSize:	The new size of the cells.
		void OnResize(u8 cellSize);

	private:
		// Initializes the arrays holding all the cells.
		void ConstructCellGrid();

		// Checks if the given indices points to a valid neighbour.
		//
		// @param x:	The x-index to check for.
		// @param y:	The y-index to check for.
		//
		// @return	true, if the indices aren't out of bounds and the cell
		//			pointed to by the coordinates hasn't been visited yet.
		bool IsValidNeighbour(u16 x, u16 y);

		// Checks if the cell at the given coordinates have a valid neighbour.
		//
		// @param x:	The x-index of the cell to check for.
		// @param y:	The y-index of the cell to check for.
		//
		// @return true, if the given cell has a valid neighbour.
		bool HasValidNeighbour(u16 x, u16 y);

		// Gets a list of all the valid neighbours surrounding the
		// cell at the given x- and y-indices.
		//
		// @param x:	The x-index of the cell.
		// @param y:	The y-index of the cell.
		//
		// @return A list of all valid neighbours surrounding the cell.
		std::array<Cell*, 4> GetValidNeighbours(u16 x, u16 y);

		// Opens the wall that borders the two cells based on where
		// the lie relative to eachother.
		// It is assumed that the cells are valid neighbouring cells.
		//
		// @param a:	The first cell.
		// @param b:	The second cell.
		void OpenWallBetween(Cell& a, Cell& b);

	private:
		u8 m_CellSize = 16;
		u16 m_Width, m_Height;
		u16 m_CurrentX = 0, m_CurrentY = 0;

		std::vector<std::vector<Cell>> m_Cells;
		std::stack<Cell> m_Path;

		u32 m_NumVisited = 0;
	};
}