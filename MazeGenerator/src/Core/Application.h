#pragma once

#include "MazeGeneration/MazeGenerator.h"
#include "Renderer/Viewport.h"

#include <cstdint>

namespace maze
{
    class Application
    {
    public:
        Application();
        ~Application();

        void Run();

    private:
        void InitGUI();

        void OnUpdate();
        void OnRender();
        void OnGuiRender();

        void ResetMaze();

    private:
        bool          m_IsRunning = true;
        bool          m_Debug = false;
        bool          m_Generating = false;
        bool          m_HideGUI = false;
        bool          m_Follow = false;
        
        uint8_t       m_Speed = 1;
        
        MazeGenerator m_Generator;
        
        Viewport      m_Camera;
    };
}