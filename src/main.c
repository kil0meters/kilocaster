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

typedef struct Player {
    Vector location;
    Vector dir;
} Player;

// use some sort of linear interpolation algorithm here
void player_move(Player *player, double amount) {
    Vector offset = v_mult_int(v_sub(player->location, player->dir),
                               amount / PLAYER_LENGTH) ;
    player->location = v_add(player->location, offset);
    player->dir = v_add(player->dir, offset);
}

void player_rotate(Player *player, double angle) {
    double px = player->location.x;
    double py = player->location.y;
    double cx = player->dir.x;
    double cy = player->dir.y;

    cx -= px;
    cy -= py;

    double _cx = (cx * cos(angle)) - (cy * sin(angle));
    double _cy = cx * sin(angle) + cy * cos(angle);

    player->dir.x = _cx + px;
    player->dir.y = _cy + py;
}

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


    Player player = { { WIN_WIDTH / 2, WIN_HEIGHT / 2 },
                      { WIN_WIDTH / 2, WIN_HEIGHT / 2 + PLAYER_LENGTH } };

    Level level = default_stage();

    while(running) {
        // move player
        if (keys[SDL_SCANCODE_W]) {
            player_move(&player, -1);
        }
        if (keys[SDL_SCANCODE_S]) {
            player_move(&player, 1);
        }
        if (keys[SDL_SCANCODE_A]) {
            player_rotate(&player, -0.05);
        }
        if (keys[SDL_SCANCODE_D]) {
            player_rotate(&player, 0.05);
        }

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
        SDL_RenderDrawLine(renderer, player.location.x, player.location.y, player.dir.x, player.dir.y);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
