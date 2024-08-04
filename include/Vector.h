#ifndef VECTOR_H

#define VECTOR_H

#include "Point.h"
typedef Point Vector;

void vector_set(Vector *v, double x, double y, double z);
void vector_print(Vector *v, FILE *fp);
void vector_copy(Vector *dest, Vector *src);
double vector_length(Vector *v);
void vector_normalize(Vector *v);
double vector_dot(Vector *a, Vector *b);
void vector_cross(Vector *a, Vector *b, Vector *c);
void vector_calculateNormal(Vector *N, Point *a, Point *b, Point *c);
void vector_subtract(Vector *a, Vector *b, Vector *c);

#endif // VECTOR_H
