#include <stdlib.h>

#include "vector.h"

static double max(double a, double b) {
    return a > b ? a : b;
}

Vector v_add(Vector v1, Vector v2) {
    Vector vector = { v1.x + v2.x, v1.y + v2.y };
    return vector;
}

Vector v_sub(Vector v1, Vector v2) {
    Vector vector = { v1.x - v2.x, v1.y - v2.y };
    return vector;
}

Vector v_square(Vector v1) {
    Vector vector = { v1.x * v1.x, v1.y * v1.y };
    return vector;
}

Vector v_max(Vector v1, Vector v2) {
    Vector vector = { max(v1.x, v2.x), max(v1.y, v2.y)};
    return vector;
}
