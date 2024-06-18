/**
 * A file for operations within the draw state, dealing with colors and shading.
 * @author Benji Northrop
 */

#include <stdlib.h>
#include "DrawState.h"

/**
 * Creates a new DrawState structure and initializes its fields.
 *
 * @return A pointer to the newly created DrawState structure.
 */
DrawState *drawstate_create(void)
{
    DrawState *ds = (DrawState *)malloc(sizeof(DrawState));
    Color White;
    Point p;

    color_set(&White, 1, 1, 1);
    point_set(&p, 0, 0, 0, 1);
    color_copy(&(ds->body), &White);
    color_copy(&(ds->color), &White);
    color_copy(&(ds->flatColor), &White);
    color_copy(&(ds->surface), &White);
    ds->surfaceCoeff = 0.0;
    ds->shade = ShadeFrame;
    ds->zBufferFlag = 0;
    ds->viewer = p;

    return ds;
}

/**
 * Sets the color field of the DrawState structure.
 *
 * @param s: Pointer to the DrawState structure.
 * @param c: The Color value to set.
 */
void drawstate_setColor(DrawState *s, Color c)
{
    if (!s)
    {
        fprintf(stderr, "Invalid pointer to drawstate_setColor\n");
        exit(-1);
    }
    color_copy(&(s->color), &c);
}

/**
 * Sets the body field of the DrawState structure.
 *
 * @param s: Pointer to the DrawState structure.
 * @param c: The Color value to set.
 */
void drawstate_setBody(DrawState *s, Color c)
{
    if (!s)
    {
        fprintf(stderr, "Invalid pointer to drawstate_setBody\n");
        exit(-1);
    }
    color_copy(&(s->body), &c);
}

/**
 * Sets the surface field of the DrawState structure.
 *
 * @param s: Pointer to the DrawState structure.
 * @param c: The Color value to set.
 */
void drawstate_setSurface(DrawState *s, Color c)
{
    if (!s)
    {
        fprintf(stderr, "Invalid pointer to drawstate_setSurface\n");
        exit(-1);
    }
    color_copy(&(s->surface), &c);
}

/**
 * Sets the surfaceCoeff field of the DrawState structure.
 *
 * @param s: Pointer to the DrawState structure.
 * @param f: The float value to set.
 */
void drawstate_setSurfaceCoeff(DrawState *s, float f)
{
    if (!s)
    {
        fprintf(stderr, "Invalid pointer to drawstate_setSurfaceCoeff\n");
        exit(-1);
    }
    s->surfaceCoeff = f;
}

/**
 * Copies the data from one DrawState structure to another.
 *
 * @param to: Pointer to the destination DrawState structure.
 * @param from: Pointer to the source DrawState structure.
 */
void drawstate_copy(DrawState *to, DrawState *from)
{
    if (!to || !from)
    {
        fprintf(stderr, "Invalid pointer to drawstate_copy\n");
        exit(-1);
    }
    color_copy(&(to->body), &(from->body));
    color_copy(&(to->color), &(from->color));
    color_copy(&(to->flatColor), &(from->flatColor));
    color_copy(&(to->surface), &(from->surface));
    to->surfaceCoeff = from->surfaceCoeff;
    to->shade = from->shade;
    to->zBufferFlag = from->zBufferFlag;
    point_copy(&(to->viewer), &(from->viewer));
}
