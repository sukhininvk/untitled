#ifndef UNTITLED_PREFERENCES_H
#define UNTITLED_PREFERENCES_H

#include <nlohmann/json.hpp>

#include <functional>
#include <string>
#include <unordered_map>

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
		int width = 1280;
		int height = 720;
		DisplayMode mode = DisplayMode::Windowed;
		int max_fps = 0; // 0 = unlimited, otherwise specify the max framerate
		VSyncMode vsync = VSyncMode::On;

		void SetDefaults();
		void CheckValid();
	};

	class Preferences
	{
	public:
		Preferences() = default;
		~Preferences() = default;

		Preferences(const Preferences&) = delete;
		Preferences& operator=(const Preferences&) = delete;

		Preferences(Preferences&&) = delete;
		Preferences& operator=(Preferences&&) = delete;

		template<typename T>
		bool RegisterSection(const std::string& name, T& preferences);

		bool Load();
		bool Save() const;

		void SetDefaults();
		void CheckValid();

	private:
		struct Section
		{
			std::function<void()> set_defaults;
			std::function<void()> check_valid;

			std::function<void(nlohmann::json&)> to_json;
			std::function<void(const nlohmann::json&)> from_json;
		};

		std::unordered_map<std::string, Section> sections;
	};

	template<typename T>
	bool Preferences::RegisterSection(const std::string& name, T& preferences)
	{
		if (name.empty())
			return false;

		if (sections.contains(name))
			return false;

		sections.emplace(
			name,
			Section
			{
				[&preferences]() { preferences.SetDefaults(); },
				[&preferences]() { preferences.CheckValid(); },
				[&preferences](nlohmann::json& j) { j = preferences; },
				[&preferences](const nlohmann::json& j) { j.get_to(preferences); }
			}
		);

		return true;
	}
}

#endif
