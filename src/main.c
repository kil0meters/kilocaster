#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdint.h>

#include <SDL2/SDL.h>

#include "level.h"
#include "vector.h"

#define WIN_WIDTH 800
#define WIN_HEIGHT 600
#define PLAYER_LENGTH 30

int main(int argc, char *argv[]) {
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
    SDL_Event event;

    SDL_Rect player = { (WIN_WIDTH / 2) - 2, (WIN_HEIGHT / 2) - 2,
                        2, 2};

    Level level = default_stage();

    while(running) {
        // move player
        level_handle_input(&level, keys);

        // printf("%.0f,%.0f\n", player.location.x, player.location.y);

        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT)
                running = false;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
        SDL_RenderClear(renderer);

        // render level
        level_render(renderer, level);

        // draw player
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderFillRect(renderer, &player);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
