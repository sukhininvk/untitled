#include "appinfo.h"
#include "preferences.h"
#include "window.h"

#include <SDL3/SDL_log.h>
#include <SDL3/SDL_video.h>

namespace Untitled
{
	Window::~Window()
	{
		Destroy();
	}

	bool Window::Create(const Preferences& preferences)
	{
		window_properties = SDL_CreateProperties();
		if (!window_properties)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create window properties: %s", SDL_GetError());
			return false;
		}

		SDL_SetStringProperty(window_properties, SDL_PROP_WINDOW_CREATE_TITLE_STRING, WINDOW_TITLE);
		SDL_SetNumberProperty(window_properties, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, preferences.video.width);
		SDL_SetNumberProperty(window_properties, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, preferences.video.height);

		switch (preferences.video.mode)
		{
		case DisplayMode::Borderless:
			SDL_SetBooleanProperty(window_properties, SDL_PROP_WINDOW_CREATE_BORDERLESS_BOOLEAN, true);
			break;

		case DisplayMode::Fullscreen:
			SDL_SetBooleanProperty(window_properties, SDL_PROP_WINDOW_CREATE_FULLSCREEN_BOOLEAN, true);
			break;
		}

		window = SDL_CreateWindowWithProperties(window_properties);
		if (!window)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create window: %s", SDL_GetError());
			return false;
		}

		SDL_DestroyProperties(window_properties);

		return true;
	}

	void Window::Destroy()
	{
		if (window)
		{
			SDL_DestroyWindow(window);
			window = nullptr;
		}
	}

	SDL_Window* Window::GetNativeHandle() const
	{
		return window;
	}
}
