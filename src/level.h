#ifndef LEVEL_H_
#define LEVEL_H_

#include <stdint.h>

#include <SDL2/SDL.h>

#include "vector.h"

typedef struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} Color;

typedef struct Wall {
    Vector start;
    Vector end;
    Color color;
} Wall;

typedef struct Level {
    Wall *walls;
    int num_walls;
    double rotation;
    Vector offset;
} Level;

Level default_stage();
void level_render(SDL_Renderer *renderer, Level level);
void level_handle_input(Level *level, const uint8_t *keys);


#endif
