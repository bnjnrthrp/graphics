#ifndef BEZIER_H
#define BEZIER_H

#include <stdlib.h>
#include "Point.h"

typedef struct BezierCurve
{
    Point cp[4]; // 4 control points
    int zbuffer; // zbuffer, default to 1
} BezierCurve;

typedef struct BezierSurface
{
    Point cp[16]; // 16 control points
    int zbuffer;  // zbuffer, defaults to 1
} BezierSurface;

// Bezier functions
void bezierCurve_init(BezierCurve *b);
void bezierCurve_set(BezierCurve *b, Point *vlist);
void bezierCurve_copy(BezierCurve *to, BezierCurve *from);
void bezierCurve_zBuffer(BezierCurve *p, int flag);
void bezierCurve_draw(BezierCurve *b, Image *src, Color c);
void bezierSurface_init(BezierSurface *b);
void bezierSurface_set(BezierSurface *b, Point *vlist);
void bezierSurface_zBuffer(BezierSurface *p, int flag);

#endif // BEZIER_H
