/*
* Ruan C. Keet (2023)
* Viewport.h
*/

#pragma once

#include "Core/Types.h"

#include <raylib.h>

namespace maze
{
    /**
    * \brief This class represents the viewport into the maze.
    * 
    * It's basically a camera that allows the user to look, move and
    * zoom into the maze.
    */
    class Viewport
    {
    public:
        /**
        * \brief Constructs the viewport with a target
        * 
        * \param target The point that the camera is looking at.
        */
        Viewport(Vector2 target, Vector2 offset);

        /*
        * \brief Destroys the viewport.
        */
        ~Viewport() = default;

        /**
        * \brief Called once per frame to update the camera.
        * 
        * \param dt       The time elapsed since the previous frame.
        * \param position The position that the generator is currently at.
        * \param follow   Flag determining whether the camera should follow the position or not.
        */
        void OnUpdate(const f32 dt, Vector2 position, bool follow);

        /**
        * \brief Renders some debug info of the camera to the window.
        */
        void OnRender() const;

        /**
        * \brief Adjusts the camera when the window is resized.
        * 
        * \param mazeDimensions The width and height of the maze.
        */
        void OnResize(Vector2 mazeDimensions);

        /**
        * \brief Calls BeginMode2D()
        */
        void Begin();

        /**
        * \brief Calles EndMode2D() 
        */
        void End();

    private:
        Camera2D m_Camera;
    };
}