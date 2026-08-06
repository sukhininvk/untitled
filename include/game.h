#ifndef UNTITLED_GAME_H
#define UNTITLED_GAME_H

#include <SDL3/SDL.h>

namespace Untitled
{
    class Game
    {
    public:
        Game() = default;
        ~Game();

        Game(const Game&) = delete;
        Game& operator=(const Game&) = delete;

        int Run();

    private:
        bool Initialize();
        void Shutdown();
        void ProcessEvents();
        static void Update(float deltaTime);
        void Render();

        bool m_running = true;
        SDL_Window* m_window = nullptr;
        SDL_Renderer* m_renderer = nullptr;
    };
}

#endif
