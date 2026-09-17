#ifndef UNTITLED_RENDERER_H
#define UNTITLED_RENDERER_H

#include "preferences.h"

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_properties.h>

namespace Untitled
{
	class Renderer
	{
	public:
		Renderer() = default;
		~Renderer();

		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		bool Create(SDL_Window* window, const Preferences& preferences);
		void Destroy();

		[[nodiscard]] SDL_Renderer* GetNativeHandle() const;

	private:
		SDL_PropertiesID renderer_properties;
		SDL_Renderer* renderer = nullptr;
	};
}

#endif