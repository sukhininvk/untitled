#include "game.h"

#include <SDL3/SDL_log.h>
#include <SDL3/SDL_timer.h>

namespace Untitled
{
    Game::~Game()
    {
        Shutdown();
    }

    int Game::Run()
    {
        if (!Initialize())
        {
            Shutdown();
            return 1;
        }

        Uint64 previousTicks = SDL_GetTicks();

        while (m_running)
        {
            const Uint64 currentTicks = SDL_GetTicks();
            const float deltaTime = static_cast<float>(currentTicks - previousTicks) / 1000.0f;
            previousTicks = currentTicks;

            ProcessEvents();
            Update(deltaTime);
            Render();
        }

        Shutdown();
        return 0;
    }

    bool Game::Initialize()
    {
        if (!SDL_Init(SDL_INIT_VIDEO))
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL: %s", SDL_GetError());
            return false;
        }

        m_window = SDL_CreateWindow("Orbit", 720, 540, 0);
        if (!m_window)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create window: %s", SDL_GetError());
            return false;
        }

        m_renderer = SDL_CreateRenderer(m_window, nullptr);
        if (!m_renderer)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create renderer: %s", SDL_GetError());
            return false;
        }
        SDL_SetRenderLogicalPresentation(m_renderer, 360, 270, SDL_LOGICAL_PRESENTATION_LETTERBOX);

        m_running = true;
        return true;
    }

    void Game::Shutdown()
    {
        if (m_renderer)
        {
            SDL_DestroyRenderer(m_renderer);
            m_renderer = nullptr;
        }

        SDL_DestroyWindow(m_window);
        SDL_Quit();
    }

    void Game::ProcessEvents()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                m_running = false;
            }
        }
    }

    void Game::Update(float deltaTime)
    {
        (void)deltaTime;
    }

    void Game::Render()
    {
        SDL_RenderClear(m_renderer);
        SDL_RenderPresent(m_renderer);
    }
}
