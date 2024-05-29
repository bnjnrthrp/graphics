#ifndef CIRCLE_H

#define CIRCLE_H
#include "Point.h"
typedef struct Circle
{
    double r; // radius
    Point c;  // center
} Circle;

void circle_set(Circle *c, Point tc, double tr);
void circle_draw_helper(Image *src, int y, int cy, int x, int cx, Color p, int *octants, int size, int fill);
void circle_draw(Circle *c, Image *src, Color p);
void circle_draw_partial(Circle *c, Image *src, Color p, int *octants, int size);
void circle_bresenham_algo(Circle *c, Image *src, Color p, int *octants, int size, int fill);
void circle_drawFill(Circle *c, Image *src, Color p);

#endif // CIRCLE_H
