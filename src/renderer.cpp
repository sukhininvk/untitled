#include "renderer.h"

#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>

namespace Untitled
{
	Renderer::~Renderer()
	{
		Destroy();
	}

	bool Renderer::Create(SDL_Window* window)
	{
		renderer_properties = SDL_CreateProperties();
		if (!renderer_properties)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create renderer properties: %s", SDL_GetError());
			return false;
		}

		SDL_SetPointerProperty(renderer_properties, SDL_PROP_RENDERER_CREATE_WINDOW_POINTER, window);

		renderer = SDL_CreateRendererWithProperties(renderer_properties);
		if (!renderer)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create renderer: %s", SDL_GetError());
			return false;
		}

		SDL_DestroyProperties(renderer_properties);

		return true;
	}

	void Renderer::Destroy()
	{
		if (renderer)
		{
			SDL_DestroyRenderer(renderer);
			renderer = nullptr;
		}
	}

	SDL_Renderer* Renderer::GetNativeHandle() const
	{
		return renderer;
	}
}
