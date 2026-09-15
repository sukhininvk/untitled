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

        Uint64 previousCounter = SDL_GetPerformanceCounter();

        while (m_running)
        {
            Uint64 currentCounter = SDL_GetPerformanceCounter();

            double deltaTime =
                static_cast<double>(currentCounter - previousCounter) /
                static_cast<double>(SDL_GetPerformanceFrequency());

            previousCounter = currentCounter;

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

        SDL_PropertiesID windowProps = SDL_CreateProperties();
        if (!windowProps)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create window properties: %s", SDL_GetError());
            return false;
        }

		// Apply display preferences to the window properties
        SDL_SetStringProperty(windowProps, SDL_PROP_WINDOW_CREATE_TITLE_STRING, WINDOW_TITLE);
        SDL_SetNumberProperty(windowProps, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, m_preferences.displayPreferences.displayWidth);
        SDL_SetNumberProperty(windowProps , SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, m_preferences.displayPreferences.displayHeight);

        switch (m_preferences.displayPreferences.displayMode)
        {
        case 1:
            SDL_SetBooleanProperty(windowProps, SDL_PROP_WINDOW_CREATE_BORDERLESS_BOOLEAN, true);
            break;

        case 2:
            SDL_SetBooleanProperty(windowProps, SDL_PROP_WINDOW_CREATE_FULLSCREEN_BOOLEAN, true);
            break;
        }

        m_window = SDL_CreateWindowWithProperties(windowProps);
        if (!m_window)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create window: %s", SDL_GetError());
            return false;
        }
        SDL_DestroyProperties(windowProps);

        SDL_PropertiesID rendererProps = SDL_CreateProperties();
        if (!rendererProps)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create renderer properties: %s", SDL_GetError());
            return false;
        }

		// Set the window pointer property for the renderer
        SDL_SetPointerProperty(rendererProps, SDL_PROP_RENDERER_CREATE_WINDOW_POINTER, m_window);

        // Apply display preferences to the renderer properties
        switch (m_preferences.displayPreferences.vsync)
        {
        case 1:
            SDL_SetNumberProperty(rendererProps, SDL_PROP_RENDERER_CREATE_PRESENT_VSYNC_NUMBER, 1);
            break;
        case 2:
            SDL_SetNumberProperty(rendererProps, SDL_PROP_RENDERER_CREATE_PRESENT_VSYNC_NUMBER, -1);
            break;
        }

        m_renderer = SDL_CreateRendererWithProperties(rendererProps);
        if (!m_renderer)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create renderer: %s", SDL_GetError());
            return false;
        }
        SDL_DestroyProperties(rendererProps);
        
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
