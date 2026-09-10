#ifndef UNTITLED_PREFERENCES_H
#define UNTITLED_PREFERENCES_H

namespace Untitled
{
	struct DisplayPreferences
	{
		int displayWidth = 1280;
		int displayHeight = 720;
		int displayMode = 0; // 0 = windowed, 1 = borderless, 2 = fullscreen
		int maxFramerate = 0; // 0 = unlimited, otherwise specify the max framerate
		int vsync = 0; // 0 = off, 1 = on, 2 = adaptive
	};

	class Preferences
	{
	public:
		bool load();
		bool save() const;
		void setDefaults();

		DisplayPreferences displayPreferences;
	};
}

#endif
