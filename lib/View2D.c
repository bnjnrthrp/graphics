#include <math.h>
#include <stdlib.h>
#include "View2D.h"

void matrix_setView2D(Matrix *vtm, View2D *view)
{
    if (!vtm || !view)
    {
        fprintf(stderr, "Invalid pointer provided to matrix_setView2D\n");
        exit(-1);
    }
    // Center points
    double Vx0 = view->vrp.val[0];
    double Vy0 = view->vrp.val[1];
    double height = view->dx * (double)(view->screeny) / (double)(view->screenx);

    matrix_identity(vtm);
    matrix_translate2D(vtm, -Vx0, -Vy0);                                                        // Translate to the origin
    matrix_rotateZ(vtm, view->x.val[0], view->x.val[1]);                                        // Rotate to align the x axis to the origin
    matrix_scale2D(vtm, (double)(view->screenx) / view->dx, (double)-(view->screeny) / height); // Scale the view window
    matrix_translate2D(vtm, view->screenx / 2, view->screeny / 2);                              // Shift view window back to center
}
