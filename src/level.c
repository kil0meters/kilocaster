#include <stdlib.h>
#include <stdint.h>

#include <SDL2/SDL.h>

#include "level.h"
#include "vector.h"

static Wall wall_new(Vector start, Vector end, Color color) {
    Wall wall = {
        start,
        end,
        color
    };

    return wall;
}

Level default_stage() {
    // c is weird
    // ^ informative comment
    static Wall walls[] = {
        { {10.0, 80.0}, {30.0, 30.0}, {0xFF, 0, 0} },
        { {30.0, 30.0}, {80.0, 100.0}, {0xFF, 0xFF, 0xFF} },
    };

    Level level = {
        .walls = walls,
        .num_walls = 2,
        .rotation = 0.0,
        .offset = { 200.0, 100.0 },
    };

    return level;
}


void level_render(SDL_Renderer *renderer, Level level) {
    Wall *walls = level.walls;

    for (int i = 0; i < level.num_walls; i++) {
        Color color = walls[i].color;
        Vector start = v_add(level.offset, walls[i].start);
        Vector end = v_add(level.offset, walls[i].end);

        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 0xFF);
        SDL_RenderDrawLine(renderer, start.x, start.y, end.x, end.y);
    }
}
