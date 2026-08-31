#include "SDL2/SDL.h"
#include <cstdio>

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    printf("Step 1: calling SDL_Init...\n");
    fflush(stdout);

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("SDL_Init FAILED: %s\n", SDL_GetError());
        fflush(stdout);
        return 1;
    }

    printf("Step 2: SDL_Init succeeded, creating window...\n");
    fflush(stdout);

    SDL_Window* window = SDL_CreateWindow(
        "SDL2 Minimal Test",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        400, 300,
        SDL_WINDOW_SHOWN
    );

    if (!window)
    {
        printf("SDL_CreateWindow FAILED: %s\n", SDL_GetError());
        fflush(stdout);
        SDL_Quit();
        return 1;
    }

    printf("Step 3: window created. It should be visible now.\n");
    printf("Step 4: waiting 3 seconds, then closing...\n");
    fflush(stdout);

    SDL_Delay(3000);

    printf("Step 5: destroying window and quitting cleanly.\n");
    fflush(stdout);

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}