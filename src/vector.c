#include <stdlib.h>

#include "vector.h"

Vector v(double x, double y) {
    Vector vector = { x, y };
    return vector;
}

Vector v_add(Vector v1, Vector v2) {
    Vector vector = { v1.x + v2.x, v1.y + v2.y };
    return vector;
}

Vector v_sub(Vector v1, Vector v2) {
    Vector vector = { v1.x - v2.x, v1.y - v2.y };
    return vector;
}

Vector v_abs(Vector v1) {
    Vector vector = { abs(v1.x), abs(v1.y) };
    return vector;
}

Vector v_mult_int(Vector v1, double val) {
    Vector vector = { v1.x * val, v1.y * val };
    return vector;
}

