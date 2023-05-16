
// Ruan C. Keet (2022)
// Application.cpp

#include "Application.h"
#include "Log.h"
#include "Types.h"

#include "Renderer/Renderer.h"

#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <extras/raygui.h>

namespace maze
{

//	==================================================
//	Public Functions
//	==================================================

	Application::Application()
	{
		SetConfigFlags(FLAG_VSYNC_HINT);

		SetTraceLogLevel(LOG_NONE);
		InitWindow(1536, 768, "Maze Generator by Keet");
		{
			Image icon = LoadImage("res/icon.png");
			SetWindowIcon(icon);
			UnloadImage(icon);
		}

		InitGUI();
		Renderer::Init();

		m_Generator = MazeGenerator::Create();

		LOG("Application created successfully.");
	}

	Application::~Application()
	{
		Renderer::Shutdown();

		CloseWindow();

		LOG("Application closed successfully.");
	}

	void Application::Run()
	{
		float dt = 0.0f;

		while (m_IsRunning)
		{
			m_IsRunning = OnUpdate(dt);
			OnRender();

			dt = GetFrameTime();
		}
	}
		
//	==================================================
//	Private Functions
//	==================================================

	void Application::InitGUI()
	{
		GuiSetStyle(BUTTON, BASE_COLOR_NORMAL,  0x222222FF);
		GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, 0x333333FF);
		GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, 0x111111FF);

		GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
		GuiSetStyle(DEFAULT, TEXT_SPACING, 2);

		GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL,  0xFEFEFEFF);
		GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, 0xFFFFFFFF);
		GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, 0xEEEEEEFF);

		GuiSetStyle(SLIDER, BASE_COLOR_NORMAL,  0x222222FF);
		GuiSetStyle(SLIDER, BASE_COLOR_FOCUSED, 0x333333FF);
		GuiSetStyle(SLIDER, BASE_COLOR_PRESSED, 0x111111FF);
	}

	bool Application::OnUpdate(const float dt)
	{
		if (IsKeyPressed(KEY_F3))
			m_Debug = !m_Debug;

		if (IsKeyPressed(KEY_ENTER))
			m_Generating = true;

		if (IsKeyPressed(KEY_R))
		{
			ResetMaze();
			m_Generating = false;
		}

		if (IsKeyPressed(KEY_LEFT))
		{
			byte cellSize = m_Generator.GetCellSize();
			cellSize <<= 1;

			if (cellSize > 64)
				cellSize = 64;

			m_Generator.OnResize(cellSize);
			ResetMaze();

			m_Generating = false;
		}
		else if (IsKeyPressed(KEY_RIGHT))
		{
			byte cellSize = m_Generator.GetCellSize();
			cellSize >>= 1;

			if (cellSize < 8)
				cellSize = 8;

			m_Generator.OnResize(cellSize);
			ResetMaze();

			m_Generating = false;
		}

		// Generate maze.
		if (m_Generating)
		{
			m_Generator.OnUpdate();
			m_Generating = !m_Generator.IsFinish();

			const float percentage = m_Generator.GetPercentageFinish();
			SetWindowTitle(TextFormat("Maze Generator by Keet - %.02f%%", percentage));
		}

		return !IsKeyPressed(KEY_ESCAPE);
	}

	void Application::OnRender()
	{
		Renderer::Begin();

		m_Generator.OnRender();

		OnGuiRender();
		if (m_Debug)
		{
			Renderer::DrawPerformanceMetrics();
			Renderer::RenderText(TextFormat("Maze size: %dx%d", m_Generator.GetWidth(), m_Generator.GetHeight()), 5, 45);
			Renderer::RenderText(TextFormat("Cell size: %d", m_Generator.GetCellSize()), 5, 65);

			const uint32 numVisited = m_Generator.GetNumVisited();
			const uint16 total		= m_Generator.GetWidth() * m_Generator.GetHeight();
			const float percentage  = m_Generator.GetPercentageFinish();
			Renderer::RenderText(TextFormat("Num. visited: %d/%d [%.2f%%]", numVisited, total, percentage), 5, 85);
		}

		Renderer::End();
	}

	void Application::OnGuiRender()
	{
		// TODO
	}

	void Application::ResetMaze()
	{
		const byte cellSize = m_Generator.GetCellSize();
		const uint16 width  = m_Generator.GetWidth();
		const uint16 height = m_Generator.GetHeight();

		m_Generator = MazeGenerator::Create(cellSize, width, height);
	}
}