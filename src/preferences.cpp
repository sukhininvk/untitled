#include "preferences.h"

#include <SDL3/SDL.h>
#include <nlohmann/json.hpp>

namespace Untitled
{
	bool Preferences::load()
	{
		// Load preferences from a file or other source
		return true;
	}

	bool Preferences::save() const
	{
		// Save preferences to a file
		return true;
	}

	void Preferences::setDefaults()
	{
		const SDL_DisplayID display = SDL_GetPrimaryDisplay();
		const SDL_DisplayMode* mode = SDL_GetDesktopDisplayMode(display);

		if (mode)
		{
			displayPreferences.displayWidth = mode->w;
			displayPreferences.displayHeight = mode->h;
		}
	}
}
