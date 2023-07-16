/*
* Ruan C. Keet (2022)
* MazeGenerator.h
*/

#pragma once

#include "Cell.h"

#include <raylib.h>
#include <array>
#include <stack>
#include <vector>

namespace maze
{
	/**
	* \brief This class handles all the maze generation logic and functions.
	*/
	class MazeGenerator
	{
	public:
		/**
		* \brief Constructs a new MazeGenerator.
		*/
		MazeGenerator();

		/**
		* \brief Constructs a new MazeGenerator with some pre-defined parameters.
		* 
		* \param cellSize:	The size to draw the cells with.
		* \param width:		The width of the grid.
		* \param height:	The height of the grid.
		*/
		MazeGenerator(u8 cellSize, u16 width, u16 height);

		/**
		* \brief Destroys the MazeGenerator.
		*/
		~MazeGenerator() = default;

		/**
		* \brief Creates a new MazeGenerator on the stack.
		* 
		* \param args:	The arguments to construct the generator with.
		*/
		template<typename ... Args>
		inline static constexpr MazeGenerator Create(Args&& ... args)
		{
			return MazeGenerator(std::forward<Args>(args)...);
		}

		/**
		* \brief Called once per frame to update the logic of the MazeGenerator.
		* 
		* NOTE: One call from this function does one 'step' of the generation 
		* algorithm.
		*/
		void OnUpdate();

		/**
		* \brief Renders the maze to the window.
		*/
		void OnRender() const;

		/**
		* \brief Checks if the generator has finished generating the maze.
		* 
		* \return true, if the generator has finished generating.
		*/
		inline bool IsFinish() const { return m_NumVisited == (m_Width * m_Height); }

		/**
		* \brief Gets the progress of the generation as a percentage.
		* 
		* \return The percentage finish of the generation algorithm.
		*/
		inline f32 GetPercentageFinish() const { return (100.0f * m_NumVisited) / (f32)(m_Width * m_Height); }

		/**
		* \brief Gets the number of cells already visited by the algorithm
		* 
		* \return The number of visited cells.
		*/
		inline u32 GetNumVisited() const { return m_NumVisited; }

		/**
		* \brief Sets the size of the cells.
		* 
		* \param cellSize:	The size to set the cells to.
		*/
		void SetCellSize(u8 cellSize);

		/**
		* \brief Gets the size of the cells.
		* 
		* \return The size of the cells.
		*/
		inline u8 GetCellSize() const { return m_CellSize; }

		/**
		* \brief Gets the width of the grid.
		* 
		* \return The width of the grid.
		*/
		inline u16 GetWidth() const { return m_Width; }

		/**
		* \brief Gets the height of the grid.
		* 
		* \return The height of the grid.
		*/
		inline u16 GetHeight() const { return m_Height; }

		/**
		* \brief Gets the current position of the generator.
		* 
		* \return The current position.
		*/
		inline Vector2 GetCurrentPosition() const 
		{ 
			return Vector2
			{ 
				(f32)(m_CurrentX * m_CellSize) - m_CellSize / 2, 
				(f32)(m_CurrentY * m_CellSize) - m_CellSize / 2
			}; 
		}

	private:
		/**
		* \brief Constructs the arrays holding all the cells.
		*/
		void ReconstructCellGrid();

		/**
		* \brief Checks if the cell at the given coordinates is a valid neighbour.
		* 
		* A neighbour is valid when it hasn't been visited yet and the coordinates
		* are within the bounds of the board.
		* 
		* \param x:	The x-coordinate.
		* \param y: The y-coordinate.
		* 
		* \return true, if the cell at the coordinates is a valid neighbour.
		*/
		bool IsValidNeighbour(u16 x, u16 y);

		/**
		* \brief Checks if the cell at the given coordinates has atleast one valid neighbour.
		* 
		* \param x:	The x-coordinate.
		* \param y:	The y-coordinate.
		* 
		* \return true, if the cell has atleast one valid neighbour.
		*/
		bool HasValidNeighbour(u16 x, u16 y);

		/**
		* \brief Gets a list of all the valid neighbours surrounding the cell at
		* the specified coordinates.
		*/
		std::array<Cell*, 4> GetValidNeighbours(u16 x, u16 y);

		/**
		* \brief Opens the wall between the two cells. It is assumed that these cells
		* are neighbouring cells.
		* 
		* \param a:	The first cell.
		* \param b:	The second cell.
		*/
		void OpenWallBetween(Cell& a, Cell& b);

	private:
		u8 m_CellSize = 16;

		u16 m_Width;
		u16 m_Height;
		u16 m_CurrentX = 0;
		u16 m_CurrentY = 0;

		u32 m_NumVisited = 0;

		std::vector<Cell> m_Cells;

		std::stack<Cell> m_Path;
	};
}