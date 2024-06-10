/**
 * This file represents the library for a Point, which is a single dot with an x, y, and z value and a
 * homogenous factor h.
 */

#include <stdio.h>
#include "Point.h"
#include <math.h>

/**
 * Set the 2D location (first two values) of the point to x and y. Sets z to 0.0 and h to 1.0.
 * @param p the point to set
 * @param x the x value
 * @param y the y value
 */
void point_set2D(Point *p, double x, double y)
{
    if (p)
    {
        p->val[0] = x;
        p->val[1] = y;
        p->val[2] = 0.0;
        p->val[3] = 1.0;
    }
}

/**
 * Set the 3D location (first three values) of the point to x, y, and z. Sets h to 1.0.
 * @param p the point to set
 * @param x the x value
 * @param y the y value
 * @param z the z value
 */
void point_set3D(Point *p, double x, double y, double z)
{
    if (p)
    {
        p->val[0] = x;
        p->val[1] = y;
        p->val[2] = z;
        p->val[3] = 1.0;
    }
}

/**
 * Sets all values of the point (x, y, z, and h)
 * @param x the x value
 * @param y the y value
 * @param z the z value
 * @param h the h value
 */
void point_set(Point *p, double x, double y, double z, double h)
{
    if (p)
    {
        p->val[0] = x;
        p->val[1] = y;
        p->val[2] = z;
        p->val[3] = h;
    }
}

/**
 * Normalizes the x and y values of a point by its homogeneous coordinate: x = x/h, y = y/h.
 * @param p the point to normalize.
 */
void point_normalize(Point *p)
{
    if (p)
    {

        double x, y, h;
        x = p->val[0];
        y = p->val[1];
        h = p->val[3];
        x = x / h;
        y = y / h;
    }
}
/**
 * Copy the point data structure
 * @param to the destination Point
 * @param from the source Point
 */
void point_copy(Point *to, Point *from)
{
    if (to && from)
    {

        for (int i = 0; i < 4; i++)
        {
            to->val[i] = from->val[i];
        }
    }
}
/**
 * Draw the point into src using color c
 * @param p the point to draw
 * @param src the image to draw it on
 * @param c the color of the point
 */
void point_draw(Point *p, Image *src, Color c)
{
    int x, y;

    if (p && src) // null check
    {
        x = (int)floor(p->val[0]);
        y = (int)floor(p->val[1]);
        image_setColor(src, src->rows - 1 - y, x, c);
    }
}

/**
 * Draw the point into src using FPixel c
 * @param p the point to draw
 * @param src the image to draw it on
 * @param c the color of the point (FPixel)
 */
void point_drawf(Point *p, Image *src, FPixel c)
{
    int x, y;
    if (p && src) // Null check
    {
        x = (int)floor(p->val[0]);
        y = (int)floor(p->val[1]);
        image_setf(src, src->rows - 1 - y, x, c);
    }
}
void point_print(Point *p, FILE *fp)
{
    if (fp)
    {
        fprintf(fp, "(%.3f, %.3f, %.3f, %.3f)\n", p->val[0], p->val[1], p->val[2], p->val[3]);
    }
}
