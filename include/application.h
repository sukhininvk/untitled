#ifndef UNTITLED_APPLICATION_H
#define UNTITLED_APPLICATION_H

#include "preferences.h"
#include "renderer.h"
#include "window.h"

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

		Window main_window;
		Renderer main_renderer;
		Preferences preferences;
	};
}

#endif
