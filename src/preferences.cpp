#include "paths.h"
#include "preferences.h"

#include <SDL3/SDL.h>
#include <nlohmann/json.hpp>
#include <fstream>

namespace Untitled
{
	bool Preferences::Load()
	{
		if (!APPDATA_PATH)
			return false;

		const std::string file_path = std::string(APPDATA_PATH) + "preferences.json";

		std::ifstream file(file_path);
		if (!file)
			return false;

		try
		{
			nlohmann::json j;
			file >> j;
			j.get_to(*this);
		}
		catch (const nlohmann::json::exception&)
		{
			return false;
		}

		return true;
	}

	bool Preferences::Save() const
	{
		if (!APPDATA_PATH)
			return false;

		std::string file_path = std::string(APPDATA_PATH) + "preferences.json";

		std::ofstream file(file_path);

		if (!file)
			return false;

		nlohmann::json j = *this;

		file << j.dump(4);
		
		return file.good();
	}

	void Preferences::SetDefaults()
	{
		const SDL_DisplayID display_id = SDL_GetPrimaryDisplay();

		if (display_id != 0)
		{
			const SDL_DisplayMode* desktop_mode = SDL_GetDesktopDisplayMode(display_id);

			if (desktop_mode)
			{
				display.width = desktop_mode->w;
				display.height = desktop_mode->h;
			}
		}

		display.mode = DisplayMode::Fullscreen;
		display.vsync = VSyncMode::On;
	}

	void Preferences::CheckValid()
	{
		
	}

	void to_json(nlohmann::json& j, const Preferences& p)
	{
		j = {
			{
				"display",
				{
					{ "width", p.display.width },
					{ "height", p.display.height },
					{ "mode", static_cast<int>(p.display.mode) },
					{ "max_fps", p.display.max_fps },
					{ "vsync", static_cast<int>(p.display.vsync) }
				}
			}
		};
	}

	void from_json(const nlohmann::json& j, Preferences& p)
	{
		j.at("display").at("width").get_to(p.display.width);
		j.at("display").at("height").get_to(p.display.height);

		int mode;
		j.at("display").at("mode").get_to(mode);
		p.display.mode = static_cast<DisplayMode>(mode);

		int vsync;
		j.at("display").at("vsync").get_to(vsync);
		p.display.vsync = static_cast<VSyncMode>(vsync);
	}
}
