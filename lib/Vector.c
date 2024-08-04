#include <stdlib.h>
#include <math.h>
#include "Vector.h"

/**
 * Constructor that sets the Vector to (x, y, z, 0.0).
 * @param v the vector
 * @param x the x coordinate
 * @param y the y coordinate
 * @param z the z coordinate
 *
 */
void vector_set(Vector *v, double x, double y, double z)
{
    // Null check
    if (!v)
    {
        fprintf(stderr, "Invalid pointer was provided to vector_set\n");
        exit(-1);
    }
    v->val[0] = x;
    v->val[1] = y;
    v->val[2] = z;
    v->val[3] = 0.0;
}

/**
 * Prints the vector out to the desired stream
 * @param v the vector
 * @param fp the output stream
 */
void vector_print(Vector *v, FILE *fp)
{
    if (!v || !fp)
    {
        fprintf(stderr, "Invalid pointer was provided to vector_print\n");
        exit(-1);
    }
    fprintf(fp, "(%.3f, %.3f, %.3f, %.3f)\n", v->val[0], v->val[1], v->val[2], v->val[3]);
}

/**
 * Copies the vector data from the src to destination
 * @param src the source vector
 * @param dest the destination vector
 */
void vector_copy(Vector *dest, Vector *src)
{
    if (!dest || !src)
    {
        fprintf(stderr, "Invalid pointer was provided to vector_copy\n");
        exit(-1);
    }
    for (int i = 0; i < 4; i++)
    {
        dest->val[i] = src->val[i];
    }
}

/**
 * Determines the euclidean length of the vector.
 * @param v the vector
 * @return the length, as a double
 */
double vector_length(Vector *v)
{
    if (!v)
    {
        fprintf(stderr, "Invalid pointer was provided to vector_length\n");
        exit(-1);
    }

    return sqrt(v->val[0] * v->val[0] + v->val[1] * v->val[1] + v->val[2] * v->val[2]);
}

void vector_subtract(Vector *a, Vector *b, Vector *c)
{
    if (!a || !b || !c)
        exit(-1);
    float tempX, tempY, tempZ;
    tempX = b->val[0] - a->val[0];
    tempY = b->val[1] - a->val[1];
    tempZ = b->val[2] - a->val[2];

    vector_set(c, tempX, tempY, tempZ);
}

/**
 * Normalize the vector to unit length
 * @param v the vector
 */
void vector_normalize(Vector *v)
{
    if (!v)
    {
        fprintf(stderr, "Invalid pointer was provided to vector_normalize\n");
        exit(-1);
    }
    double length = vector_length(v);
    v->val[0] /= length;
    v->val[1] /= length;
    v->val[2] /= length;
}

/**
 * Returns the scalar product of a . b (dot product)
 * @param a the first vector
 * @param b the second vector
 * @return double the scalar product
 */
double vector_dot(Vector *a, Vector *b)
{
    if (!a || !b)
    {
        fprintf(stderr, "Invalid pointer was provided to vector_dot\n");
        exit(-1);
    }
    return a->val[0] * b->val[0] + a->val[1] * b->val[1] + a->val[2] * b->val[2];
}

/**
 * Calculates the cross product of a . b and puts the result into c
 * @param a the first vector
 * @param b the second vector
 * @param c the resulting vector
 */
void vector_cross(Vector *a, Vector *b, Vector *c)
{
    if (!a || !b || !c)
    {
        fprintf(stderr, "Invalid pointer was provided to vector_cross\n");
        exit(-1);
    }
    Vector tmp;
    tmp.val[0] = a->val[1] * b->val[2] - a->val[2] * b->val[1]; // Cx = Ay * Bz - Az * By
    tmp.val[1] = a->val[2] * b->val[0] - a->val[0] * b->val[2]; // Cy = Az * Bx - Ax * Bz
    tmp.val[2] = a->val[0] * b->val[1] - a->val[1] * b->val[0]; // Cz = Ax * By - Ay * Bx
    tmp.val[3] = 0;                                             // Confirm c is 0.0
    vector_copy(c, &tmp);
}

// Calculates the surface normals at b using points a, b, c.
void vector_calculateNormal(Vector *N, Point *a, Point *b, Point *c)
{
    if (!N || !a || !b || !c)
    {
        return;
    }
    // Determine the surface normal at b and save it to the normals structure. Assumes a->b->c goes in a clockwise fashion
    Vector Vba, Vca;
    // Cross the two vectors and save it in N
    vector_set(&Vba, b->val[0] - a->val[0], b->val[1] - a->val[1], b->val[2] - a->val[2]);
    vector_set(&Vca, c->val[0] - a->val[0], c->val[1] - a->val[1], c->val[2] - a->val[2]);
    vector_cross(&Vba, &Vca, N);
}
