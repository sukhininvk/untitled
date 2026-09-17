#ifndef UNTITLED_WINDOW_H
#define UNTITLED_WINDOW_H

#include "preferences.h"

#include <SDL3/SDL_video.h>
#include <SDL3/SDL_properties.h>

namespace Untitled
{
	class Window
	{
	public:
		Window() = default;
		~Window();

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		bool Create(const Preferences& preferences);
		void Destroy();

		[[nodiscard]] SDL_Window* GetNativeHandle() const;

	private:
		SDL_PropertiesID window_properties;
		SDL_Window* window = nullptr;
	};
}

#endif
