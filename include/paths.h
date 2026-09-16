#ifndef UNTITLED_PATHS_H
#define UNTITLED_PATHS_H

#include "appinfo.h"

#include <SDL3/SDL_filesystem.h>

namespace Untitled
{
	const char* APPDATA_PATH = SDL_GetPrefPath(ORG_NAME, APP_NAME);
}

#endif
