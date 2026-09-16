#include "application.h"
#include "preferences.h"

#include <SDL3/SDL_init.h>
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
		
		if (!main_window.Create(preferences))
		{
			return false;
		}

		if (!main_renderer.Create(main_window.GetNativeHandle(), preferences))
		{
			return false;
		}

		is_running = true;
		return true;
	}

	void Application::Shutdown()
	{
		main_renderer.Destroy();
		main_window.Destroy();

		SDL_Quit();
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
		SDL_RenderClear(main_renderer.GetNativeHandle());
		SDL_RenderPresent(main_renderer.GetNativeHandle());
	}
}
