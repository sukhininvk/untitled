#include "preferences.h"

#include <SDL3/SDL.h>
#include <nlohmann/json.hpp>

namespace Untitled
{
	bool Preferences::Load()
	{
		// Load preferences from a file or other source
		return true;
	}

	bool Preferences::Save() const
	{
		// Save preferences to a file
		return true;
	}

	void Preferences::SetDefaults()
	{
		const SDL_DisplayID display_id = SDL_GetPrimaryDisplay();
		const SDL_DisplayMode* desktop_mode = SDL_GetDesktopDisplayMode(display_id);

		if (desktop_mode)
		{
			display.width = desktop_mode->w;
			display.height = desktop_mode->h;
		}

		display.mode = 2;
		display.vsync = 1;
	}

	void Preferences::CheckValid()
	{
		
	}
}
