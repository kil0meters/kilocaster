#include <stdlib.h>

#include "vector.h"

Vector v_add(Vector v1, Vector v2) {
    Vector vector = { v1.x + v2.x, v1.y + v2.y };
    return vector;
}

Vector v_sub(Vector v1, Vector v2) {
    Vector vector = { v1.x - v2.x, v1.y - v2.y };
    return vector;
}
