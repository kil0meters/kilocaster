#ifndef VECTOR_H_
#define VECTOR_H_

typedef struct Vector {
    double x;
    double y;
} Vector;

Vector v_add(Vector v1, Vector v2);
Vector v_sub(Vector v1, Vector v2);
Vector v_square(Vector v1);

#endif
