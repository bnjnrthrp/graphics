#ifndef LINE_H
#define LINE_H
#include "Point.h"

typedef struct Line
{
    int zBuffer;
    Point a;
    Point b;
} Line;

void line_set2D(Line *l, double x0, double y0, double x1, double y1);
void line_set(Line *l, Point ta, Point tb);
void line_zBuffer(Line *l, int flag);
void line_normalize(Line *l);
void line_copy(Line *to, Line *from);
void line_draw(Line *l, Image *src, Color c);
#endif // LINE_H
