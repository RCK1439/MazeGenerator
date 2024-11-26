#include "Application.h"
#include "Constants.h"
#include "Renderer/Renderer.h"

#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <extras/raygui.h>
#include <algorithm>

namespace maze
{

    Application::Application() :
        m_Camera(Vector2 { static_cast<float>(SCREEN_WIDTH) * 0.5f, static_cast<float>(SCREEN_HEIGHT) * 0.5f },
                 Vector2 { static_cast<float>(SCREEN_WIDTH) * 0.5f, static_cast<float>(SCREEN_HEIGHT) * 0.5f })
    {
        SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);

#ifdef MG_RELEASE
        SetTraceLogLevel(LOG_NONE);
#endif
        InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Maze Generator");

        SetWindowMinSize(1280, 640);

        const Image icon = LoadImage("res/icon.png");
        SetWindowIcon(icon);
        UnloadImage(icon);

        Renderer::Init();
        InitGUI();

        m_Generator = MazeGenerator::Create();
    }

    Application::~Application()
    {
        Renderer::Shutdown();
        CloseWindow();
    }

    void Application::Run()
    {
        while (!WindowShouldClose())
        {
            OnUpdate();
            OnRender();
        }
    }

    void Application::InitGUI()
    {
        constexpr int32_t GUI_GRAY = 0x222222FF;
        constexpr int32_t GUI_LIGHT_GRAY = 0x333333FF;
        constexpr int32_t GUI_DARK_GRAY = 0x111111FF;
        constexpr int32_t GUI_BLUE = 0xFEFEFEFF;
        constexpr int32_t GUI_LIGHT_BLUE = 0xFFFFFFFF;
        constexpr int32_t GUI_DARK_BLUE = 0xEEEEEEFF;

        GuiSetFont(Renderer::GetFont());

        GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, GUI_GRAY);
        GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, GUI_LIGHT_GRAY);
        GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, GUI_DARK_GRAY);

        GuiSetStyle(DEFAULT, TEXT_SIZE, 16);
        GuiSetStyle(DEFAULT, TEXT_SPACING, 1);
        GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);

        GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, GUI_BLUE);
        GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, GUI_LIGHT_BLUE);
        GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, GUI_DARK_BLUE);

        GuiSetStyle(SLIDER, BASE_COLOR_NORMAL, GUI_GRAY);
        GuiSetStyle(SLIDER, BASE_COLOR_FOCUSED, GUI_LIGHT_GRAY);
        GuiSetStyle(SLIDER, BASE_COLOR_PRESSED, GUI_DARK_GRAY);

        GuiSetStyle(DEFAULT, BACKGROUND_COLOR, GUI_GRAY);
        GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, GUI_DARK_GRAY);
        GuiSetStyle(DEFAULT, BORDER_COLOR_NORMAL, GUI_LIGHT_BLUE);

        GuiFade(0.7f);
    }

    void Application::OnUpdate()
    {
        if (IsKeyPressed(KEY_F3))
        {
            m_Debug = !m_Debug;
        }

        if (IsKeyPressed(KEY_H))
        {
            m_HideGUI = !m_HideGUI;
        }

        if (IsKeyPressed(KEY_F))
        {
            const int32_t monitor = GetCurrentMonitor();
            const int32_t width = GetMonitorWidth(monitor);
            const int32_t height = GetMonitorHeight(monitor);

            SetWindowSize(width, height);
            ToggleFullscreen();
        }

        if (IsWindowResized())
        {
            ResetMaze();
            m_Camera.OnResize();
        }

        if (m_Generating)
        {
            for (uint8_t i = 0; i < m_Speed; i++)
            {
                m_Generator.OnUpdate();
                m_Generating = !m_Generator.IsFinish();
            }
        }

        m_Camera.OnUpdate(m_Generator.GetCurrentPosition(), m_Follow);
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
            const uint16_t total = m_Generator.GetWidth() * m_Generator.GetHeight();
            const uint32_t numVisited = m_Generator.GetNumVisited();
            const float percentage = m_Generator.GetPercentageFinish();

            Renderer::DrawPerformanceMetrics(); // Ends at y = 95.

            Renderer::RenderText(TextFormat("Maze size: %dx%d", m_Generator.GetWidth(), m_Generator.GetHeight()), 5, 125);
            Renderer::RenderText(TextFormat("Cell size: %d", m_Generator.GetCellSize()), 5, 155);
            Renderer::RenderText(TextFormat("Num. visited: %d/%d [%.2f%%]", numVisited, total, percentage), 5, 185);
            Renderer::RenderText(TextFormat("Version: %s", MAZE_VERSION.data()), 5, 215);

            m_Camera.OnRender(); // Ends at y = 305.
        }

        Renderer::End();
    }

    void Application::OnGuiRender()
    {
        if (m_HideGUI)
        {
            return;
        }

        const float screenHeight = static_cast<float>(GetScreenHeight());

        // Play section.
        {
            constexpr float PLAY_PANEL_WIDTH = 515.0f;
            constexpr float PLAY_PANEL_HEIGHT = 90.0f;
            constexpr float PLAY_BUTTON_WIDTH = 250.0f;
            constexpr float PLAY_BUTTON_HEIGHT = 50.0f;

            const Rectangle playPanel =
            {
                0.0f,
                screenHeight - PLAY_PANEL_HEIGHT,
                PLAY_PANEL_WIDTH,
                PLAY_PANEL_HEIGHT
            };

            GuiPanel(playPanel, "Play");

            if (GuiButton({ 5.0f, screenHeight - PLAY_BUTTON_HEIGHT - 7, PLAY_BUTTON_WIDTH, PLAY_BUTTON_HEIGHT }, "Start"))
            {
                m_Generating = true;
            }

            if (GuiButton({ PLAY_BUTTON_WIDTH + 10.0f, screenHeight - PLAY_BUTTON_HEIGHT - 7, PLAY_BUTTON_WIDTH, PLAY_BUTTON_HEIGHT }, "Reset"))
            {
                ResetMaze();
            }
        }

        // Size section.
        {
            constexpr float SIZE_PANEL_WIDTH = 165.0f;
            constexpr float SIZE_PANEL_HEIGHT = 90.0f;
            constexpr float SIZE_BUTTON_WIDTH = 50.0f;
            constexpr float SIZE_BUTTON_HEIGHT = 50.0f;

            const Rectangle sizePanel =
            {
                515.0f,
                screenHeight - SIZE_PANEL_HEIGHT,
                SIZE_PANEL_WIDTH,
                SIZE_PANEL_HEIGHT
            };

            uint8_t cellSize = m_Generator.GetCellSize();

            GuiPanel(sizePanel, "Cell Size [px]");

            if (GuiButton({ sizePanel.x + 5.0f, screenHeight - SIZE_BUTTON_HEIGHT - 7, SIZE_BUTTON_WIDTH, SIZE_BUTTON_HEIGHT }, "<"))
            {
                cellSize = std::clamp(static_cast<uint8_t>(cellSize >> 1), MIN_CELL_SIZE, MAX_CELL_SIZE);

                m_Generator.SetCellSize(cellSize);
                ResetMaze();
            }
            else if (GuiButton({ sizePanel.width - SIZE_BUTTON_WIDTH + 510.0f, screenHeight - SIZE_BUTTON_HEIGHT - 7, SIZE_BUTTON_WIDTH, SIZE_BUTTON_HEIGHT }, ">"))
            {
                cellSize = std::clamp(static_cast<uint8_t>(cellSize << 1), MIN_CELL_SIZE, MAX_CELL_SIZE);

                m_Generator.SetCellSize(cellSize);
                ResetMaze();
            }

            GuiDrawText(TextFormat("%d", cellSize), sizePanel, TEXT_ALIGN_CENTER, { 255, 255, 255, 128 });
        }

        // Speed
        {
            constexpr float SPEED_PANEL_WIDTH = 155.0f;
            constexpr float SPEED_PANEL_HEIGHT = 90.0f;
            constexpr float SPEED_BUTTON_WIDTH = 50.0f;
            constexpr float SPEED_BUTTON_HEIGHT = 50.0f;

            const Rectangle speedPanel =
            {
                680.0f,
                screenHeight - SPEED_PANEL_HEIGHT,
                SPEED_PANEL_WIDTH,
                SPEED_PANEL_HEIGHT
            };

            GuiPanel(speedPanel, "Speed");

            if (GuiButton({ speedPanel.x + 5.0f, screenHeight - SPEED_BUTTON_HEIGHT - 7, SPEED_BUTTON_WIDTH, SPEED_BUTTON_HEIGHT }, "<"))
            {
                m_Speed >>= 1;
            }
            else if (GuiButton({ speedPanel.width - SPEED_BUTTON_WIDTH + 675.0f, screenHeight - SPEED_BUTTON_HEIGHT - 7, SPEED_BUTTON_WIDTH, SPEED_BUTTON_HEIGHT }, ">"))
            {
                m_Speed <<= 1;
            }

            m_Speed = std::clamp(m_Speed, MIN_GENERATION_SPEED, MAX_GENERATION_SPEED);

            GuiDrawText(TextFormat("%dx", m_Speed), speedPanel, TEXT_ALIGN_CENTER, { 255, 255, 255, 128 });
        }

        // Camera
        {
            constexpr float panelWidth  = 155.0f;
            constexpr float panelHeight = 90.0f;
            constexpr float boxWidth    = 50.0f;
            constexpr float boxHeight   = 50.0f;

            const Rectangle panel =
            {
                835.0f,
                screenHeight - panelHeight,
                panelWidth,
                panelHeight
            };
            const Rectangle button = 
            { 
                panel.x + 5.0f,
                screenHeight - boxHeight - 7,
                boxWidth,
                boxHeight
            };

            GuiPanel(panel, "Camera");

            m_Follow = GuiCheckBox(button, "Follow             ", m_Follow);
        }
    }

    void Application::ResetMaze()
    {
        const uint8_t cellSize = m_Generator.GetCellSize();

        const uint16_t width  = m_Generator.GetWidth();
        const uint16_t height = m_Generator.GetHeight();

        m_Generator = MazeGenerator::Create(cellSize, width, height);

        m_Generating = false;
    }
}