#include <stdlib.h>
#include <stdint.h>

#include <SDL2/SDL.h>

#include "level.h"
#include "vector.h"

/*
typedef struct Ray {
    double dir;
    double dist;
    Color color;
} Ray;
*/

Level default_stage() {
    // c is weird
    // ^ informative comment
    static Wall walls[] = {
        { {10.0, 80.0}, {30.0, 30.0}, {0xFF, 0, 0} },
        { {30.0, 30.0}, {120.0, -50.0}, {0xFF, 0xFF, 0xFF} },
        { {120.0, -50.0}, {300.0, -20.0}, {0, 0xFF, 0} },
        { {500.0, 450.0}, {300.0, -20.0}, {0xFF, 0xFF, 0}},
        { {10.0, 80.0}, {500.0, 450.0}, {0, 0, 0xFF} }
    };

    Level level = {
        .walls = walls,
        .num_walls = 5,
        .rotation = 0.0,
        .offset = { 200.0, 100.0 },
    };

    return level;
}

static Vector calculate_rotation(Vector v, double rotation) {
    double vx = v.x - 400;
    double vy = v.y - 300;

    double cx = (vx * cos(rotation)) - (vy * sin(rotation));
    double cy = vx * sin(rotation) + vy * cos(rotation);

    Vector vector = { cx + 400, cy + 300 };
    return vector;
}

// Ray *cast_rays(Level *level) {}

void level_render(SDL_Renderer *renderer, Level level) {
    Wall *walls = level.walls;

    for (int i = 0; i < level.num_walls; i++) {
        Color color = walls[i].color;
        Vector start = calculate_rotation(v_add(level.offset, walls[i].start), level.rotation);
        Vector end = calculate_rotation(v_add(level.offset, walls[i].end), level.rotation);

        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 0xFF);
        SDL_RenderDrawLine(renderer, start.x, start.y, end.x, end.y);
    }
}

static void player_move(Level *level, double amount) {
    double theta = level->rotation;
    level->offset.x -= amount * sin(-theta);
    level->offset.y += amount * cos(-theta);
}

void level_handle_input(Level *level, const uint8_t *keys) {
    if (keys[SDL_SCANCODE_W]) {
        player_move(level, 1);
    }
    if (keys[SDL_SCANCODE_S]) {
        player_move(level, -1);
    }
    if (keys[SDL_SCANCODE_A]) {
        level->rotation += 0.01;
    }
    if (keys[SDL_SCANCODE_D]) {
        level->rotation -= 0.01;
    }
}
