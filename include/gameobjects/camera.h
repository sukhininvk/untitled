#ifndef UNTITLED_CAMERA_H
#define UNTITLED_CAMERA_H

#include <SDL3/SDL.h>

namespace Untitled
{
    class Camera
    {
    public:
        void SetPosition(float x, float y);
        void SetZoom(float zoom);

        float GetX() const;
        float GetY() const;
        float GetZoom() const;

        SDL_FPoint WorldToScreen(SDL_FPoint position) const;
        SDL_FPoint ScreenToWorld(SDL_FPoint position) const;

    private:
        float m_x = 0.0f;
        float m_y = 0.0f;
        float m_zoom = 1.0f;
    };
}

#endif
