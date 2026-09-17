#ifndef UNTITLED_PREFERENCES_H
#define UNTITLED_PREFERENCES_H

#include <nlohmann/json.hpp>

namespace Untitled
{
	enum class DisplayMode
	{
		Windowed,
		Borderless,
		Fullscreen
	};

	enum class VSyncMode
	{
		Off,
		On,
		Half,
		Adaptive
	};

	struct DisplayPreferences
	{
		int width = 2160;
		int height = 1440;
		DisplayMode mode = DisplayMode::Windowed;
		int max_fps = 0; // 0 = unlimited, otherwise specify the max framerate
		VSyncMode vsync = VSyncMode::On;
	};

	struct Preferences
	{
		bool Load();
		bool Save() const;
		void SetDefaults();
		void CheckValid();

		DisplayPreferences display;	
	};

	void to_json(nlohmann::json& j, const Preferences& p);
	void from_json(const nlohmann::json& j, Preferences& p);
}

#endif
