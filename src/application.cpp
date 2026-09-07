#include "application.h"
#include "preferences.h"
#include "appinfo.h"

#include <SDL3/SDL_log.h>
#include <SDL3/SDL_timer.h>

namespace Untitled
{
    Application::~Application()
    {
        Shutdown();
    }

    int Application::Run()
    {
        if (!Initialize())
        {
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

        return 0;
    }

    bool Application::Initialize()
    {
        if (!SDL_Init(SDL_INIT_VIDEO))
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL: %s", SDL_GetError());
            return false;
        }

        SDL_PropertiesID props = SDL_CreateProperties();
        if (!props)
        {
            SDL_Log("Failed to create properties: %s", SDL_GetError());
            return false;
        }

        SDL_SetStringProperty(props, SDL_PROP_WINDOW_CREATE_TITLE_STRING, WINDOW_TITLE);
        SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, 2560);
        SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, 1440);

        m_window = SDL_CreateWindowWithProperties(props);
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

		SDL_DestroyProperties(props);
        
        m_running = true;
        return true;
    }

    void Application::Shutdown()
    {
        if (m_renderer)
        {
            SDL_DestroyRenderer(m_renderer);
            m_renderer = nullptr;
        }

        if (m_window)
        {
            SDL_DestroyWindow(m_window);
            m_window = nullptr;
        }

        SDL_Quit();
        m_running = false;
    }

    void Application::ProcessEvents()
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

    void Application::Update(float deltaTime)
    {
        (void)deltaTime;
    }

    void Application::Render()
    {
        SDL_RenderClear(m_renderer);
        SDL_RenderPresent(m_renderer);
    }
}
