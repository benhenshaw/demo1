#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#define assert SDL_assert

typedef uint32_t u32;

int main()
{
    int error = SDL_Init(SDL_INIT_EVERYTHING);
    assert(error == 0);

    SDL_Window * window = SDL_CreateWindow("",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        640, 480, 0);
    assert(window);

    u32 * pixels = SDL_GetWindowSurface(window)->pixels;
    assert(pixels);

    while (true)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                exit(0);
            }
        }

        SDL_UpdateWindowSurface(window);
    }
}
