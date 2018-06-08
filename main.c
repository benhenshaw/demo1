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

    SDL_AudioSpec audio_spec_request = {
        .format   = AUDIO_F32,
        .freq     = 48000,
        .channels = 2,
        .samples  = 512,
    };

    int audio_device = SDL_OpenAudioDevice(NULL, false,
        &audio_spec_request, NULL, 0);
    assert(audio_device);

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
