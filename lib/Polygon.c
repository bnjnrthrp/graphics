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
        free(p->vertex);

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

    // Check for internal data, free if required
    if (p->vertex)
        free(p->vertex);

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
        p->vertex[i] = vlist[i];
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
