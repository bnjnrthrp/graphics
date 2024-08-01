/**
 * This file represents the library for a Point, which is a single dot with an x, y, and z value and a
 * homogenous factor h.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Point.h"

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
        if (p->val[3] == 0)
        {
            fprintf(stderr, "Uh oh, you tried to divide by 0 in point_normalize\n");
            point_print(p, stdout);
            exit(-1);
        }
        p->val[0] = p->val[0] / p->val[3];
        p->val[1] = p->val[1] / p->val[3];
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
        to->pert = from->pert;
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

/**
 * Finds the midpoint of two points and saves it in a provided destination point
 *
 * @param ab the destination midpoint
 * @param a the start point
 * @param b the end point
 */
void point_findMidpoint(Point *ab, Point *a, Point *b)
{
    if (!ab || !a || !b)
    {
        fprintf(stderr, "Invalid pointer provided to point_findMidpoint\n");
        exit(-1);
    }

    point_set3D(ab, (a->val[0] + b->val[0]) * 0.5, (a->val[1] + b->val[1]) * 0.5, (a->val[2] + b->val[2]) * 0.5);
    ab->pert = 0;
}

/**
 * Applies a perturbation to a point, given a roughness scale and length. Adjusts the y coordinate by adding this perturbation.
 * Perturbation = random * 2.0 - 1, giving us a a range between -1.0 to 1.0. This is then multiplified by length ^ roughness to
 * provide a final adjustment range of (-l^r, l^r). This allows for smaller adjustments when adjusting the midpoint of two points close together.
 *
 * @param a the midpoint to adjust
 * @param roughness the roughness value, typically something between 0.5-1.5
 * @param length the length of the line of which a is the midpoint
 */
void point_perturb(Point *a, double roughness, double length)
{
    if (!a)
    {
        fprintf(stderr, "Invalid pointer provided to point_perturb\n");
        exit(-1);
    }
    if (roughness < 1)
    {
        return;
    }
    double adjustment = ((drand48() * 2.0 - 1.0));
    Point pt;
    printf("point's pert is %d\n", a->pert);
    if (a->pert == 0)
    {
        point_set3D(a,
                    a->val[0],
                    a->val[1] + adjustment * pow(length, roughness),
                    a->val[2]);
        a->pert = 1;
    }
    else
    {
        printf("Skipping this endpoint\n");
    }
    printf("Perts point is now %d\n", a->pert);
}
