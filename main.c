#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <SDL2/SDL.h>
#define assert SDL_assert

typedef uint32_t u32;
typedef uint64_t u64;
typedef float f32;

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

    u64 target_frame_rate = 60;
    u64 ticks_per_second = SDL_GetPerformanceFrequency();
    u64 ticks_per_frame = ticks_per_second / target_frame_rate;
    u64 ticks_per_millisecond = ticks_per_second / 1000;

    while (true)
    {
        u64 time_stamp = SDL_GetPerformanceCounter();

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                exit(0);
            }
        }

        memset(pixels, 0, 640 * 480 * 4);

        for (int y = 0; y < 480; ++y)
        {
            int x = (time_stamp / 5000000) % 640;
            pixels[x + y * 640] = ~0;
        }

        SDL_UpdateWindowSurface(window);

        if (SDL_GetPerformanceCounter() - time_stamp < ticks_per_frame - ticks_per_millisecond)
        {
            struct timespec sleep_time = {
                .tv_nsec = (ticks_per_frame - (ticks_per_millisecond * 2))
                    - (SDL_GetPerformanceCounter() - time_stamp)
             };
            nanosleep(&sleep_time, NULL);
            while (SDL_GetPerformanceCounter() - time_stamp < ticks_per_frame);
        }

        printf("%f\n", (SDL_GetPerformanceCounter() - time_stamp) / (double)ticks_per_second);
    }
}
