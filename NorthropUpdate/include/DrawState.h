/**
 * A struct indicating the draw state, dealing with colors and shading.
 */
#ifndef DRAWSTATE_H

#define DRAWSTATE_H

#include "Color.h"
#include "Point.h"

typedef enum ShadeMethod
{
    ShadeFrame,
    ShadeConstant,
    ShadeDepth,
    ShadeFlat,
    ShadeGouraud,
    ShadePhong
} ShadeMethod;

typedef struct DrawState
{
    Color color;
    Color flatColor;
    Color body;
    Color surface;
    float surfaceCoeff;
    ShadeMethod shade;
    int zBufferFlag;
    Point viewer;
} DrawState;

DrawState *drawstate_create(void);
void drawstate_setColor(DrawState *s, Color c);
void drawstate_setBody(DrawState *s, Color c);
void drawstate_setSurface(DrawState *s, Color c);
void drawstate_setSurfaceCoeff(DrawState *s, float f);
void drawstate_copy(DrawState *to, DrawState *from);

#endif // DRAWSTATE_H;
