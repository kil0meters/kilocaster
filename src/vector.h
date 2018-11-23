#ifndef VECTOR_H_
#define VECTOR_H_

typedef struct Vector {
    double x;
    double y;
} Vector;

Vector v(double x, double y);
Vector v_add(Vector v1, Vector v2);
Vector v_sub(Vector v1, Vector v2);
Vector v_abs(Vector v1);
Vector v_mult_int(Vector v1, double val);

#endif
