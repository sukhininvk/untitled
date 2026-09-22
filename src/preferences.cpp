#include "paths.h"
#include "preferences.h"

#include <nlohmann/json.hpp>
#include <SDL3/SDL_video.h>

#include <fstream>

namespace Untitled
{
	Preferences::Preferences()
	{
		RegisterCategory(
			"video",
			[this]() { video.SetDefaults(); },
			[this]() { video.CheckValid(); });

		RegisterParameter("video", "width", video.width);
		RegisterParameter("video", "height", video.height);
		RegisterParameter("video", "mode", video.mode);
	}

	bool Preferences::RegisterCategory(
		const std::string& name,
		std::function<void()> set_defaults,
		std::function<void()> check_valid)
	{
		if (name.empty() || sections.contains(name))
			return false;

		Section section;
		section.set_defaults = std::move(set_defaults);
		section.check_valid = std::move(check_valid);
		sections.emplace(name, std::move(section));

		return true;
	}

	bool Preferences::RegisterParameter(const std::string& category, const std::string& name, Parameter parameter)
	{
		if (category.empty() || name.empty())
			return false;

		auto category_it = sections.find(category);
		if (category_it == sections.end())
			return false;

		Section& section = category_it->second;
		if (section.uses_whole_object_serialization || section.parameters.contains(name))
			return false;

		section.parameters.emplace(name, std::move(parameter));
		return true;
	}

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

				const nlohmann::json& section_json = json[name];

				if (section.uses_whole_object_serialization)
				{
					section.from_json(section_json);
					continue;
				}

				if (!section_json.is_object())
					return false;

				for (auto& [parameter_name, parameter] : section.parameters)
				{
					if (section_json.contains(parameter_name))
						parameter.from_json(section_json[parameter_name]);
				}
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

			if (section.uses_whole_object_serialization)
			{
				section.to_json(section_json);
			}
			else
			{
				section_json = nlohmann::json::object();
				for (const auto& [parameter_name, parameter] : section.parameters)
				{
					nlohmann::json parameter_json;
					parameter.to_json(parameter_json);
					section_json[parameter_name] = std::move(parameter_json);
				}
			}

			json[name] = section_json;
		}

		file << json.dump(4);

		return file.good();
	}

	void Preferences::SetDefaults()
	{
		for (auto& [name, section] : sections)
		{
			if (section.set_defaults)
				section.set_defaults();

			for (auto& [parameter_name, parameter] : section.parameters)
			{
				if (parameter.has_default)
					parameter.from_json(parameter.default_value);
			}
		}
	}

	void Preferences::CheckValid()
	{
		for (auto& [name, section] : sections)
		{
			if (section.check_valid)
				section.check_valid();
		}
	}

	void VideoPreferences::SetDefaults()
	{
		width = 1280;
		height = 720;
		mode = DisplayMode::Fullscreen;

		const SDL_DisplayID display_id = SDL_GetPrimaryDisplay();

		if (!display_id)
			return;

		const SDL_DisplayMode* display_mode = SDL_GetDesktopDisplayMode(display_id);

		if (!display_mode)
			return;

		width = display_mode->w;
		height = display_mode->h;
	}

	void VideoPreferences::CheckValid()
	{
		if (width < 640)
			width = 640;

		if (height < 480)
			height = 480;

		if (max_fps < 30)
			max_fps = 0;

		if (static_cast<int>(mode) < 0 || static_cast<int>(mode) > 2)
			mode = DisplayMode::Windowed;
	}

	void to_json(nlohmann::json& j, const VideoPreferences& p)
	{
		j = nlohmann::json{
			{"width", p.width},
			{"height", p.height},
			{"mode", static_cast<int>(p.mode)}
		};
	}

	void from_json(const nlohmann::json& j, VideoPreferences& p)
	{
		j.at("width").get_to(p.width);
		j.at("height").get_to(p.height);
		p.mode = static_cast<DisplayMode>(j.at("mode").get<int>());
	}
}
