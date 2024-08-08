#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "polygon.h"

double ray_cast(Lighting *l, int index, Point *p);
int ray_polygonIntersect(Vector *V, Point *p, Light *l);
int ray_triangleIntersect(Vector *V, Point *p, Light *l);
Color ray_calculateShadows(Vector *V, Point *pt, Lighting *l);

#endif // RAYTRACER_H
