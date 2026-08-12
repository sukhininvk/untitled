#include "application.h"

#include <SDL3/SDL_main.h>

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    Untitled::Application app;
    return app.Run();
}