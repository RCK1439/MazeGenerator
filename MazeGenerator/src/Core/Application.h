#pragma once

#include "MazeGeneration/MazeGenerator.h"

namespace maze
{
	class Application
	{
	public:
		Application();
		~Application();

		void Run();

	public:
		void InitGUI();

		[[nodiscard]] bool OnUpdate(const float dt);
		void OnRender();
		void OnGuiRender();

		void ResetMaze();

	private:
		bool m_IsRunning = true;
		bool m_Debug = false;

		bool m_HideGui = false;
		bool m_Generating = false;
		bool m_Reset = false;
		bool m_Start = false;

		MazeGenerator m_Generator;
	};
}