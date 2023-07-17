#include "Viewport.h"

#include "Core/Constants.h"
#include "Core/Log.h"

#include "Renderer/Renderer.h"

#include <raymath.h>
#include <algorithm>

namespace maze
{
    static constexpr f32 ZOOM_SPEED = 5.0f;
    static constexpr f32 PAN_SPEED  = 250.0f;

    static constexpr f32 MIN_ZOOM = 1.0f;
    static constexpr f32 MAX_ZOOM = 8.0f;

    Viewport::Viewport(Vector2 target, Vector2 offset)
    {
        m_Camera.offset = offset;
        m_Camera.target = target;
        m_Camera.rotation = 0.0f;
        m_Camera.zoom = MIN_ZOOM;
    }

    void Viewport::OnUpdate(const f32 dt, Vector2 position, bool follow)
    {
        if (IsKeyDown(KEY_KP_ADD))       m_Camera.zoom += ZOOM_SPEED * dt;
        if (IsKeyDown(KEY_KP_SUBTRACT))  m_Camera.zoom -= ZOOM_SPEED * dt;

        if (IsKeyDown(KEY_W)) m_Camera.target.y -= PAN_SPEED * dt;
        if (IsKeyDown(KEY_A)) m_Camera.target.x -= PAN_SPEED * dt;
        if (IsKeyDown(KEY_S)) m_Camera.target.y += PAN_SPEED * dt;
        if (IsKeyDown(KEY_D)) m_Camera.target.x += PAN_SPEED * dt;

        if (follow)
        {
            constexpr f32 MIN_SPEED = 30;
            constexpr f32 MIN_EFFECT_LENGTH = 10;

            const Vector2 diff = Vector2Subtract(position, m_Camera.target);

            const f32 length = Vector2Length(diff);
            if (length > MIN_EFFECT_LENGTH)
            {
                const f32 speed = std::max<f32>(length, MIN_SPEED);

                m_Camera.target = Vector2Add(m_Camera.target, Vector2Scale(diff, speed * dt / length));
            }
        }

        m_Camera.zoom = std::clamp<f32>(m_Camera.zoom, MIN_ZOOM, MAX_ZOOM);

        const Vector2 min = { m_Camera.offset.x / m_Camera.zoom, m_Camera.offset.y / m_Camera.zoom };
        const Vector2 max = { 2 * m_Camera.offset.x - min.x, 2 * m_Camera.offset.y - min.y };

        m_Camera.target = Vector2Clamp(m_Camera.target, min, max);
    }

    void Viewport::OnRender() const
    {
        Renderer::RenderText("Camera: ", 5, 215);
        Renderer::RenderText(TextFormat(" - Target: %.2f, %.2f (Use WASD to move)", m_Camera.target.x, m_Camera.target.y), 5, 245);
        Renderer::RenderText(TextFormat(" - Zoom: %.2f (Use +- to zoom)", m_Camera.zoom), 5, 275);
    }

    void Viewport::OnResize()
    {
        m_Camera.offset = { GetScreenWidth() * 0.5f, GetScreenHeight() * 0.5f };
        m_Camera.target = m_Camera.offset;
        m_Camera.zoom = MIN_ZOOM;
    }

    void Viewport::Begin()
    {
        BeginMode2D(m_Camera);
    }

    void Viewport::End()
    {
        EndMode2D();
    }
}