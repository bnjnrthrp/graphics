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
    p->color = NULL;
    p->normal = NULL;
    p->vertex3D = NULL;
    p->normalPhong = NULL;
    p->zBuffer = 1;
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

    // Check for vertex, color, and normal data, free if found and reset to null
    if (p->vertex)
        free(p->vertex);
    p->vertex = NULL;

    if (p->color)
        free(p->color);
    p->color = NULL;

    if (p->normal)
        free(p->normal);
    p->normal = NULL;

    if (p->vertex3D)
        free(p->vertex3D);

    if (p->normalPhong)
        free(p->normalPhong);
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
 * Sets the colors of each vertex of a polygon
 * @param p the polygon
 * @param numV the number of colors in the list
 * @param clist a pointer to the color array
 */
void polygon_setColors(Polygon *p, int numV, Color *clist)
{
    if (!p || !clist)
    {
        fprintf(stderr, "A null pointer was provided to polygon_setColors\n");
        exit(-1);
    }

    // Check to ensure number of vertices is non-negative
    if (numV < 0)
    {
        fprintf(stderr, "Number of vertices provided was < 0 (polygon_setColors())\n");
        exit(-1);
    }

    // If there's already a colors list, then clear it out and reinitialize the memory
    if (p->color)
    {
        free(p->color);
    }
    p->color = (Color *)malloc(sizeof(Color) * numV);
    // Null check incase memory failed
    if (!p->color)
    {
        fprintf(stderr, "polygon vertex memory allocation failed\n");
        exit(-1);
    }
    for (int i = 0; i < numV; i++)
    {
        color_copy(&(p->color[i]), &(clist[i]));
    }
}

/**
 * Sets the normals of the polygon
 * @param p the polygon
 * @param numV the number of vertices in the vector list
 * @param nlist a pointer to an array of normals
 */
void polygon_setNormals(Polygon *p, int numV, Vector *nlist)
{
    if (!p || !nlist)
    {
        fprintf(stderr, "A null pointer was provided to polygon_setNormals\n");
        exit(-1);
    }

    // Check to ensure number of vertices is non-negative
    if (numV < 0)
    {
        fprintf(stderr, "Number of vertices provided was < 0 (polygon_setNormals())\n");
        exit(-1);
    }

    // If there's already a Normals list, then clear it out and reinitialize the memory
    if (p->normal)
    {
        free(p->normal);
    }
    p->normal = (Vector *)malloc(sizeof(Vector) * numV);
    // Null check incase memory failed
    if (!p->normal)
    {
        fprintf(stderr, "polygon vertex memory allocation failed\n");
        exit(-1);
    }
    for (int i = 0; i < numV; i++)
    {
        vector_copy(&(p->normal[i]), &(nlist[i]));
        vector_normalize(&(p->normal[i]));
    }
}

void polygon_setNormalsPhong(Polygon *p, int numV, Vector *nlist)
{
    if (!p || !nlist)
    {
        fprintf(stderr, "A null pointer was provided to polygon_setNormals\n");
        exit(-1);
    }

    // Check to ensure number of vertices is non-negative
    if (numV < 0)
    {
        fprintf(stderr, "Number of vertices provided was < 0 (polygon_setNormalsPhong())\n");
        exit(-1);
    }

    // If there's already a Normals list, then clear it out and reinitialize the memory
    if (p->normalPhong)
    {
        free(p->normalPhong);
    }
    p->normalPhong = (Vector *)malloc(sizeof(Vector) * numV);
    // Null check incase memory failed
    if (!p->normalPhong)
    {
        fprintf(stderr, "polygon vertex memory allocation failed\n");
        exit(-1);
    }
    for (int i = 0; i < numV; i++)
    {
        vector_copy(&(p->normalPhong[i]), &(nlist[i]));
        vector_normalize(&(p->normalPhong[i]));
    }
}

/**
 * Sets the vertex list for the points in 3D space (after LTM and GTM, stores for Phong shading)
 */
void polygon_setVertex3D(Polygon *p, int numV, Point *plist)
{
    if (!p || !plist)
    {
        fprintf(stderr, "Invalid pointer provided to polygon_setVertex3D\n");
        exit(-1);
    }
    if (numV < 0)
    {
        fprintf(stderr, "NumV to polygon_setVertex3D was < 0\n");
        return;
    }

    if (p->vertex3D)
        free(p->vertex3D);

    p->vertex3D = (Point *)malloc(sizeof(Point) * numV);

    if (!p->vertex3D)
    {
        fprintf(stderr, "Point3d memory allocation fail in polygon_setVertex3D\n");
        exit(-1);
    }
    for (int i = 0; i < numV; i++)
    {
        point_copy(&(p->vertex3D[i]), &(plist[i]));
    }
}

/**
 * Sets all the fields of a polygon
 * @param p the polygon to set
 * @param numV the number of vertices of the polygon
 * @param clist a pointer to the color array
 * @param nlist a pointer to the normals array
 * @param zBuffer The z-buffer value
 * @param oneSided if the polygon is one-sided or not
 */
void polygon_setAll(Polygon *p, int numV, Point *vlist, Color *clist, Vector *nlist, int zBuffer, int oneSided)
{
    if (!p || !vlist || !clist || !nlist)
    {
        fprintf(stderr, "A null pointer was provided to polygon_setAll\n");
        exit(-1);
    }
    // Bounds check the other data
    if (numV < 0 || zBuffer < 0 || zBuffer > 1 || oneSided < 0 || oneSided > 1)
    {
        fprintf(stderr, "Invalid parameter (numV, zBuffer, or oneSided) provided to polygon_setAll\n");
        exit(-1);
    }
    p->nVertex = numV;
    polygon_set(p, numV, vlist);
    polygon_setColors(p, numV, clist);
    polygon_setNormals(p, numV, nlist);
    polygon_zBuffer(p, zBuffer);
    polygon_setSided(p, oneSided);
}

/**
 * Sets the zbuffer of a polygon
 * @param p the polygon
 * @param flag 1 for true, 0 for false
 */
void polygon_zBuffer(Polygon *p, int flag)
{
    if (!p)
    {
        fprintf(stderr, "A null pointer was provided to polygon_zBuffer\n");
        exit(-1);
    }
    if (flag == 0 || flag == 1)
    {
        p->zBuffer = flag;
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
    if (from->color)
    {
        polygon_setColors(to, from->nVertex, from->color);
    }
    if (from->normal)
    {
        polygon_setNormals(to, from->nVertex, from->normal);
    }
    if (from->normalPhong)
    {
        polygon_setNormalsPhong(to, from->nVertex, from->normalPhong);
    }
    if (from->vertex3D)
    {
        polygon_setVertex3D(to, from->nVertex, from->vertex3D);
    }

    polygon_zBuffer(to, from->zBuffer);
    polygon_setSided(to, from->oneSided);
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
    if (p->normalPhong)
    {
        printf("Normals:\n");
        for (int i = 0; i < p->nVertex; i++)
        {
            vector_print(&(p->normalPhong[i]), fp);
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
        point_normalize(&(p->vertex3D[i]));
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

/**
 * Calculates the shading at each vertex of a polygon
 *
 * @param p the polygon to calculate colors
 */
void polygon_shade(Polygon *p, DrawState *ds, Lighting *l)
{
    if (!p || !ds)
    {
        fprintf(stderr, "Invalid pointer sent to polygon_shade\n");
        exit(-1);
    }
    int i;
    Color c[p->nVertex];

    for (i = 0; i < p->nVertex; i++)
    {
        Vector tempV;
        vector_subtract(&(p->vertex[i]), &(ds->viewer), &tempV);
        lighting_shading(l, &(p->normal[i]), &tempV, &(p->vertex[i]), &(ds->body), &(ds->surface), ds->surfaceCoeff, p->oneSided, &c[i]);
    }

    polygon_setColors(p, p->nVertex, c);
}
