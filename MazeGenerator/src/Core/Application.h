/*
* Ruan C. Keet (2022)
* Application.h
*/

#pragma once

#include "MazeGeneration/MazeGenerator.h"

namespace maze
{
	/**
	* \brief Defines the application and contains the main-loop.
	*/
	class Application
	{
	public:
		/**
		* \brief Initializes the window and constructs the application.
		*/
		Application();

		/**
		* \brief Closes the window and destroys the application.
		*/
		~Application();

		/**
		* \brief Runs the main-loop of the application.
		*/
		void Run();

	public:
		/**
		* \brief Initializes all UI components of the application.
		*/
		void InitGUI();

		/**
		* \brief Called once per frame to update the logic of the application.
		* 
		* \param dt:	The time elapsed since the last frame.
		* 
		* \return true, if the application is still running.
		*/
		[[nodiscard]] bool OnUpdate(const f32 dt);

		/**
		* \brief Called once per frame to render the application to the window.
		*/
		void OnRender();

		/**
		* \brief Renders the UI components of the application to the window.
		*/
		void OnGuiRender();

		/**
		* \brief Resets the maze generator to its start-state.
		*/
		void ResetMaze();

	private:
		bool m_IsRunning = true;
		bool m_Debug = false;

		bool m_Generating = false;
		MazeGenerator m_Generator;

		u8 m_Speed = 1;
		bool m_HideGUI = false;
	};
}