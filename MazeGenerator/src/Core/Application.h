
// Ruan C. Keet (2022)
// Application.h

#pragma once

#include "MazeGeneration/MazeGenerator.h"

namespace maze
{
	// This class defines the application and contains
	// the main-loop.
	class Application
	{
	public:
		// Initializes the window and constructs the application.
		Application();

		// Closes the window and destroys the application.
		~Application();

		// Runs the main-loop of the program.
		void Run();

	public:
		// Initializes all the UI components of the application.
		void InitGUI();

		// Called once per frame to update the logic of the application.
		//
		// @param dt:	The time elapsed since the previous frame.
		//
		// @return false, if the program should terminate.
		[[nodiscard]] bool OnUpdate(const float dt);

		// Renders the entire program to the window.
		void OnRender();

		// Renders all the UI components to the window.
		void OnGuiRender();

		// Resets the maze generator.
		void ResetMaze();

	private:
		bool m_IsRunning = true;
		bool m_Debug = false;

		bool m_Generating = false;
		MazeGenerator m_Generator;
	};
}