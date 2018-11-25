#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include <SDL2/SDL.h>

#include "level.h"
#include "vector.h"
#include "screen.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "error: Expected level file as arugment\n");
        return 1;
    }

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("raycaster",
                           SDL_WINDOWPOS_UNDEFINED,
                           SDL_WINDOWPOS_UNDEFINED,
                           WIN_WIDTH, WIN_HEIGHT,
                           SDL_WINDOW_OPENGL);
    if (window == NULL) return 1;

    // vsynced renderer for window
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
                              SDL_RENDERER_ACCELERATED |
                              SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) return 1;

    const uint8_t *keys = SDL_GetKeyboardState(NULL);

    bool running = true;
    bool is_2d = true;
    SDL_Event event;

    Level level = load_stage_from_file(argv[1]);

    while(running) {
        // move player
        level_handle_input(&level, keys);

        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT)
                running = false;

            else if(event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_f && is_2d == true) {
                    is_2d = false;
                }
                else if (event.key.keysym.sym == SDLK_f && is_2d == false) {
                    is_2d = true;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
        SDL_RenderClear(renderer);

        // render level
        if (is_2d) {
            level_render_2d(renderer, level);
        }
        else {
            level_render_3d(renderer, level);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
