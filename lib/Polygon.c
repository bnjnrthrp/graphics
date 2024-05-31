/**
 * This class represents a polygon, which can take many shapes and vertices.
 *
 * @author Benji Northrop
 */

#include <stdlib.h>
#include "Polygon.h"

// Constructors
/**
 * Returns an allocate Polygon pointer
 * initialized to its default values
 * @return Polygon pointer
 */
Polygon *polygon_create(void);

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
}

/**
 * Frees the internal data and resets the fields of a polygon
 *
 * @param p the polygon to initialize
 */
void polygon_clear(Polygon *p)
{
    // Null check
    if (!p)
    {
        fprintf(stderr, "A null pointer was provided to polygon_clear\n");
        exit(-1);
    }
}

// setters/getters
/**
 * Sets the oneSided value of the polygon
 *
 * @param p the polygon to initialize
 * @param oneSided 1 for true, 0 for false (two-sided)
 */
void polygon_setSided(Polygon *p, int oneSided)
{
    // Null check
    if (!p)
    {
        fprintf(stderr, "A null pointer was provided to polygon_setSided\n");
        exit(-1);
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
    if (!p || !src)
    {
        fprintf(stderr, "A null pointer was provided to polygon_draw\n");
        exit(-1);
    }
}

/**
 * Draw a filled polygon on an image using the color c.
 * Uses a scanline z-buffer rendering algorithm
 */
void polygon_drawFill(Polygon *p, Image *src, Color c)
{
    // Null check
    if (!p || !src)
    {
        fprintf(stderr, "A null pointer was provided to polygon_drawFill\n");
        exit(-1);
    }
}
