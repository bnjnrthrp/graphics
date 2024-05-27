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
#endif // ELLIPSE_H
