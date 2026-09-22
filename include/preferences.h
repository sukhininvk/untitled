#ifndef UNTITLED_PREFERENCES_H
#define UNTITLED_PREFERENCES_H

#include <nlohmann/json.hpp>

#include <functional>
#include <string>
#include <typeindex>
#include <type_traits>
#include <unordered_map>
#include <utility>

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

	class VideoPreferences
	{
	public:
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
		Preferences();
		~Preferences() = default;

		Preferences(const Preferences&) = delete;
		Preferences& operator=(const Preferences&) = delete;

		Preferences(Preferences&&) = delete;
		Preferences& operator=(Preferences&&) = delete;

		template<typename T>
		bool RegisterSection(const std::string& name, T& preferences);

		// Register a category before adding parameters to it. Optional callbacks
		// are useful when a category contains values which must be initialized or
		// validated together.
		bool RegisterCategory(
			const std::string& name,
			std::function<void()> set_defaults = {},
			std::function<void()> check_valid = {});

		// A parameter is bound to the supplied variable. Its value is read from
		// and written to the preferences file automatically.
		template<typename T>
		bool RegisterParameter(const std::string& category, const std::string& name, T& parameter);

		// Registers a parameter and supplies the value SetDefaults() will assign.
		template<typename T>
		bool RegisterParameter(
			const std::string& category,
			const std::string& name,
			T& parameter,
			const T& default_value);

		// Replaces the default registered for a parameter. Engine parameters can
		// be changed through this method as well as application-defined ones.
		template<typename T>
		bool SetDefault(const std::string& category, const std::string& name, const T& value);

		bool Load();
		bool Save() const;

		void SetDefaults();
		void CheckValid();

		VideoPreferences video;

	private:
		struct Parameter
		{
			std::function<void(nlohmann::json&)> to_json;
			std::function<void(const nlohmann::json&)> from_json;
			std::type_index type{ typeid(void) };
			nlohmann::json default_value;
			bool has_default = false;
		};

		struct Section
		{
			std::function<void()> set_defaults;
			std::function<void()> check_valid;

			std::function<void(nlohmann::json&)> to_json;
			std::function<void(const nlohmann::json&)> from_json;
			std::unordered_map<std::string, Parameter> parameters;
			bool uses_whole_object_serialization = false;
		};

		bool RegisterParameter(const std::string& category, const std::string& name, Parameter parameter);

		std::unordered_map<std::string, Section> sections;
	};

	template<typename T>
	bool Preferences::RegisterSection(const std::string& name, T& preferences)
	{
		if (name.empty())
			return false;

		if (sections.contains(name))
			return false;

		Section section;
		section.set_defaults = [&preferences]() { preferences.SetDefaults(); };
		section.check_valid = [&preferences]() { preferences.CheckValid(); };
		section.to_json = [&preferences](nlohmann::json& j) { j = preferences; };
		section.from_json = [&preferences](const nlohmann::json& j) { j.get_to(preferences); };
		section.uses_whole_object_serialization = true;

		sections.emplace(name, std::move(section));

		return true;
	}

	template<typename T>
	bool Preferences::RegisterParameter(const std::string& category, const std::string& name, T& parameter)
	{
		using ValueType = std::remove_cvref_t<T>;

		Parameter registered_parameter;
		registered_parameter.to_json = [parameter_ptr = &parameter](nlohmann::json& j) { j = *parameter_ptr; };
		registered_parameter.from_json = [parameter_ptr = &parameter](const nlohmann::json& j) { j.get_to(*parameter_ptr); };
		registered_parameter.type = std::type_index(typeid(ValueType));

		return RegisterParameter(category, name, std::move(registered_parameter));
	}

	template<typename T>
	bool Preferences::RegisterParameter(
		const std::string& category,
		const std::string& name,
		T& parameter,
		const T& default_value)
	{
		using ValueType = std::remove_cvref_t<T>;

		Parameter registered_parameter;
		registered_parameter.to_json = [parameter_ptr = &parameter](nlohmann::json& j) { j = *parameter_ptr; };
		registered_parameter.from_json = [parameter_ptr = &parameter](const nlohmann::json& j) { j.get_to(*parameter_ptr); };
		registered_parameter.type = std::type_index(typeid(ValueType));
		registered_parameter.default_value = default_value;
		registered_parameter.has_default = true;

		return RegisterParameter(category, name, std::move(registered_parameter));
	}

	template<typename T>
	bool Preferences::SetDefault(const std::string& category, const std::string& name, const T& value)
	{
		using ValueType = std::remove_cvref_t<T>;

		auto category_it = sections.find(category);
		if (category_it == sections.end())
			return false;

		auto parameter_it = category_it->second.parameters.find(name);
		if (parameter_it == category_it->second.parameters.end())
			return false;

		Parameter& parameter = parameter_it->second;
		if (parameter.type != std::type_index(typeid(ValueType)))
			return false;

		parameter.default_value = value;
		parameter.has_default = true;

		return true;
	}
}

#endif
