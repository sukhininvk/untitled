#ifndef UNTITLED_PREFERENCES_H
#define UNTITLED_PREFERENCES_H

namespace Untitled
{
	struct DisplayPreferences
	{
		int width = 2160;
		int height = 1440;
		int mode = 0; // 0 = windowed, 1 = borderless, 2 = fullscreen
		int max_fps = 0; // 0 = unlimited, otherwise specify the max framerate
		int vsync = 1; // 0 = off, 1 = on, 2 = adaptive
	};

	class Preferences
	{
	public:
		bool Load();
		bool Save() const;
		void SetDefaults();
		void CheckValid();

		DisplayPreferences display;
	};
}

#endif
