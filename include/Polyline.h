#ifndef POLYLINE_H

#define POLYLINE_H
#include "Point.h"

typedef struct Polyine
{
    int zBuffer;   // default true (1), determine if use zBuffer
    int numVertex; // Num of vertices
    Point *vertex; // Vertex info
} Polyline;

// Constructors / Setters
Polyline *polyline_create();
Polyline *polyline_createp(int numV, Point *vlist);
void polyline_free(Polyline *p);
void polyline_init(Polyline *p);
void polyline_set(Polyline *p, int numV, Point *vlist);
void polyline_clear(Polyline *p);

// utility
void polyline_zBuffer(Polyline *p, int flag);
void polyline_copy(Polyline *to, Polyline *from);
void polyline_print(Polyline *p, FILE *fp);
void polyline_normalize(Polyline *p);
void polyline_draw(Polyline *p, Image *src, Color c);

#endif // POLYLINE_H
