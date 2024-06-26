/**
 * This class represents a polygon, which can take many shapes and vertices.
 *
 * @author Benji Northrop
 */

#include <stdlib.h>
#include "Polygon.h"
#include "list.h"
#include "Line.h"

// Constructors
/**
 * Returns an allocate Polygon pointer
 * initialized to its default values
 * @return Polygon pointer
 */
Polygon *polygon_create(void)
{
    Polygon *p = (Polygon *)malloc(sizeof(Polygon));
    if (!p)
    {
        fprintf(stderr, "Memory allocation failed in polygon_create\n");
        exit(-1);
    }
    // Initialize the polygon to default values
    polygon_init(p);
    return p;
}

/**
 * Returns an allocated Polygon pointer with the vertex
 * list initialized to a copy of the points in vlist.
 *
 * @param numV the number of vertices
 * @param vlist the list of vertices
 * @return Polygon pointer
 */
Polygon *polygon_createp(int numV, Point *vlist)
{
    // Null check
    if (!vlist)
    {
        fprintf(stderr, "A null pointer was provided to polygon_createp\n");
        exit(-1);
    }

    if (numV < 0)
    {
        fprintf(stderr, "Negative number provided to polygon_createp for numV\n");
        exit(-1);
    }
    // Create the polygon pointer
    Polygon *p = polygon_create();
    // Set up the vertex list
    polygon_set(p, numV, vlist);

    return p;
}

/**
 * Frees the internal data for a Polygon and the Polygon pointer
 */
void polygon_free(Polygon *p)
{
    // Null check
    if (!p)
    {
        fprintf(stderr, "A null pointer was provided to polygon_free\n");
        exit(-1);
    }
    // Check for vertex data, free if found
    if (p->vertex)
    {
        free(p->vertex);
    }
    // free the Polygon
    free(p);
}

// Initialize, set, free
/**
 * Initializes the existing Polygon to an empty Polygon
 *
 * @param p the polygon to initialize
 */
void polygon_init(Polygon *p)
{
    // Null check
    if (!p)
    {
        fprintf(stderr, "A null pointer was provided to polygon_init\n");
        exit(-1);
    }
    p->oneSided = 1;
    p->nVertex = 0;
    p->vertex = NULL;
}

/**
 * Initializes the vertex array to the points in vlist
 *
 * @param p the polygon to initialize
 * @param numV the number of vertices in the list
 * @param vlist the list of points
 */
void polygon_set(Polygon *p, int numV, Point *vlist)
{
    // Null check
    if (!p || !vlist)
    {
        fprintf(stderr, "A null pointer was provided to polygon_set\n");
        exit(-1);
    }

    // Check to ensure number of vertices is non-negative
    if (numV < 0)
    {
        fprintf(stderr, "Number of vertices provided was < 0 (polygon_set())\n");
        exit(-1);
    }

    // If there's already a vertex list, then clear it out
    if (p->vertex)
    {
        polygon_clear(p);
    }

    // Allocate the memory for the vertex list in the polygon struct
    p->vertex = (Point *)malloc(sizeof(Point) * numV);

    // Null check incase the malloc failed
    if (!p->vertex)
    {
        fprintf(stderr, "polygon vertex memory allocation failed\n");
        exit(-1);
    }

    // Copy data over from vlist to p->vertex. Avoids aliasing.
    for (int i = 0; i < numV; i++)
    {
        point_copy(&(p->vertex[i]), &(vlist[i]));
    }
    p->nVertex = numV;
}

/**
 * Frees the internal data and resets the fields of a polygon
 *
 * @param p the polygon to clear and reset
 */
void polygon_clear(Polygon *p)
{
    // Null check
    if (!p)
    {
        fprintf(stderr, "A null pointer was provided to polygon_clear\n");
        exit(-1);
    }

    // Check for vertex data, free if found and reset to null
    if (p->vertex)
        free(p->vertex);
    p->vertex = NULL;

    // Reinitialize the polygon to default values
    polygon_init(p);
}

// setters/getters
/**
 * Sets the oneSided value of the polygon
 *
 * @param p the polygon to initialize
 * @param oneSided 1 for one sided, 0 for two-sided
 */
void polygon_setSided(Polygon *p, int oneSided)
{
    // Null check
    if (!p)
    {
        fprintf(stderr, "A null pointer was provided to polygon_setSided\n");
        exit(-1);
    }

    // Check bounds of oneSided. Do nothing if outside range
    if (oneSided == 0 || oneSided == 1)
    {
        p->oneSided = oneSided;
    }
}

/**
 * De-allocates/allocates space and copies the vertex and color data from
 * one polygon to another
 *
 * @param to the destination polygon
 * @param from the source polygon
 */
void polygon_copy(Polygon *to, Polygon *from)
{
    // Null check
    if (!to || !from)
    {
        fprintf(stderr, "A null pointer was provided to polygon_copy\n");
        exit(-1);
    }

    // Check if destination has vertices to free. Set to 0.
    if (to->vertex)
    {
        free(to->vertex);
        to->nVertex = 0;
        to->vertex = NULL;
    }

    // Check the source has vertices to copy,
    // then copy that data over with polygon_set
    if (from->vertex)
    {
        polygon_set(to, from->nVertex, from->vertex);
    }
}

/**
 * Prints the polygon data to the stream designated by the file pointer
 *
 * @param p the polygon to print
 * @param fp the output stream
 */
void polygon_print(Polygon *p, FILE *fp)
{
    // Null check
    if (!p || !fp)
    {
        fprintf(stderr, "A null pointer was provided to polygon_print\n");
        exit(-1);
    }

    // Print the basic struct info
    printf("Sides (1 for 1 sided, 0 for 2-sided): %d\n", p->oneSided);
    printf("Num Vertices: %d\n", p->nVertex);

    // Check there are vertices to print
    if (!p->vertex)
    {
        printf("Vertices: None\n");
    }
    else
    {
        printf("Vertices:\n");
        // If there is data, iterate through list and print each point.
        for (int i = 0; i < p->nVertex; i++)
        {
            point_print(&(p->vertex[i]), fp);
        }
    }
}

/**
 * Normalizes the x and y values of each vertex by the homogeneous coord
 *
 * @param p the polygon to normalize
 */
void polygon_normalize(Polygon *p)
{
    // Null check
    if (!p)
    {
        fprintf(stderr, "A null pointer was provided to polygon_normalize\n");
        exit(-1);
    }

    // Null check the vertex list
    if (!p->vertex)
    {
        fprintf(stderr, "No vertices to normalize\n");
        exit(-1);
    }

    for (int i = 0; i < p->nVertex; i++)
    {
        point_normalize(&(p->vertex[i]));
    }
}

/**
 * Draw the outline of the polygon using a provided color
 *
 * @param p the polygon to draw
 * @param src the image to draw it on
 * @param c the color to use
 */
void polygon_draw(Polygon *p, Image *src, Color c)
{
    // Null check
    if (!p || !src || !p->vertex)
    {
        fprintf(stderr, "A null pointer was provided to polygon_draw\n");
        exit(-1);
    }

    Line line;
    for (int i = 0; i < p->nVertex - 1; i++)
    {
        line_set(&line, p->vertex[i], p->vertex[i + 1]);
        line_draw(&line, src, c);
    }
    // Connect the last point and first point
    line_set(&line, p->vertex[p->nVertex - 1], p->vertex[0]);
    line_draw(&line, src, c);
}

/**
 * Helper function to determine the minimum x or y value. Takes three double values
 * and returns the lowest value.
 */
static double minDouble(double a, double b, double c)
{
    // Compare a to b, then compare the smaller of those to c
    if (a < b) // case a < b
    {
        if (a < c) // Case a < b < c or a < c < b
        {
            return a;
        }
        else // Case c < a < b
        {
            return c;
        }
    }
    else if (b < c) // case b < a
    {
        return b; // case b < c < a
    }
    return c; // case c < b < a
};

/**
 * Helper function to determine the maximum x or y value. Takes 3 double values and
 * returns the highest value
 */
static double maxDouble(double a, double b, double c)
{
    // Compare a to b, then compare the smaller of those to c
    if (a > b) // case a > b
    {
        if (a > c) // Case a > b > c or a > c > b
        {
            return a;
        }
        else // Case c > a > b
        {
            return c;
        }
    }
    else if (b > c) // case b > a
    {
        return b; // case b > c > a
    }
    return c; // case c > b > a
}

/**
 * Draws and fills a polygon triangle with a color using barycentric triangle algorithm
 *
 * @param p the polygon to draw, must be only 3 vertices
 * @param src the image to draw it on
 * @param c the color of the polygon
 */
void polygon_drawFillB(Polygon *p, Image *src, Color c)
{
    Point A, B, C;
    double a, b, g, minX, maxX, minY, maxY, Xa, Ya, Xb, Yb, Xc, Yc, Px, Py;
    int i, j;

    // Null checks
    if (!p || !src)
    {
        fprintf(stderr, "Null pointer provided to polygon_drawFillB\n");
        exit(-1);
    }

    if (p->nVertex != 3)
    {
        printf("Incorrect number of vertices for a barycentric triangle!\n");
        return;
    }

    // Assign initial points A, B, C
    A = p->vertex[0];
    B = p->vertex[1];
    C = p->vertex[2];
    // Assign to x and y components for readability
    Xa = A.val[0];
    Ya = A.val[1];
    Xb = B.val[0];
    Yb = B.val[1];
    Xc = C.val[0];
    Yc = C.val[1];

    // Determine if the triangle is CCW - shoestring formula will be negative
    double area = (Xa * Yb + Xb * Yc + Xc * Ya) - (Ya * Xb + Yb * Xc + Yc * Xa);

    if (area > 0)
    { // if positive, then order was clockwise
        Point tmp = B;
        B = C;
        C = tmp;
    }
    else if (area == 0)
    {
        printf("Points are colinear\n");
        return;
    }

    // Determine the boundary box dimensions.
    // Min to max x, min to max y
    minX = (int)(minDouble(Xa, Xb, Xc) + .5);
    maxX = (int)(maxDouble(Xa, Xb, Xc) + .5);
    minY = (int)(minDouble(Ya, Yb, Yc) + .5);
    maxY = (int)(maxDouble(Ya, Yb, Yc) + .5);

    // For each pixel in boundary box,
    // if the pixel is in the triangle, color it
    for (i = minY; i < maxY; i++)
    {
        for (j = minX; j < maxX; j++)
        {
            Px = (float)j + .5; // pixel x value
            Py = (float)i + .5; // pixel y value
            // Calculate beta nd gamma, then deduce alpha
            b = (((Yc - Ya) * Px) + ((Xa - Xc) * Py) + (Xc * Ya) - (Xa * Yc)) / (((Yc - Ya) * Xb) + ((Xa - Xc) * Yb) + (Xc * Ya) - (Xa * Yc));
            g = (((Ya - Yb) * Px) + ((Xb - Xa) * Py) + (Xa * Yb) - (Xb * Ya)) / (((Ya - Yb) * Xc) + ((Xb - Xa) * Yc) + (Xa * Yb) - (Xb * Ya));
            a = 1 - b - g;

            if (a >= 0 && a <= 1 && b >= 0 && b <= 1 && g >= 0 && g <= 1)
            {
                image_setColor(src, i, j, c);
            }
        }
    }
}
