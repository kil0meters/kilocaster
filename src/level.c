#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

#include "level.h"
#include "vector.h"
#include "screen.h"

typedef struct Ray {
    double dir;
    double dist;
    Color color;
} Ray;

Level load_stage_from_file(const char *filename) {
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        fprintf(stderr, "error: Could not open file \"%s\"\n", filename);
        exit(1);
    }


    static Wall walls[50];

    double x1,y1,x2,y2;
    uint8_t r,g,b;

    int i = 0;
    char buffer[256];
    while (fgets(buffer, 256, file)) {

        int res = sscanf(buffer, "%lf,%lf--%lf,%lf:%hhd,%hhd,%hhd",
                         &x1, &y1, &x2, &y2, &r, &g, &b);

        if (res != 7) {
            fprintf(stderr, "error: Invalid file contents\n");
            exit(1);
        }

        Wall wall = { {x1, y1}, {x2, y2}, {r, g, b} };
        walls[i] = wall;
        i++;
    }

    Level level = {
        .walls = walls,
        .num_walls = i,
        .rotation = 0.0,
        .offset = { 200, 100 },
    };

    return level;
}

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
        .offset = { 400.0, 300.0 },
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

static bool line_collision(Vector a1,
                           Vector a2,
                           Vector b1,
                           Vector b2,
                           Vector *intersection_point) {
    float s1_x, s1_y, s2_x, s2_y;
    s1_x = a2.x - a1.x;     s1_y = a2.y - a1.y;
    s2_x = b2.x - b1.x;     s2_y = b2.y - b1.y;

    float s, t;
    s = (-s1_y * (a1.x - b1.x) + s1_x * (a1.y - b1.y)) / (-s2_x * s1_y + s1_x * s2_y);
    t = ( s2_x * (a1.y - b1.y) - s2_y * (a1.x - b1.x)) / (-s2_x * s1_y + s1_x * s2_y);

    if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
    {
        intersection_point->x = a1.x + (t * s1_x);
        intersection_point->y = a1.y + (t * s1_y);
        return true;
    }

    return false;
}

// tests if a ray collides with a given wall
static Ray test_ray(Wall wall, double angle) {
    Vector center = { WIN_WIDTH / 2, WIN_HEIGHT / 2 };

    Vector max_length = { center.x + 800 * cos(angle), center.y + 800 * sin(angle) };

    Ray ray = {angle, 1.0, {0,0,0}};

    Vector intersection_point;
    if (line_collision(wall.start, wall.end,
                       center, max_length,
                       &intersection_point)) {
        Vector tmp = v_square(v_sub(intersection_point, center));
        double dist = sqrt(tmp.x + tmp.y);

        ray.dist = dist;
        ray.color = wall.color;
    }
    return ray;
}

void level_render_3d(SDL_Renderer *renderer, Level level) {
    Wall *walls = level.walls;
    for (int x = 0; x < WIN_WIDTH; x++) {
        double angle = (((double) x / 763.9433)) - 2.094395;

        for (int j = 0; j < level.num_walls; j++) {
            Wall adjusted_wall = {
                calculate_rotation(v_add(level.offset, walls[j].start), level.rotation),
                calculate_rotation(v_add(level.offset, walls[j].end), level.rotation),
                walls[j].color
            };

            Ray ray = test_ray(adjusted_wall, angle);
            if (ray.color.r != 0 ||
                ray.color.g != 0 ||
                ray.color.b != 0) {

                double height = (1000.0 - ray.dist) / 2;
                Color color = ray.color;

                if (WIN_HEIGHT - height < height) {
                    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 0xFF);
                    SDL_RenderDrawLine(renderer, x, WIN_HEIGHT - height, x, height);
                }
                break;
            }
        }
    }
}

void level_render_2d(SDL_Renderer *renderer, Level level) {
    Wall *walls = level.walls;

    // draw player
    SDL_Rect player = { (WIN_WIDTH / 2) - 2, (WIN_HEIGHT / 2) - 2, 2, 2 };
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderFillRect(renderer, &player);

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
