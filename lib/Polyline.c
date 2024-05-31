#include <stdlib.h>
#include "Polyline.h"
#include "Line.h"

/**
 * Returns an allocated Polyline pointer
 * initialized so that numVertex is 0 and vertex is NULL.
 * @return an initialized polyline
 */
Polyline *polyline_create(void)
{
    Polyline *p = (Polyline *)malloc(sizeof(Polyline));
    if (!p)
    {
        fprintf(stderr, "Error creating the image\n");
        exit(-1);
    }
    polyline_init(p);
    return p;
}

/**
 * Returns an allocated Polyline pointer
 * with the vertex list initialized to the points in vlist.
 *
 * @param numV the number of vertices in the list
 * @param vlist the list of points
 * @return the polyline
 */
Polyline *polyline_createp(int numV, Point *vlist)
{
    // Error if vlist is null
    if (!vlist)
    {
        fprintf(stderr, "the vlist was empty\n");
        exit(-1);
    }
    Polyline *p = polyline_create();
    p->vertex = (Point *)malloc(sizeof(Point) * numV);
    if (!p->vertex)
    {
        fprintf(stderr, "polyline memory allocation failed\n");
        exit(-1);
    }
    for (int i = 0; i < numV; i++)
    {
        p->vertex[i] = vlist[i];
    }
    p->numVertex = numV;
    return p;
}

/**
 * Frees the internal data and the Polyline pointer.
 * @param p the Polyline to free
 */
void polyline_free(Polyline *p)
{
    if (p) // Null check
    {
        polyline_clear(p);
        free(p);
    }
}

/**
 * Initializes the pre-existing Polyline to an empty Polyline.
 * @param p the Polyline to initialize
 */
void polyline_init(Polyline *p)
{
    if (p) // Null check
    {
        p->zBuffer = 1;
        p->numVertex = 0;
        p->vertex = NULL;
    }
}

/**
 * Initializes the vertex list to the points in vlist.
 * De-allocates/allocates the vertex list for p, as necessary.
 *
 * @param p the Polyline to modify
 * @param numV the number of vertices in the list
 * @param vList the list of Points
 */
void polyline_set(Polyline *p, int numV, Point *vlist)
{
    if (!p || !vlist)
    {
        fprintf(stderr, "A null pointer was provided\n");
        exit(-1);
    }

    if (p->vertex && vlist)
    {
        polyline_clear(p);
    }
    p->vertex = (Point *)malloc(sizeof(Point) * numV);
    for (int i = 0; i < numV; i++)
    {
        p->vertex[i] = vlist[i];
    }
    p->numVertex = numV;
}

/**
 * Frees the internal data for a Polyline,
 * if necessary, and sets numVertex to 0 and vertex to NULL.
 *
 * @param p the polyline to clear
 */
void polyline_clear(Polyline *p)
{
    // Null check
    if (p)
    {
        if (p->vertex) // check there is already data before freeing
            free(p->vertex);
        p->vertex = NULL;
        p->numVertex = 0;
        p->zBuffer = 1;
    }
}

// utility
/**
 * Sets the z-buffer flag to the given value.
 */
void polyline_zBuffer(Polyline *p, int flag)
{
    if (!p)
    {
        fprintf(stderr, "A null pointer was provided\n");
        exit(-1);
    }
    if (flag == 0 || flag == 1)
        p->zBuffer = flag;
}

/**
 * De-allocates/allocates space as necessary in the destination Polyline
 * data structure and copies the vertex data from the source
 * polyline (from) to the destination (to).
 *
 * @param to the destination Polyline
 * @param from the source Polyline
 */
void polyline_copy(Polyline *to, Polyline *from)
{
    if (!to || !from) // Null checks
    {
        fprintf(stderr, "A null pointer was provided\n");
        exit(-1);
    }
    if (to->vertex) // Check if destination has memory, free if its there
        polyline_clear(to);
    polyline_set(to, from->numVertex, from->vertex);
}

/**
 * Prints Polyline data to the stream designated by the FILE pointer
 *
 * @param p the polyline to print
 * @param fp the output stream
 */
void polyline_print(Polyline *p, FILE *fp)
{
    // Null check for the polyline, file, and the vertex list
    if (p && fp && p->vertex)
    {
        for (int i = 0; i < p->numVertex; i++)
        {
            point_print(&(p->vertex[i]), fp);
        }
    }
}

/**
 * Normalize the x and y values of each vertex by the homogeneous coordinate.
 *
 * @param p the Polyline
 */
void polyline_normalize(Polyline *p)
{
    // Null check
    if (p && p->vertex)
    {
        for (int i = 0; i < p->numVertex; i++)
        {
            point_normalize(&(p->vertex[i]));
        }
    }
}

/**
 * Draw the polyline using color c and the z-buffer, if appropriate
 *
 * @param p the Polyline to draw
 * @param src the Image to draw it onto
 * @param c the color of the line
 */
void polyline_draw(Polyline *p, Image *src, Color c)
{
    // Null checks for polyline, image, and vertex list
    if (p && src && p->vertex)
    {
        Line line;
        for (int i = 0; i < p->numVertex - 1; i++)
        {
            line_set(&line, p->vertex[i], p->vertex[i + 1]);
            line_draw(&line, src, c);
        }
    }
}
