#include "game.h"

#include <SDL3/SDL_main.h>

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    Game::Application app;
    return app.Run();
}