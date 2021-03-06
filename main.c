#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <SDL2/SDL.h>
#define assert SDL_assert

typedef uint8_t  u8;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int64_t  s64;
typedef float f32;

int main()
{
    int error = SDL_Init(SDL_INIT_EVERYTHING);
    assert(error == 0);

    SDL_Window * window = SDL_CreateWindow("",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        640, 480, 0);
    assert(window);

    SDL_Surface * screen_surface = SDL_GetWindowSurface(window);
    assert(screen_surface);
    u32 * pixels = screen_surface->pixels;

    SDL_AudioSpec audio_spec_request = {
        .format   = AUDIO_F32,
        .freq     = 48000,
        .channels = 2,
        .samples  = 512,
    };
    int audio_device = SDL_OpenAudioDevice(NULL, false, &audio_spec_request, NULL, 0);
    assert(audio_device);

    u64 target_frame_rate = 60;

    int samples_per_frame = (48000 / target_frame_rate) * 2;
    f32 * audio_buffer = malloc(samples_per_frame * sizeof(f32));

    for (int i = 0; i < samples_per_frame; i += 2)
    {
        audio_buffer[i+0] = sinf((f32)i/samples_per_frame * M_PI * 8.0f) * 0.1f;
        audio_buffer[i+1] = sinf((f32)i/samples_per_frame * M_PI * 8.0f) * 0.1f;
    }

    SDL_PauseAudioDevice(audio_device, false);

    u64 ticks_per_second = SDL_GetPerformanceFrequency();
    assert(ticks_per_second == 1000000000);
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
            else if (event.type == SDL_MOUSEMOTION)
            {
                f32 mx = event.motion.x;
                mx /= 640;
                mx *= 2.0f;
                mx -= 1.0f;
                mx *= 0.1f;
                for (int i = 0; i < samples_per_frame; i += 2)
                {
                    audio_buffer[i+0] = sinf((f32)i/samples_per_frame * M_PI * 8.0f) * (0.1f - mx);
                    audio_buffer[i+1] = sinf((f32)i/samples_per_frame * M_PI * 8.0f) * (0.1f + mx);
                }
            }
        }

        SDL_QueueAudio(audio_device, audio_buffer, samples_per_frame * sizeof(f32));

        memset(pixels, 0, 640 * 480 * 4);

        for (int y = 0; y < 480; ++y)
        {
            int x = 0;
            SDL_GetMouseState(&x, NULL);
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
    }
}
