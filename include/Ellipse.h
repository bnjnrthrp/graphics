#ifndef ELLIPSE_H
#define ELLIPSE_H
#include "Point.h"

typedef struct Ellipse
{
    double ra; // major axis radius
    double rb; // minor axis radius
    Point c;   // center
    double a;  // angle of major axis to X-axis
} Ellipse;

void ellipse_set(Ellipse *e, Point tc, double ta, double tb);
void ellipse_draw(Ellipse *e, Image *src, Color p);
void ellipse_drawFill(Ellipse *e, Image *src, Color p);
void ellipse_draw_partial(Ellipse *e, Image *src, Color p, int *quadrants, int size);
void ellipse_draw_helper(Image *src, int y, int cy, int x, int cx, Color p, int *quadrants, int size);
#endif // ELLIPSE_H
