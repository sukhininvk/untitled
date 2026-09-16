#include "paths.h"
#include "preferences.h"

#include <SDL3/SDL.h>
#include <nlohmann/json.hpp>
#include <fstream>

namespace Untitled
{
	bool Preferences::Load()
	{
		/*
		if (!APPDATA_PATH)
			return false;

		std::string file_path = std::string(APPDATA_PATH) + "preferences.json";

		std::ifstream file(file_path);

		if (!file)
			return false;

		nlohmann::json j;
		file >> j;

		j.get_to(*this);
		*/
		return true;
	}

	bool Preferences::Save() const
	{
		/*
		if (!APPDATA_PATH)
			return false;

		std::string file_path = std::string(APPDATA_PATH) + "preferences.json";

		std::ofstream file(file_path);

		if (!file)
			return false;

		nlohmann::json j = *this;

		file << j.dump(4);
		*/
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
	}

	void Preferences::CheckValid()
	{
		
	}

	void to_json(nlohmann::json& j, const Preferences& p)
	{

	}

	void from_json(const nlohmann::json& j, Preferences& p)
	{

	}
}
