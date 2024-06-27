/**
 * This file represents the Bezier curve and Bezier surfaces datatypes and their functions
 * @author Benji Northrop
 */

#include <stdlib.h>
#include "Bezier.h"

/**
 * Initializes a Bezier curve by setting the zbuffer flag to 1 and the curve points to the X-axis between 0 and 1.
 *
 * @param b The BezierCurve to initialize.
 */
void bezierCurve_init(BezierCurve *b)
{
    if (!b)
    {
        fprintf(stderr, "Null pointer provided to bezierCurve_init\n");
        exit(-1);
    }

    point_set(&(b->cp[0]), 0.0, 0.0, 0.0, 1.0);
    point_set(&(b->cp[1]), .3, 0.0, 0.0, 1.0);
    point_set(&(b->cp[2]), 0.6, 0.0, 0.0, 1.0);
    point_set(&(b->cp[3]), 1.0, 0.0, 0.0, 1.0);
    b->zbuffer = 1;
}

/**
 * Initializes a Bezier surface by setting the zbuffer flag to 1 and the surface to the X-Z plane between (0, 0) and (1, 1).
 *
 * @param b The BezierSurface to initialize.
 */
void bezierSurface_init(BezierSurface *b)
{
    if (!b)
    {
        fprintf(stderr, "Null pointer provided to bezierSurface_init\n");
        exit(-1);
    }
}

/**
 * Sets the control points of the BezierCurve to the four points in the vlist array.
 *
 * @param b The BezierCurve to set.
 * @param vlist An array of four points representing the control points.
 */
void bezierCurve_set(BezierCurve *b, Point *vlist)
{
    if (!b || !vlist)
    {
        fprintf(stderr, "Null pointer provided to bezierCurve_set\n");
        exit(-1);
    }
}

/**
 * Sets the control points of the BezierSurface to the 16 points in the vlist array.
 *
 * @param b The BezierSurface to set.
 * @param vlist An array of sixteen points representing the control points.
 */
void bezierSurface_set(BezierSurface *b, Point *vlist)
{
    if (!b || !vlist)
    {
        fprintf(stderr, "Null pointer provided to bezierSurface_set\n");
        exit(-1);
    }
}

/**
 * Sets the z-buffer flag of a Bezier curve to the given value.
 *
 * @param p The BezierCurve whose z-buffer flag is to be set.
 * @param flag The value to set the z-buffer flag to.
 */
void bezierCurve_zBuffer(BezierCurve *p, int flag)
{
    if (!p)
    {
        fprintf(stderr, "Null pointer provided to bezierCurve_zbuffer\n");
        exit(-1);
    }
}

/**
 * Sets the z-buffer flag of a Bezier surface to the given value.
 *
 * @param p The BezierSurface whose z-buffer flag is to be set.
 * @param flag The value to set the z-buffer flag to.
 */
void bezierSurface_zBuffer(BezierSurface *p, int flag)
{
    if (!p)
    {
        fprintf(stderr, "Null pointer provided to bezierSurface_zbuffer\n");
        exit(-1);
    }
}

/**
 * Draws the Bezier curve, given in screen coordinates, into the image using the given color.
 *
 * @param b The BezierCurve to draw.
 * @param src The image to draw the Bezier curve into.
 * @param c The color to use for drawing the Bezier curve.
 */
void bezierCurve_draw(BezierCurve *b, Image *src, Color c)
{
    if (!b || !src)
    {
        fprintf(stderr, "Null pointer provided to bezierCurve_draw\n");
        exit(-1);
    }
}

/**
 * Uses the de Casteljau algorithm to subdivide the Bezier curve divisions times, then adds the lines connecting the control points to the module.
 *
 * @param m The Module to add the lines to.
 * @param b The BezierCurve to subdivide.
 * @param divisions The number of times to subdivide the Bezier curve.
 */
void module_bezierCurve(Module *m, BezierCurve *b, int divisions)
{
    if (!m || !b)
    {
        fprintf(stderr, "Null pointer provided to module_bezierCurve\n");
        exit(-1);
    }
}

/**
 * Uses the de Casteljau algorithm to subdivide the Bezier surface divisions times, then draws either the lines connecting the control points, if solid is 0, or draws triangles using the four corner control points.
 *
 * @param m The Module to add the lines or triangles to.
 * @param b The BezierSurface to subdivide.
 * @param divisions The number of times to subdivide the Bezier surface.
 * @param solid If 0, draws lines; if 1, draws triangles.
 */
void module_bezierSurface(Module *m, BezierSurface *b, int divisions, int solid)
{
    if (!m || !b)
    {
        fprintf(stderr, "Null pointer provided to module_bezierSurface\n");
        exit(-1);
    }
}
