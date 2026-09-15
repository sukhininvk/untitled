#ifndef UNTITLED_APPLICATION_H
#define UNTITLED_APPLICATION_H

#include "preferences.h"

#include <SDL3/SDL.h>

namespace Untitled
{
	class Application
	{
	public:
		Application() = default;
		~Application();

		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;

		int Run();

	private:
		bool Initialize();
		void Shutdown();
		void ProcessEvents();
		static void Update(double delta_time);
		void Render();

		bool is_running = true;
		SDL_Window* main_window = nullptr;
		SDL_Renderer* main_renderer = nullptr;

		Preferences preferences;
	};
}

#endif
