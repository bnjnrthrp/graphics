/**
 * This is a library file for matrix operations
 * @author Benji Northrop
 */

#include <stdlib.h>
#include "Matrix.h"

// 2D and Generic Functions

/**
 * Prints the contents of the matrix to the specified output stream.
 *
 * @param m A pointer to the matrix to be printed.
 * @param fp A pointer to the file where the matrix will be printed.
 */
void matrix_print(Matrix *m, FILE *fp)
{
    if (!m || !fp)
    {
        fprintf(stderr, "Invalid pointer to matrix_print\n");
        exit(-1);
    }
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (j != 3)
            {
                fprintf(fp, "%d, ", m->m[i][j]);
            }
            else
            {
                fprintf(fp, "%d\n", m->m[i][j]);
            }
        }
    }
    fprintf(fp, "\n"); // Add extra blank line at bottom
}

/**
 * Clears the matrix by setting all its elements to zero.
 *
 * @param m A pointer to the matrix to be cleared.
 */
void matrix_clear(Matrix *m)
{
    if (!m)
    {
        fprintf(stderr, "Invalid pointer to matrix_clear\n");
        exit(-1);
    }
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            m->m[i][j] = 0; // Set matrix to 0
        }
    }
}

/**
 * Sets the matrix to the identity matrix.
 *
 * @param m A pointer to the matrix to be set as identity.
 */
void matrix_identity(Matrix *m)
{
    if (!m)
    {
        fprintf(stderr, "Invalid pointer to matrix_identity\n");
        exit(-1);
    }
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (i == j)
            {
                m->m[i][j] = 1; // Will only change the diagonal
            }
        }
    }
}

/**
 * Retrieves the value at the specified row and column of the matrix.
 *
 * @param m A pointer to the matrix.
 * @param r The row index.
 * @param c The column index.
 * @return The value at the specified row and column of the matrix.
 */
double matrix_get(Matrix *m, int r, int c)
{
    if (!m)
    {
        fprintf(stderr, "Invalid pointer to matrix_get\n");
        exit(-1);
    }
    // Ensure r and c within bounds
    if (r < 0 || c < 0 || r > 3 || c > 3)
    {
        fprintf(stderr, "r and c provided out of bounds in matrix_get\n");
        exit(-1);
    }
    return m->m[r][c];
}

/**
 * Sets the value at the specified row and column of the matrix.
 *
 * @param m A pointer to the matrix.
 * @param r The row index.
 * @param c The column index.
 * @param v The value to set.
 */
void matrix_set(Matrix *m, int r, int c, double v)
{
    if (!m)
    {
        fprintf(stderr, "Invalid pointer to matrix_set\n");
        exit(-1);
    }
    // Ensure r and c within bounds
    if (r < 0 || c < 0 || r > 3 || c > 3)
    {
        fprintf(stderr, "r and c provided out of bounds in matrix_get\n");
        exit(-1);
    }
    m->m[r][c] = v;
}

/**
 * Copies the contents of the source matrix to the destination matrix.
 *
 * @param dest A pointer to the destination matrix.
 * @param src A pointer to the source matrix.
 */
void matrix_copy(Matrix *dest, Matrix *src)
{
    if (!dest || !src)
    {
        fprintf(stderr, "Invalid pointer to matrix_copy\n");
        exit(-1);
    }
    // Iterate through each item in the matrix and copy the value over
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {

            dest->m[i][j] = src->m[i][j];
        }
    }
}

/**
 * Transposes the matrix over the diagonal. **Only works for 4x4 matrices**
 *
 * @param m A pointer to the matrix to be transposed.
 */
void matrix_transpose(Matrix *m)
{
    if (!m)
    {
        fprintf(stderr, "Invalid pointer to matrix_transpose\n");
        exit(-1);
    }
    double tmp[4][4];

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            // Create a tmp matrix that will hold the result
            tmp[j][i] = m->m[i][j];
        }
    }

    // Copy over the resulting tmp back into m->m
    matrix_copy(m, &tmp);
}

/**
 * Multiplies two matrices and stores the result in a third matrix.
 *
 * @param left A pointer to the left matrix.
 * @param right A pointer to the right matrix.
 * @param m A pointer to the result matrix.
 */
void matrix_multiply(Matrix *left, Matrix *right, Matrix *m)
{
    if (!left || !right || !m)
    {
        fprintf(stderr, "Invalid pointer to matrix_multiply\n");
        exit(-1);
    }
}

/**
 * Transforms a point using the matrix.
 *
 * @param m A pointer to the transformation matrix.
 * @param p A pointer to the input point.
 * @param q A pointer to the output point.
 */
void matrix_xformPoint(Matrix *m, Point *p, Point *q)
{
    if (!m || !p || !q)
    {
        fprintf(stderr, "Invalid pointer to matrix_xformPoint\n");
        exit(-1);
    }
}

/**
 * Transforms a vector using the matrix.
 *
 * @param m A pointer to the transformation matrix.
 * @param p A pointer to the input vector.
 * @param q A pointer to the output vector.
 */
void matrix_xformVector(Matrix *m, Vector *p, Vector *q)
{
    if (!m || !p || !q)
    {
        fprintf(stderr, "Invalid pointer to matrix_xformVector\n");
        exit(-1);
    }
}

/**
 * Transforms a polygon using the matrix.
 *
 * @param m A pointer to the transformation matrix.
 * @param p A pointer to the polygon to be transformed.
 */
void matrix_xformPolygon(Matrix *m, Polygon *p)
{
    if (!m || !p)
    {
        fprintf(stderr, "Invalid pointer to matrix_xformPolygon\n");
        exit(-1);
    }
}

/**
 * Transforms a polyline using the matrix.
 *
 * @param m A pointer to the transformation matrix.
 * @param p A pointer to the polyline to be transformed.
 */
void matrix_xformPolyline(Matrix *m, Polyline *p)
{
    if (!m || !p)
    {
        fprintf(stderr, "Invalid pointer to matrix_xformPolyline\n");
        exit(-1);
    }
}

/**
 * Transforms a line using the matrix.
 *
 * @param m A pointer to the transformation matrix.
 * @param line A pointer to the line to be transformed.
 */
void matrix_xformLine(Matrix *m, Line *line)
{
    if (!m || !line)
    {
        fprintf(stderr, "Invalid pointer to matrix_xformLine\n");
        exit(-1);
    }
}

/**
 * Applies a 2D scaling transformation to the matrix.
 *
 * @param m A pointer to the matrix to be scaled.
 * @param sx The scaling factor in the x direction.
 * @param sy The scaling factor in the y direction.
 */
void matrix_scale2D(Matrix *m, double sx, double sy)
{
    if (!m)
    {
        fprintf(stderr, "Invalid pointer to matrix_scale2D\n");
        exit(-1);
    }
}

/**
 * Applies a rotation around the Z-axis to the matrix.
 *
 * @param m A pointer to the matrix to be rotated.
 * @param cth The cosine of the rotation angle.
 * @param sth The sine of the rotation angle.
 */
void matrix_rotateZ(Matrix *m, double cth, double sth)
{
    if (!m)
    {
        fprintf(stderr, "Invalid pointer to matrix_rotateZ\n");
        exit(-1);
    }
}

/**
 * Applies a 2D translation to the matrix.
 *
 * @param m A pointer to the matrix to be translated.
 * @param tx The translation distance in the x direction.
 * @param ty The translation distance in the y direction.
 */
void matrix_translate2D(Matrix *m, double tx, double ty)
{
    if (!m)
    {
        fprintf(stderr, "Invalid pointer to matrix_translate2D\n");
        exit(-1);
    }
}

/**
 * Applies a 2D shear transformation to the matrix.
 *
 * @param m A pointer to the matrix to be sheared.
 * @param shx The shear factor in the x direction.
 * @param shy The shear factor in the y direction.
 */
void matrix_shear2D(Matrix *m, double shx, double shy)
{
    if (!m)
    {
        fprintf(stderr, "Invalid pointer to matrix_shear2D\n");
        exit(-1);
    }
}

// 3D functions

/**
 * Applies a 3D translation to the matrix.
 *
 * @param m A pointer to the matrix to be translated.
 * @param tx The translation distance in the x direction.
 * @param ty The translation distance in the y direction.
 * @param tz The translation distance in the z direction.
 */
void matrix_translate(Matrix *m, double tx, double ty, double tz)
{
    if (!m)
    {
        fprintf(stderr, "Invalid pointer to matrix_translate\n");
        exit(-1);
    }
}

/**
 * Applies a 3D scaling transformation to the matrix.
 *
 * @param m A pointer to the matrix to be scaled.
 * @param sx The scaling factor in the x direction.
 * @param sy The scaling factor in the y direction.
 * @param sz The scaling factor in the z direction.
 */
void matrix_scale(Matrix *m, double sx, double sy, double sz)
{
    if (!m)
    {
        fprintf(stderr, "Invalid pointer to matrix_scale\n");
        exit(-1);
    }
}

/**
 * Applies a rotation around the X-axis to the matrix.
 *
 * @param m A pointer to the matrix to be rotated.
 * @param cth The cosine of the rotation angle.
 * @param sth The sine of the rotation angle.
 */
void matrix_rotateX(Matrix *m, double cth, double sth)
{
    if (!m)
    {
        fprintf(stderr, "Invalid pointer to matrix_rotateX\n");
        exit(-1);
    }
}

/**
 * Applies a rotation around the Y-axis to the matrix.
 *
 * @param m A pointer to the matrix to be rotated.
 * @param cth The cosine of the rotation angle.
 * @param sth The sine of the rotation angle.
 */
void matrix_rotateY(Matrix *m, double cth, double sth)
{
    if (!m)
    {
        fprintf(stderr, "Invalid pointer to matrix_rotateY\n");
        exit(-1);
    }
}

/**
 * Applies a rotation around an arbitrary axis to the matrix.
 *
 * @param m A pointer to the matrix to be rotated.
 * @param u The x component of the rotation axis.
 * @param v The y component of the rotation axis.
 * @param w The z component of the rotation axis.
 */
void matrix_rotateXYZ(Matrix *m, Vector *u, Vector *v, Vector *w)
{
    if (!m || !u || !v || !w)
    {
        fprintf(stderr, "Invalid pointer to matrix_rotateXYZ\n");
        exit(-1);
    }
}

/**
 * Applies a shear transformation along the Z-axis to the matrix.
 *
 * @param m A pointer to the matrix to be sheared.
 * @param shx The shear factor in the x direction.
 * @param shy The shear factor in the y direction.
 */
void matrix_shearZ(Matrix *m, double shx, double shy)
{
    if (!m)
    {
        fprintf(stderr, "Invalid pointer to matrix_shearZ\n");
        exit(-1);
    }
}

/**
 * Applies a perspective transformation to the matrix.
 *
 * @param m A pointer to the matrix to be transformed.
 * @param d The distance to the projection plane.
 */
void matrix_perspective(Matrix *m, double d)
{
    if (!m)
    {
        fprintf(stderr, "Invalid pointer to matrix_perspective\n");
        exit(-1);
    }
}
