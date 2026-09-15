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

		Uint64 previous_counter = SDL_GetPerformanceCounter();

		while (is_running)
		{
			Uint64 current_counter = SDL_GetPerformanceCounter();

			double delta_time =
				static_cast<double>(current_counter - previous_counter) /
				static_cast<double>(SDL_GetPerformanceFrequency());

			previous_counter = current_counter;

			ProcessEvents();
			Update(delta_time);
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

		SDL_PropertiesID window_properties = SDL_CreateProperties();
		if (!window_properties)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create window properties: %s", SDL_GetError());
			return false;
		}

		// Apply display preferences to the window properties
		SDL_SetStringProperty(window_properties, SDL_PROP_WINDOW_CREATE_TITLE_STRING, WINDOW_TITLE);
		SDL_SetNumberProperty(window_properties, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, preferences.display.width);
		SDL_SetNumberProperty(window_properties, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, preferences.display.height);

		switch (preferences.display.mode)
		{
		case 1:
			SDL_SetBooleanProperty(window_properties, SDL_PROP_WINDOW_CREATE_BORDERLESS_BOOLEAN, true);
			break;

		case 2:
			SDL_SetBooleanProperty(window_properties, SDL_PROP_WINDOW_CREATE_FULLSCREEN_BOOLEAN, true);
			break;
		}

		main_window = SDL_CreateWindowWithProperties(window_properties);
		if (!main_window)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create window: %s", SDL_GetError());
			return false;
		}
		SDL_DestroyProperties(window_properties);

		SDL_PropertiesID renderer_properties = SDL_CreateProperties();
		if (!renderer_properties)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create renderer properties: %s", SDL_GetError());
			return false;
		}

		// Set the window pointer property for the renderer
		SDL_SetPointerProperty(renderer_properties, SDL_PROP_RENDERER_CREATE_WINDOW_POINTER, main_window);

		// Apply display preferences to the renderer properties
		switch (preferences.display.vsync)
		{
		case 1:
			SDL_SetNumberProperty(renderer_properties, SDL_PROP_RENDERER_CREATE_PRESENT_VSYNC_NUMBER, 1);
			break;
		case 2:
			SDL_SetNumberProperty(renderer_properties, SDL_PROP_RENDERER_CREATE_PRESENT_VSYNC_NUMBER, -1);
			break;
		}

		main_renderer = SDL_CreateRendererWithProperties(renderer_properties);
		if (!main_renderer)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create renderer: %s", SDL_GetError());
			return false;
		}
		SDL_DestroyProperties(renderer_properties);
		
		is_running = true;
		return true;
	}

	void Application::Shutdown()
	{
		if (main_renderer)
		{
			SDL_DestroyRenderer(main_renderer);
			main_renderer = nullptr;
		}

		if (main_window)
		{
			SDL_DestroyWindow(main_window);
			main_window = nullptr;
		}

		SDL_Quit();
		is_running = false;
	}

	void Application::ProcessEvents()
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_EVENT_QUIT)
			{
				is_running = false;
			}
		}
	}

	void Application::Update(double delta_time)
	{
		(void)delta_time;
	}

	void Application::Render()
	{
		SDL_RenderClear(main_renderer);
		SDL_RenderPresent(main_renderer);
	}
}
