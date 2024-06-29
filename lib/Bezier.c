/**
 * This file represents the Bezier curve and Bezier surfaces datatypes and their functions
 * @author Benji Northrop
 */

#include <stdlib.h>
#include "Bezier.h"
#include "Line.h"
#include "Point.h"

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
    point_set(&(b->cp[1]), .33, 0.0, 0.0, 1.0);
    point_set(&(b->cp[2]), 0.66, 0.0, 0.0, 1.0);
    point_set(&(b->cp[3]), 1.0, 0.0, 0.0, 1.0);
    b->zbuffer = 1;
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

    for (int i = 0; i < 4; i++)
    {
        point_copy(&(b->cp[i]), &(vlist[i]));
    }
}

/**
 * Copies the data of a beziercurve from one to another
 * @param to the destination Bezier curve
 * @param from the source Bezier curve
 */
void bezierCurve_copy(BezierCurve *to, BezierCurve *from)
{
    if (!to || !from)
    {
        fprintf(stderr, "Invalid pointer provided to bezierCurve_copy\n");
        exit(-1);
    }

    // Copy each point
    for (int i = 0; i < 4; i++)
    {
        point_copy(&(to->cp[i]), &(from->cp[i]));
    }
    to->zbuffer = from->zbuffer;
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
    if (flag == 0 || flag == 1)
    {
        p->zbuffer = flag;
    }
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

    point_set(&b->cp[0], 0.0, 0.0, 0.0, 1.0);
    point_set(&b->cp[1], 0.0, 0.0, 0.33, 1.0);
    point_set(&b->cp[2], 0.0, 0.0, 0.66, 1.0);
    point_set(&b->cp[3], 0.0, 0.0, 1.0, 1.0);
    point_set(&b->cp[4], 0.33, 0.0, 0.0, 1.0);
    point_set(&b->cp[5], 0.33, 0.0, 0.33, 1.0);
    point_set(&b->cp[6], 0.33, 0.0, 0.66, 1.0);
    point_set(&b->cp[7], 0.33, 0.0, 1.0, 1.0);
    point_set(&b->cp[8], 0.66, 0.0, 0.0, 1.0);
    point_set(&b->cp[9], 0.66, 0.0, 0.33, 1.0);
    point_set(&b->cp[10], 0.66, 0.0, 0.66, 1.0);
    point_set(&b->cp[11], 0.66, 0.0, 1.0, 1.0);
    point_set(&b->cp[12], 1.0, 0.0, 0.0, 1.0);
    point_set(&b->cp[13], 1.0, 0.0, 0.33, 1.0);
    point_set(&b->cp[14], 1.0, 0.0, 0.66, 1.0);
    point_set(&b->cp[15], 1.0, 0.0, 1.0, 1.0);
    b->zbuffer = 1;
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
    if (flag == 0 || flag == 1)
    {
        p->zbuffer = flag;
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

    for (int i = 0; i < 16; i++)
    {
        point_copy(&(b->cp[i]), &(vlist[i]));
    }
}

/**
 * Helper function that will check the bounding box of a bezier curve and return true if within
 * specified limits
 * @param b the BezierCurve
 * @param dim the threshold dimension desired. Recommend 10
 *
 * @return 1 if the maximum dimension of the bounding box is less than
 */
static int checkBoundingBox(BezierCurve *b, int dim)
{
    if (!b || dim < 1)
    {
        fprintf(stderr, "Illegal input provided to checkBoundingBox\n");
        exit(-1);
    }

    int minX, minY, maxX, maxY;

    // Initialize the first point as both min and max
    minX = maxX = b->cp[0].val[0];
    minY = maxY = b->cp[0].val[1];

    // Iterate through each Point and keep track of the min and max values of x, y, and z.
    for (int i = 1; i < 4; i++)
    {
        if (b->cp[i].val[0] < minX)
        {
            minX = b->cp[i].val[0];
        }
        else if (b->cp[i].val[0] > maxX)
        {
            maxX = b->cp[i].val[0];
        }

        if (b->cp[i].val[1] < minY)
        {
            minY = b->cp[i].val[1];
        }
        else if (b->cp[i].val[1] > maxY)
        {
            maxY = b->cp[i].val[1];
        }
    }

    // If all dimensions are within boundaries of the provided boundary box, return 1
    if (maxX - minX <= dim && maxY - minY <= dim)
    {
        return 1;
    }
    // else return false
    return 0;
}

/**
 * Helper function that runs Casteljau's algorithm on a beziercurve to determine the control points for the 2 subcurves.
 *
 * @param b the original Bezier curve
 * @param vlist a pointer to an array of points for the subpoints
 */
static void casteljau(BezierCurve *b, Point *vlist)
{
    if (!b || !vlist)
    {
        fprintf(stderr, "Invalid pointer provided to casteljau function\n");
        exit(-1);
    }
    // Initialize the points for holding
    Point q0, q1, q2, q3, r0, r1, r2, r3;

    // Calculate the 7 control points from the original curve, for readability
    point_copy(&q0, &(b->cp[0]));                                                                                                                      // q0 = p0
    point_copy(&r3, &(b->cp[3]));                                                                                                                      // r3 = p3
    point_set(&q1, .5 * (b->cp[0].val[0] + b->cp[1].val[0]), .5 * (b->cp[0].val[1] + b->cp[1].val[1]), .5 * (b->cp[0].val[2] + b->cp[1].val[2]), 1.0); // q1 = .5 * (p0 + p1)
    point_set(&r2, .5 * (b->cp[2].val[0] + b->cp[3].val[0]), .5 * (b->cp[2].val[1] + b->cp[3].val[1]), .5 * (b->cp[2].val[2] + b->cp[3].val[2]), 1);   // r2 = .5 * (p2 + p3)
    point_set(&q2, .5 * q1.val[0] + .25 * (b->cp[1].val[0] + b->cp[2].val[0]),                                                                         // q2 = .5 * q1 + .25 * (p1 + p2)
              .5 * q1.val[1] + .25 * (b->cp[1].val[1] + b->cp[2].val[1]),
              .5 * q1.val[2] + .25 * (b->cp[1].val[2] + b->cp[2].val[2]),
              1);
    point_set(&r1, .5 * r2.val[0] + .25 * (b->cp[1].val[0] + b->cp[2].val[0]),
              .5 * r2.val[1] + .25 * (b->cp[1].val[1] + b->cp[2].val[1]),
              .5 * r2.val[2] + .25 * (b->cp[1].val[2] + b->cp[2].val[2]),
              1);                                                                                                // r1 = .5 * r2 + .25 * (p1 + p2)
    point_set(&q3, .5 * (q2.val[0] + r1.val[0]), .5 * (q2.val[1] + r1.val[1]), .5 * (q2.val[2] + r1.val[2]), 1); // q3 = .5 * (q2 + r1)
    point_copy(&r0, &q3);                                                                                        // r0 = q3

    // Copy points over to the array, for ease of access
    point_copy(&(vlist[0]), &q0);
    point_copy(&(vlist[1]), &q1);
    point_copy(&(vlist[2]), &q2);
    point_copy(&(vlist[3]), &q3);
    point_copy(&(vlist[4]), &r0);
    point_copy(&(vlist[5]), &r1);
    point_copy(&(vlist[6]), &r2);
    point_copy(&(vlist[7]), &r3);
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
    int threshold = 10; // 10 pixels for the threshold
    Line line;
    // Base case: If the max dimension of a bounding box is < than the threshold, draw the segments and return
    if (checkBoundingBox(b, threshold))
    {
        for (int i = 0; i < 3; i++)
        {
            line_set(&line, b->cp[i], b->cp[i + 1]);
            line_draw(&line, src, c);
        }
    }
    else
    {
        // Initialize the sub-curves
        BezierCurve left, right;
        bezierCurve_init(&left);
        bezierCurve_init(&right);
        Point vList[8];

        casteljau(b, vList);

        // Set the points to the new bezierCurves and recursively call the function
        bezierCurve_set(&left, &(vList[0]));
        bezierCurve_set(&right, &(vList[4]));
        bezierCurve_draw(&left, src, c);
        bezierCurve_draw(&right, src, c);
    }
}
