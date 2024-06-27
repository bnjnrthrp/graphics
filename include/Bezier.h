#ifndef BEZIER_H
#define BEZIER_H

#include <stdlib.h>
#include "Point.h"
#include "Module.h"

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
void bezierSurface_init(BezierSurface *b);
void bezierCurve_set(BezierCurve *b, Point *vlist);
void bezierSurface_set(BezierSurface *b, Point *vlist);
void bezierCurve_zBuffer(BezierCurve *p, int flag);
void bezierSurface_zBuffer(BezierCurve *p, int flag);
void bezierCurve_draw(BezierCurve *b, Image *src, Color c);

// Module functions
void module_bezierCurve(Module *m, BezierCurve *b, int divisions);
void module_bezierSurface(Module *m, BezierSurface *b, int divisions, int solid);
#endif // BEZIER_H
