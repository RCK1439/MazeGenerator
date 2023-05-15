
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
		GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, 0x222222FF);
		GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, 0x333333FF);
		GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, 0x111111FF);

		GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
		GuiSetStyle(DEFAULT, TEXT_SPACING, 2);

		GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, 0xFEFEFEFF);
		GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, 0xFFFFFFFF);
		GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, 0xEEEEEEFF);

		GuiSetStyle(SLIDER, BASE_COLOR_NORMAL, 0x222222FF);
		GuiSetStyle(SLIDER, BASE_COLOR_FOCUSED, 0x333333FF);
		GuiSetStyle(SLIDER, BASE_COLOR_PRESSED, 0x111111FF);
	}

	bool Application::OnUpdate(const float dt)
	{
		if (IsKeyPressed(KEY_F3))
			m_Debug = !m_Debug;

		if (IsKeyPressed(KEY_H))
			m_HideGui = !m_HideGui;

		if (m_Start)
		{
			m_Generating = true;
			m_Start = false;
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
			Renderer::DrawPerformanceMetrics();

		Renderer::End();
	}

	void Application::OnGuiRender()
	{
		if (m_HideGui)
			return;

		const float buttonWidth  = 200.0f;
		const float buttonHeight = 50.0f;

		const uint16 screenWidth  = GetScreenWidth();
		const uint16 screenHeight = GetScreenHeight();

		const byte initialCellSize = m_Generator.GetCellSize();

		byte cellSize = (byte)GuiSlider({ 5.0f, screenHeight - buttonHeight - 5.0f, buttonWidth, buttonHeight }, nullptr, "64", (float)initialCellSize, 0.0f, 64.0f);

		cellSize = (cellSize >> 4) << 4;	// Round to the nearest 4th.

		if (cellSize == 0)
			cellSize = 4;

		if (cellSize != initialCellSize)
		{
			m_Reset		 = true;
			m_Generating = false;

			m_Generator.OnResize(cellSize);
			ResetMaze();

			return;
		}

		m_Reset = GuiButton({ 5.0f, screenHeight - 2.0f * buttonHeight - 8.0f, buttonWidth, buttonHeight }, "Reset");
		m_Start = GuiButton({ 5.0f, screenHeight - 3.0f * buttonHeight - 11.0f, buttonWidth, buttonHeight }, "Start");

		if (m_Reset)
		{
			ResetMaze();
			m_Generating = false;
		}
	}

	void Application::ResetMaze()
	{
		const byte cellSize = m_Generator.GetCellSize();
		const uint16 width  = m_Generator.GetWidth();
		const uint16 height = m_Generator.GetHeight();

		m_Generator = MazeGenerator::Create(cellSize, width, height);
	}
}