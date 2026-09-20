#include "paths.h"
#include "preferences.h"

#include <nlohmann/json.hpp>
#include <SDL3/SDL_video.h>

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
			nlohmann::json json;
			file >> json;

			for (auto& [name, section] : sections)
			{
				if (!json.contains(name))
					continue;

				section.from_json(json[name]);
			}
		}
		catch (const nlohmann::json::exception&)
		{
			return false;
		}

		CheckValid();

		return true;
	}

	bool Preferences::Save() const
	{
		if (!APPDATA_PATH)
			return false;

		const std::string file_path = std::string(APPDATA_PATH) + "preferences.json";

		std::ofstream file(file_path);

		if (!file)
			return false;

		nlohmann::json json;

		for (const auto& [name, section] : sections)
		{
			nlohmann::json section_json;
			section.to_json(section_json);
			json[name] = section_json;
		}

		file << json.dump(4);

		return file.good();
	}

	void Preferences::SetDefaults()
	{
		for (auto& [name, section] : sections)
		{
			section.set_defaults();
		}
	}

	void Preferences::CheckValid()
	{
		for (auto& [name, section] : sections)
		{
			section.check_valid();
		}
	}

	void DisplayPreferences::SetDefaults()
	{
		width = 1280;
		height = 720;
		mode = DisplayMode::Fullscreen;
		max_fps = 0;
		vsync = VSyncMode::On;

		const SDL_DisplayID display_id = SDL_GetPrimaryDisplay();

		if (!display_id)
			return;

		const SDL_DisplayMode* display_mode = SDL_GetDesktopDisplayMode(display_id);

		if (!display_mode)
			return;

		width = display_mode->w;
		height = display_mode->h;
	}

	void DisplayPreferences::CheckValid()
	{
		if (width < 640)
			width = 640;

		if (height < 480)
			height = 480;

		if (max_fps < 0)
			max_fps = 0;

		if (static_cast<int>(mode) < 0 || static_cast<int>(mode) > 2)
			mode = DisplayMode::Windowed;

		if (static_cast<int>(vsync) < 0 || static_cast<int>(vsync) > 3)
			vsync = VSyncMode::On;
	}

	void to_json(nlohmann::json& j, const DisplayPreferences& p)
	{
		j = nlohmann::json{
			{"width", p.width},
			{"height", p.height},
			{"mode", static_cast<int>(p.mode)},
			{"max_fps", p.max_fps},
			{"vsync", static_cast<int>(p.vsync)}
		};
	}

	void from_json(const nlohmann::json& j, DisplayPreferences& p)
	{
		j.at("width").get_to(p.width);
		j.at("height").get_to(p.height);
		p.mode = static_cast<DisplayMode>(j.at("mode").get<int>());
		j.at("max_fps").get_to(p.max_fps);
		p.vsync = static_cast<VSyncMode>(j.at("vsync").get<int>());
	}
}
