
// Ruan C. Keet (2022)
// Application.cpp

#include "Application.h"
#include "Log.h"
#include "Types.h"

#include "Renderer/Renderer.h"

#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <extras/raygui.h>
#include <algorithm>

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

		constexpr int gray		= 0x222222FF;
		constexpr int lightGray = 0x333333FF;
		constexpr int darkGray  = 0x111111FF;

		constexpr int blue      = 0xFEFEFEFF;
		constexpr int lightBlue = 0xFFFFFFFF;
		constexpr int darkBlue  = 0xEEEEEEFF;

		GuiSetStyle(BUTTON, BASE_COLOR_NORMAL,  gray);
		GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, lightGray);
		GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, darkGray);

		GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
		GuiSetStyle(DEFAULT, TEXT_SPACING, 2);
		GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);

		GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL,  blue);
		GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, lightBlue);
		GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, darkBlue);

		GuiSetStyle(SLIDER, BASE_COLOR_NORMAL,  gray);
		GuiSetStyle(SLIDER, BASE_COLOR_FOCUSED, lightGray);
		GuiSetStyle(SLIDER, BASE_COLOR_PRESSED, darkGray);

		GuiSetStyle(DEFAULT, BACKGROUND_COLOR,	  gray);
		GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL,   darkGray);
		GuiSetStyle(DEFAULT, BORDER_COLOR_NORMAL, lightBlue);

		GuiFade(0.7f);
	}

	bool Application::OnUpdate(const float dt)
	{
		if (IsKeyPressed(KEY_F3))
			m_Debug = !m_Debug;

		if (IsKeyPressed(KEY_H))
			m_HideGUI = !m_HideGUI;

		// Generate maze.
		if (m_Generating)
		{
			for (byte i = 0; i < m_Speed; ++i)
			{
				m_Generator.OnUpdate();
				m_Generating = !m_Generator.IsFinish();
			}

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
		if (m_HideGUI)
			return;

		const float screenWidth  = static_cast<float>(GetScreenWidth());
		const float screenHeight = static_cast<float>(GetScreenHeight());

		// Play section.
		{
			constexpr float panelWidth = 515.0f;
			constexpr float panelHeight = 90.0f;
			const Rectangle panel =
			{
				0.0f,
				screenHeight - panelHeight,
				panelWidth,
				panelHeight
			};
			GuiPanel(panel, "Play");

			constexpr float buttonWidth = 250.0f;
			constexpr float buttonHeight = 50.0f;
			if (GuiButton({ 5.0f, screenHeight - buttonHeight - 7, buttonWidth, buttonHeight }, "Start"))
				m_Generating = true;

			if (GuiButton({ buttonWidth + 10.0f, screenHeight - buttonHeight - 7, buttonWidth, buttonHeight }, "Reset"))
			{
				ResetMaze();
				m_Generating = false;
			}
		}

		// Size section.
		{
			byte cellSize = m_Generator.GetCellSize();

			constexpr float panelWidth = 155.0f;
			constexpr float panelHeight = 90.0f;
			const Rectangle panel =
			{
				515.0f,
				screenHeight - panelHeight,
				panelWidth,
				panelHeight
			};
			GuiPanel(panel, "Cell Size (px)");

			constexpr float buttonWidth  = 50.0f;
			constexpr float buttonHeight = 50.0f;
			if (GuiButton({ panel.x + 5.0f, screenHeight - buttonHeight - 7, buttonWidth, buttonHeight }, "<"))
			{
				cellSize >>= 1;

				if (cellSize < 8)
					cellSize = 8;

				m_Generator.OnResize(cellSize);
				ResetMaze();

				m_Generating = false;
			}

			if (GuiButton({ panel.width - buttonWidth + 510.0f, screenHeight - buttonHeight - 7, buttonWidth, buttonHeight }, ">"))
			{
				cellSize <<= 1;

				if (cellSize > 64)
					cellSize = 64;

				m_Generator.OnResize(cellSize);
				ResetMaze();

				m_Generating = false;
			}

			GuiDrawText(TextFormat("%d", cellSize), panel, TEXT_ALIGN_CENTER, { 255, 255, 255, 128 });
		}

		// Speed
		{
			byte cellSize = m_Generator.GetCellSize();

			constexpr float panelWidth = 155.0f;
			constexpr float panelHeight = 90.0f;
			const Rectangle panel =
			{
				670.0f,
				screenHeight - panelHeight,
				panelWidth,
				panelHeight
			};
			GuiPanel(panel, "Speed");

			constexpr float buttonWidth = 50.0f;
			constexpr float buttonHeight = 50.0f;
			if (GuiButton({ panel.x + 5.0f, screenHeight - buttonHeight - 7, buttonWidth, buttonHeight }, "<"))
			{
				m_Speed >>= 1;
			}

			if (GuiButton({ panel.width - buttonWidth + 665.0f, screenHeight - buttonHeight - 7, buttonWidth, buttonHeight }, ">"))
			{
				m_Speed <<= 1;
			}

			m_Speed = std::clamp<byte>(m_Speed, 1, 32);

			GuiDrawText(TextFormat("%dx", m_Speed), panel, TEXT_ALIGN_CENTER, { 255, 255, 255, 128 });
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