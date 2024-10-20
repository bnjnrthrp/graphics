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
 * Calculates the vector from one point to another. Calculates as v = dest - src
 *
 * @param v the resultant vector
 * @param src the source point
 * @param dest the destination point
 */
void vector_setPoints(Vector *v, Point *src, Point *dest)
{
    if (!v || !src || !dest)
    {
        fprintf(stderr, "Invalid pointer was provided to vector_setPoints\n");
        exit(-1);
    }
    v->val[0] = dest->val[0] - src->val[0];
    v->val[1] = dest->val[1] - src->val[1];
    v->val[2] = dest->val[2] - src->val[2];
    v->val[3] = 0.0;
}

/**
 * Calculates the end point given by the parametric equation of a line.
 * @param A the anchor point
 * @param t the distance to travel down the vector
 * @param V the vector to travel
 * @param p the resulting point
 */
void vector_calcParametric(Point *A, double t, Vector *V, Point *p)
{
    // Update this and fix the equation
    // p = A + tV
    if (!A || !V || !p)
    {
        fprintf(stderr, "Invalid pointer provided to vector_calcParametric\n");
        exit(-1);
    }
    point_set3D(p, A->val[0] + t * V->val[0], A->val[1] + t * V->val[1], A->val[2] + t * V->val[2]);
}

/**
 * Multiplies a vector by a scalar value
 */
void vector_multiplyScalar(Vector *v, double scalar)
{
    if (!v)
    {
        fprintf(stderr, "Invalid pointer provided to vector_multiplyScalar\n");
        exit(-1);
    }

    v->val[0] *= scalar;
    v->val[1] *= scalar;
    v->val[2] *= scalar;
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

void vector_inverse(Vector *input, Vector *output)
{
    if (!input || !output)
    {
        fprintf(stderr, "Invalid pointer provided to vector_inverse\n");
        exit(-1);
    }
    for (int i = 0; i < 3; i++)
    {
        output->val[i] = -(input->val[i]);
    }
}

/**
 * Subtracts vector a from b and puts the result in c
 */
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
