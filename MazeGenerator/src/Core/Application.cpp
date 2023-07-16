/*
* Ruan C. Keet (2022)
* Application.cpp
*/

#include "Application.h"
#include "Constants.h"
#include "Log.h"
#include "Types.h"

#include "Renderer/Renderer.h"

#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <extras/raygui.h>
#include <algorithm>

namespace maze
{

	Application::Application() :
		m_Camera({ SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f },
				 { SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f })
	{
		SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);

		SetTraceLogLevel(LOG_NONE);
		InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Maze Generator");

		SetWindowMinSize(1280, 640);

		Image icon = LoadImage("res/icon.png");
		SetWindowIcon(icon);
		UnloadImage(icon);

		Renderer::Init();
		InitGUI();

		m_Generator = MazeGenerator::Create();
		LOG("Application created successfully.\n");
	}

	Application::~Application()
	{
		Renderer::Shutdown();

		CloseWindow();
		LOG("Application closed successfully.\n");
	}

	void Application::Run()
	{
		f32 dt = 0.0f;

		while (m_IsRunning)
		{
			m_IsRunning = OnUpdate(dt);
			OnRender();

			dt = GetFrameTime();
		}
	}

	void Application::InitGUI()
	{
		constexpr s32 gray		= 0x222222FF;
		constexpr s32 lightGray = 0x333333FF;
		constexpr s32 darkGray  = 0x111111FF;
		constexpr s32 blue      = 0xFEFEFEFF;
		constexpr s32 lightBlue = 0xFFFFFFFF;
		constexpr s32 darkBlue  = 0xEEEEEEFF;

		GuiSetFont(Renderer::GetFont());

		GuiSetStyle(BUTTON, BASE_COLOR_NORMAL,  gray);
		GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, lightGray);
		GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, darkGray);

		GuiSetStyle(DEFAULT, TEXT_SIZE, 16);
		GuiSetStyle(DEFAULT, TEXT_SPACING, 1);
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

	bool Application::OnUpdate(const f32 dt)
	{
		if (IsKeyPressed(KEY_F3))
			m_Debug = !m_Debug;

		if (IsKeyPressed(KEY_H))
			m_HideGUI = !m_HideGUI;

		if (IsKeyPressed(KEY_F))
		{
			s32 monitor = GetCurrentMonitor();
			s32 width   = GetMonitorWidth(monitor);
			s32 height  = GetMonitorHeight(monitor);

			SetWindowSize(width, height);
			ToggleFullscreen();
		}

		if (IsWindowResized())
		{
			ResetMaze();

			Vector2 newDimensions =
			{
				(f32)m_Generator.GetWidth() * m_Generator.GetCellSize(),
				(f32)m_Generator.GetHeight() * m_Generator.GetCellSize()
			};
			m_Camera.OnResize(newDimensions);
		}

		if (m_Generating)
		{
			for (u8 i = 0; i < m_Speed; i++)
			{
				m_Generator.OnUpdate();
				m_Generating = !m_Generator.IsFinish();
			}
		}

		m_Camera.OnUpdate(dt, m_Generator.GetCurrentPosition(), m_Follow);

		return !WindowShouldClose();
	}

	void Application::OnRender()
	{
		Renderer::Begin();

		m_Camera.Begin();
		m_Generator.OnRender();
		m_Camera.End();

		OnGuiRender();
		if (m_Debug)
		{
			const u16 total = m_Generator.GetWidth() * m_Generator.GetHeight();

			const u32 screenWidth  = GetScreenWidth();
			const u32 screenHeight = GetScreenHeight();
			const u32 numVisited   = m_Generator.GetNumVisited();

			const f32 percentage = m_Generator.GetPercentageFinish();

			Renderer::DrawPerformanceMetrics();

			Renderer::RenderText(TextFormat("Maze size: %dx%d", m_Generator.GetWidth(), m_Generator.GetHeight()), 5, 65);
			Renderer::RenderText(TextFormat("Cell size: %d", m_Generator.GetCellSize()), 5, 95);
			Renderer::RenderText(TextFormat("Num. visited: %d/%d [%.2f%%]", numVisited, total, percentage), 5, 125);
			Renderer::RenderText(TextFormat("Version: %s", MAZE_VERSION), 5, 155);
			Renderer::RenderText(TextFormat("Dimensions: %hux%hu", screenWidth, screenHeight), 5, 185);

			m_Camera.OnRender(); // Ends at y = 275.
		}

		Renderer::End();
	}

	void Application::OnGuiRender()
	{
		if (m_HideGUI)
			return;

		const f32 screenWidth  = (f32)GetScreenWidth();
		const f32 screenHeight = (f32)GetScreenHeight();

		// Play section.
		{
			constexpr f32 panelWidth   = 515.0f;
			constexpr f32 panelHeight  = 90.0f;
			constexpr f32 buttonWidth  = 250.0f;
			constexpr f32 buttonHeight = 50.0f;

			const Rectangle panel =
			{
				0.0f,
				screenHeight - panelHeight,
				panelWidth,
				panelHeight
			};

			GuiPanel(panel, "Play");

			if (GuiButton({ 5.0f, screenHeight - buttonHeight - 7, buttonWidth, buttonHeight }, "Start"))
				m_Generating = true;

			if (GuiButton({ buttonWidth + 10.0f, screenHeight - buttonHeight - 7, buttonWidth, buttonHeight }, "Reset"))
				ResetMaze();
		}

		// Size section.
		{
			constexpr f32 panelWidth   = 165.0f;
			constexpr f32 panelHeight  = 90.0f;
			constexpr f32 buttonWidth  = 50.0f;
			constexpr f32 buttonHeight = 50.0f;

			const Rectangle panel =
			{
				515.0f,
				screenHeight - panelHeight,
				panelWidth,
				panelHeight
			};

			u8 cellSize = m_Generator.GetCellSize();

			GuiPanel(panel, "Cell Size [px]");

			if (GuiButton({ panel.x + 5.0f, screenHeight - buttonHeight - 7, buttonWidth, buttonHeight }, "<"))
			{
				cellSize = std::clamp<u8>(u8(cellSize >> 1), MIN_CELL_SIZE, MAX_CELL_SIZE);

				m_Generator.SetCellSize(cellSize);
				ResetMaze();
			}
			else if (GuiButton({ panel.width - buttonWidth + 510.0f, screenHeight - buttonHeight - 7, buttonWidth, buttonHeight }, ">"))
			{
				cellSize = std::clamp<u8>(u8(cellSize << 1), MIN_CELL_SIZE, MAX_CELL_SIZE);

				m_Generator.SetCellSize(cellSize);
				ResetMaze();
			}

			GuiDrawText(TextFormat("%d", cellSize), panel, TEXT_ALIGN_CENTER, { 255, 255, 255, 128 });
		}

		// Speed
		{
			constexpr f32 panelWidth   = 155.0f;
			constexpr f32 panelHeight  = 90.0f;
			constexpr f32 buttonWidth  = 50.0f;
			constexpr f32 buttonHeight = 50.0f;

			const Rectangle panel =
			{
				680.0f,
				screenHeight - panelHeight,
				panelWidth,
				panelHeight
			};

			u8 cellSize = m_Generator.GetCellSize();

			GuiPanel(panel, "Speed");

			if (GuiButton({ panel.x + 5.0f, screenHeight - buttonHeight - 7, buttonWidth, buttonHeight }, "<"))
			{
				m_Speed >>= 1;
			}
			else if (GuiButton({ panel.width - buttonWidth + 675.0f, screenHeight - buttonHeight - 7, buttonWidth, buttonHeight }, ">"))
			{
				m_Speed <<= 1;
			}

			m_Speed = std::clamp<u8>(m_Speed, MIN_GENERATION_SPEED, MAX_GENERATION_SPEED);

			GuiDrawText(TextFormat("%dx", m_Speed), panel, TEXT_ALIGN_CENTER, { 255, 255, 255, 128 });
		}

		// Camera
		{
			constexpr f32 panelWidth  = 155.0f;
			constexpr f32 panelHeight = 90.0f;
			constexpr f32 boxWidth    = 50.0f;
			constexpr f32 boxHeight   = 50.0f;

			const Rectangle panel =
			{
				835.0f,
				screenHeight - panelHeight,
				panelWidth,
				panelHeight
			};
			const Rectangle button = { panel.x + 5.0f, screenHeight - boxHeight - 7, boxWidth, boxHeight };

			GuiPanel(panel, "Camera");

			m_Follow = GuiCheckBox(button, "Follow  ", m_Follow);
		}
	}

	void Application::ResetMaze()
	{
		const u8 cellSize = m_Generator.GetCellSize();

		const u16 width  = m_Generator.GetWidth();
		const u16 height = m_Generator.GetHeight();

		m_Generator = MazeGenerator::Create(cellSize, width, height);

		m_Generating = false;
	}
}