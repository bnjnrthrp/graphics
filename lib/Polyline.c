#include "Polyline.h"

/**
 * Returns an allocated Polyline pointer
 * initialized so that numVertex is 0 and vertex is NULL.
 * @return an initialized polyline
 */
Polyline *polyline_create()
{
    Polyline *p = (Polyline *)malloc(sizeof(Polyline));
    if (!p)
    {
        fprintf(stderr, "Error creating the image\n");
        exit(-1);
    }
    polyline_init(p);
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
    if (vlist)
    {
        Polyline *p = polyline_create();
        p->vertex = (Point *)malloc(sizeof(Point) * numV);
        for (int i = 0; i < numV; i++)
        {
            p->vertex[i] = vlist[i];
        }
        p->numVertex = numV;
        return p;
    }
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
        if (p->vertex) // Check if data in the vertex list
        {
            polyline_clear(p);
        }
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
    if (p->vertex)
        free(p->vertex);
    p->numVertex = 0;
    p->zBuffer = 1;
}

// utility
/**
 * Sets the z-buffer flag to the given value.
 */
void polyline_zBuffer(Polyline *p, int flag)
{
    if (p && (flag == 0 || flag == 1))
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
    if (to && from) // Null checks
    {
        if (to->vertex) // Check if destination has memory, free if its there
            polyline_clear(to);
        polyline_set(to, from->numVertex, from->vertex);
    }
}

/**
 * Prints Polyline data to the stream designated by the FILE pointer
 *
 * @param p the polyline to print
 * @param fp the output stream
 */
void polyline_print(Polyline *p, FILE *fp);

/**
 * Normalize the x and y values of each vertex by the homogeneous coordinate.
 *
 * @param p the Polyline
 */
void polyline_normalize(Polyline *p);

/**
 * Draw the polyline using color c and the z-buffer, if appropriate
 *
 * @param p the Polyline to draw
 * @param src the Image to draw it onto
 * @param c the color of the line
 */
void polyline_draw(Polyline *p, Image *src, Color c);
