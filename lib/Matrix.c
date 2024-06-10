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
                fprintf(fp, "%.3f, ", m->m[i][j]);
            }
            else
            {
                fprintf(fp, "%.3f\n", m->m[i][j]);
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
                m->m[i][j] = 1.0; // Will only change the diagonal
            }
            else
            {
                m->m[i][j] = 0.0; // Else, results are 0
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
    Matrix tmp;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            // Create a tmp matrix that will hold the result
            tmp.m[j][i] = m->m[i][j];
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
    Matrix tmp;
    double row0, row1, row2, row3, col0, col1, col2, col3;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            // Assignments for readability
            row0 = left->m[i][0];
            row1 = left->m[i][1];
            row2 = left->m[i][2];
            row3 = left->m[i][3];
            col0 = right->m[0][j];
            col1 = right->m[1][j];
            col2 = right->m[2][j];
            col3 = right->m[3][j];

            // Goes through each cell in tmp[i][j] and multiplies the appropriate row and column together

            double result = row0 * col0 + row1 * col1 + row2 * col2 + row3 * col3;

            matrix_set(&tmp, i, j, result);
        }
    }
    matrix_copy(m, &tmp);
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
    // Check if p and q refer to the same point, and early return if so.
    if (p == q)
    {
        return;
    }
    double result;
    for (int i = 0; i < 4; i++)
    {
        result = 0.0;
        for (int j = 0; j < 4; j++)
        {
            // Sums the products of each column in the row of the matrix corresponding to the single column in the point.
            result += m->m[i][j] * p->val[i];
        }
        q->val[i] = result;
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
    // Check if p and q refer to the same point, and early return if so.
    if (p == q)
    {
        return;
    }
    double result;
    for (int i = 0; i < 4; i++)
    {
        result = 0;
        for (int j = 0; j < 4; j++)
        {
            // Sums the products of each column in the row of the matrix corresponding to the single column in the vector.
            result += m->m[i][j] * p->val[i];
        }
        q->val[i] = result;
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
    // Check that p has vertexes to manipulate
    if (!p->vertex || p->nVertex < 1)
    {
        fprintf(stderr, "Empty polygon provided to matrix_xformPolygon\n");
        exit(-1);
    }
    // Create a temp pointlist to hold the transformed list
    Point tmpVertex[p->nVertex];

    // In each point in the list, xform by the matrix and store it in
    for (int i = 0; i < p->nVertex; i++)
    {
        // Perform the transformation
        matrix_xformPoint(m, &(p->vertex[i]), &(tmpVertex[i]));
        // Copy the results back into the original polygon
        point_copy(&(p->vertex[i]), &(tmpVertex[i]));
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
    // Check that p has vertexes to manipulate
    if (!p->vertex || p->numVertex < 1)
    {
        fprintf(stderr, "Empty polyline provided to matrix_xformPolyline\n");
        exit(-1);
    }
    // Create a temp pointlist to hold the transformed list
    Point tmpVertex[p->numVertex];

    // In each point in the list, xform by the matrix and store it in
    for (int i = 0; i < p->numVertex; i++)
    {
        // Perform the transformation
        matrix_xformPoint(m, &(p->vertex[i]), &(tmpVertex[i]));
        // Copy the results back into the original polygon
        point_copy(&(p->vertex[i]), &(tmpVertex[i]));
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
    Point tmp;
    // 1. Transform a and put the results into tmp
    matrix_xformPoint(m, &(line->a), &tmp);
    // 2. copy the results from tmp back to a
    point_copy(&(line->a), &tmp);

    // Repeat 1 and 2 for b
    matrix_xformPoint(m, &(line->b), &tmp);
    point_copy(&(line->b), &tmp);
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
    // Initialize an identity matrix
    Matrix scale;
    matrix_identity(&scale);
    // Set the sx and sy in their correct positions
    matrix_set(&scale, 0, 0, sx);
    matrix_set(&scale, 1, 1, sy);

    // Multiply the matrices
    matrix_multiply(&scale, m, m);
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

    // Initialize an identity matrix
    Matrix rotate;
    matrix_identity(&rotate);
    // Set the sx and sy in their correct positions
    matrix_set(&rotate, 0, 0, cth);
    matrix_set(&rotate, 0, 1, -sth);
    matrix_set(&rotate, 1, 0, sth);
    matrix_set(&rotate, 1, 1, cth);

    // Multiply the matrices
    matrix_multiply(&rotate, m, m);
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
    // Initialize an identity matrix
    Matrix translate;
    matrix_identity(&translate);
    // Set the sx and sy in their correct positions
    matrix_set(&translate, 0, 3, tx);
    matrix_set(&translate, 1, 3, ty);

    // Multiply the matrices
    matrix_multiply(&translate, m, m);
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
    // Initialize an identity matrix
    Matrix shear;
    matrix_identity(&shear);
    // Set the sx and sy in their correct positions
    matrix_set(&shear, 0, 1, shx);
    matrix_set(&shear, 1, 0, shy);

    // Multiply the matrices
    matrix_multiply(&shear, m, m);
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

    // Initialize an identity matrix
    Matrix translate;
    matrix_identity(&translate);
    // Set the sx and sy in their correct positions
    matrix_set(&translate, 0, 3, tx);
    matrix_set(&translate, 1, 3, ty);
    matrix_set(&translate, 2, 3, tz);

    // Multiply the matrices
    matrix_multiply(&translate, m, m);
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
    // Initialize an identity matrix
    Matrix scale;
    matrix_identity(&scale);
    // Set the sx and sy in their correct positions
    matrix_set(&scale, 0, 0, sx);
    matrix_set(&scale, 1, 1, sy);
    matrix_set(&scale, 2, 2, sz);

    // Multiply the matrices
    matrix_multiply(&scale, m, m);
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
    // Initialize an identity matrix
    Matrix rotate;
    matrix_identity(&rotate);
    // Set the sx and sy in their correct positions
    matrix_set(&rotate, 1, 1, cth);
    matrix_set(&rotate, 1, 2, -sth);
    matrix_set(&rotate, 2, 1, sth);
    matrix_set(&rotate, 2, 2, cth);

    // Multiply the matrices
    matrix_multiply(&rotate, m, m);
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
    // Initialize an identity matrix
    Matrix rotate;
    matrix_identity(&rotate);
    // Set the sx and sy in their correct positions
    matrix_set(&rotate, 0, 0, cth);
    matrix_set(&rotate, 0, 2, sth);
    matrix_set(&rotate, 2, 0, -sth);
    matrix_set(&rotate, 2, 2, cth);

    // Multiply the matrices
    matrix_multiply(&rotate, m, m);
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
    // Initialize an identity matrix
    Matrix rotate;
    matrix_identity(&rotate);
    // Set the sx and sy in their correct positions
    /**
     * ux uy uz 0
     * vx vy vz 0
     * wx wy wz 0
     * 0  0  0  1
     */
    // u
    matrix_set(&rotate, 0, 0, u->val[0]);
    matrix_set(&rotate, 0, 1, u->val[1]);
    matrix_set(&rotate, 0, 2, u->val[2]);
    // v
    matrix_set(&rotate, 1, 0, v->val[0]);
    matrix_set(&rotate, 1, 1, v->val[1]);
    matrix_set(&rotate, 1, 2, v->val[2]);
    // w
    matrix_set(&rotate, 2, 0, w->val[0]);
    matrix_set(&rotate, 2, 1, w->val[1]);
    matrix_set(&rotate, 2, 2, w->val[2]);

    // Multiply the matrices
    matrix_multiply(&rotate, m, m);
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
    // Initialize an identity matrix
    Matrix shear;
    matrix_identity(&shear);
    // Set the sx and sy in their correct positions
    matrix_set(&shear, 0, 2, shx);
    matrix_set(&shear, 1, 2, shy);

    // Multiply the matrices
    matrix_multiply(&shear, m, m);
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
    // Initialize an identity matrix
    Matrix prspctv;
    matrix_identity(&prspctv);
    // Set the sx and sy in their correct positions
    matrix_set(&prspctv, 3, 2, (double)1 / d);
    matrix_set(&prspctv, 3, 3, 0);

    // Multiply the matrices
    matrix_multiply(&prspctv, m, m);
}
