#ifndef POLYGON_H
#define POLYGON_H
#include "Point.h"
#include "Vector.h"
#include "DrawState.h"
#include "Lighting.h"

typedef struct Polygon
{
    int oneSided;
    int nVertex;
    Point *vertex;
    Color *color;
    Vector *normal;
    int zBuffer;
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
void polygon_setColors(Polygon *p, int numV, Color *clist);
void polygon_setNormals(Polygon *p, int numV, Vector *nlist);
void polygon_setAll(Polygon *p, int numV, Point *vlist, Color *clist, Vector *nlist, int zBuffer, int oneSided);
void polygon_zBuffer(Polygon *p, int flag);
void polygon_copy(Polygon *to, Polygon *from);
void polygon_print(Polygon *p, FILE *fp);
void polygon_normalize(Polygon *p);
void polygon_draw(Polygon *p, Image *src, Color c);
void polygon_drawFill(Polygon *p, Image *src, Color c);
void polygon_drawFillB(Polygon *p, Image *src, Color c);
void polygon_drawShade(Polygon *p, Image *src, DrawState *ds, Lighting *l);
void polygon_shade(Polygon *p, DrawState *ds, Lighting *l);

#endif // POLYGON_H
