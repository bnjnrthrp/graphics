#ifndef POINT_H
#define POINT_H

#include <stdio.h>
#include "Image.h"
#include "Color.h"

/**
 * Point represents a point in 3D space, consisting of x, y, and z coordinates and
 * and h value for homogeneous coordinates. z defaults to 0 and h defaults to 1.0
 */
typedef struct Point
{
    double val[4]; // x, y, z, h
} Point;

void point_set2D(Point *p, double x, double y);
void point_set3D(Point *p, double x, double y, double z);
void point_set(Point *p, double x, double y, double z, double h);
void point_normalize(Point *p);
void point_copy(Point *to, Point *from);
void point_draw(Point *p, Image *src, Color c);
void point_drawf(Point *p, Image *src, FPixel c);
void point_print(Point *p, FILE *fp);
#endif // POINT_H
