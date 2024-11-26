#pragma once

#include <raylib.h>

namespace maze
{
    class Viewport
    {
    public:
        explicit Viewport(Vector2 target, Vector2 offset);
        ~Viewport() noexcept = default;

        void OnUpdate(Vector2 position, bool follow);
        void OnRender() const;

        void OnResize();

        void Begin() const;
        void End() const;

    private:
        Camera2D m_Camera;
    };
}