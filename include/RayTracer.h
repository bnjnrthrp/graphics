#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "Polygon.h"

typedef struct RayTracer
{
    Polygon *db;
    int size;
    int max;
} RayTracer;

RayTracer *rayTracer_create(void);
void rayTracer_init(RayTracer *rt, int size);
void rayTracer_add(RayTracer *rt, Polygon *p);
void rayTracer_expand(RayTracer *rt);
void rayTracer_dbFree(RayTracer *rt);
Polygon *rayTracer_closestPolygon(Point *src, RayTracer *rt, Vector *Vij, Point *x);
int ray_triIntersect(Point *src, Polygon *p, Vector *Vij, Point *x);
float ray_calculateT(Point *src, Point *dest, Vector *V);
#endif // RAYTRACER_H
