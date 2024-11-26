#include "Viewport.h"

#include "Core/Constants.h"

#include "Renderer/Renderer.h"

#include <raymath.h>
#include <algorithm>

namespace maze
{
    static constexpr float ZOOM_SPEED = 5.0f;
    static constexpr float PAN_SPEED  = 250.0f;

    static constexpr float MIN_ZOOM = 1.0f;
    static constexpr float MAX_ZOOM = 8.0f;

    Viewport::Viewport(Vector2 target, Vector2 offset) :
        m_Camera { offset, target, 0.0f, MIN_ZOOM }
    {}

    void Viewport::OnUpdate(Vector2 position, bool follow)
    {
        const float dt = GetFrameTime();

        if (IsKeyDown(KEY_Q))  m_Camera.zoom += ZOOM_SPEED * dt;
        if (IsKeyDown(KEY_E))  m_Camera.zoom -= ZOOM_SPEED * dt;

        if (IsKeyDown(KEY_W)) m_Camera.target.y -= PAN_SPEED * dt;
        if (IsKeyDown(KEY_A)) m_Camera.target.x -= PAN_SPEED * dt;
        if (IsKeyDown(KEY_S)) m_Camera.target.y += PAN_SPEED * dt;
        if (IsKeyDown(KEY_D)) m_Camera.target.x += PAN_SPEED * dt;

        if (follow)
        {
            constexpr float MIN_SPEED = 30.0f;
            constexpr float MIN_EFFECT_LENGTH = 10.0f;

            const Vector2 diff = Vector2Subtract(position, m_Camera.target);

            const float length = Vector2Length(diff);
            if (length > MIN_EFFECT_LENGTH)
            {
                const float speed = std::max(length, MIN_SPEED);
                m_Camera.target = Vector2Add(m_Camera.target, Vector2Scale(diff, speed * dt / length));
            }
        }

        m_Camera.zoom = std::clamp(m_Camera.zoom, MIN_ZOOM, MAX_ZOOM);

        const Vector2 min = Vector2
        {
            m_Camera.offset.x / m_Camera.zoom,
            m_Camera.offset.y / m_Camera.zoom
        };
        const Vector2 max = Vector2
        {
            2.0f * m_Camera.offset.x - min.x,
            2.0f * m_Camera.offset.y - min.y
        };

        m_Camera.target = Vector2Clamp(m_Camera.target, min, max);
    }

    void Viewport::OnRender() const
    {
        Renderer::RenderText("Camera: ", 5, 245);
        Renderer::RenderText(TextFormat(" - Target: %.2f, %.2f (Use [W], [A], [S] and [D] to move)", m_Camera.target.x, m_Camera.target.y), 5, 275);
        Renderer::RenderText(TextFormat(" - Zoom: %.2f (Use [Q]/[E] to zoom)", m_Camera.zoom), 5, 305);
    }

    void Viewport::OnResize()
    {
        m_Camera.offset = Vector2
        {
            static_cast<float>(GetScreenWidth()) * 0.5f,
            static_cast<float>(GetScreenHeight()) * 0.5f
        };
        m_Camera.target = m_Camera.offset;
        m_Camera.zoom = MIN_ZOOM;
    }

    void Viewport::Begin() const
    {
        BeginMode2D(m_Camera);
    }

    void Viewport::End() const
    {
        EndMode2D();
    }
}