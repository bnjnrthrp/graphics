#ifndef POLYGON_H
#define POLYGON_H
#include "Point.h"

typedef struct Polygon
{
    int oneSided;
    int nVertex;
    Point *vertex;
} Polygon;

// Constructors
Polygon *polygon_create(void);
Polygon *polygon_createp(int numV, Point *vlist);
void polygon_free(Polygon *p);

// Initialize, set, free
void polygon_init(Polygon *p);
void polygon_set(Polygon *p, int numV, Point *vlist);
void polygon_clear(Polygon *p);

// setters/getters
void polygon_setSided(Polygon *p, int oneSided);
void polygon_copy(Polygon *to, Polygon *from);
void polygon_print(Polygon *p, FILE *fp);
void polygon_normalize(Polygon *p);
void polygon_draw(Polygon *p, Image *src, Color c);
void polygon_drawFill(Polygon *p, Image *src, Color c);

#endif // POLYGON_H
