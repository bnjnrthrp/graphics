#ifndef VIEW2D_H
#define VIEW2D_H

#include "Matrix.h"

typedef struct View2D
{
    Point vrp;   // Center of the view rectangle V0 in world coordinates
    double dx;   // Width of view rectangle du in world coordinates
    Vector x;    // theta v, x axis as normalized vector (nx, ny).
    int screenx; // number of cols in output image
    int screeny; // number of rows in output image
} View2D;

void matrix_setView2D(Matrix *vtm, View2D *view);
#endif // VIEW2D_H
