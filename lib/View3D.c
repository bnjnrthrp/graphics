/**
 * Builds a 3D viewing pipeline
 *
 * @author Benji Northrop
 */

#include <stdlib.h>
#include "View3D.h"

/**
 * Builds the 3D view pipeline
 * @param vtm the view matrix
 * @param the 3D view struct
 */
void matrix_setView3D(Matrix *vtm, View3D *view)
{
    if (!vtm || !view)
    {
        fprintf(stderr, "Invalid pointer provided to matrix_setView3D\n");
        exit(-1);
    }
    Vector U;
    Vector vup2;
    Vector vpn2;
    double b2;
    double d2;
    vector_copy(&vpn2, &(view->vpn));
    matrix_identity(vtm);
    // debug
    printf("Matrix identity\n");
    matrix_print(vtm, stdout);
    vector_cross(&(view->vup), &(view->vpn), &U);                                   // U = VUP x VPN
    vector_cross(&(view->vpn), &U, &(vup2));                                        // VUP2 = VPN x U
    matrix_translate(vtm, -view->vrp.val[0], -view->vrp.val[1], -view->vrp.val[2]); // VTM1 = T(-VRP);
    // debug
    printf("After translation\n");
    matrix_print(vtm, stdout);
    // debug
    vector_normalize(&U);
    vector_normalize(&vup2);
    vector_normalize(&vpn2);
    printf("view reference axes");
    vector_print(&U, stdout);
    vector_print(&vup2, stdout);
    vector_print(&vpn2, stdout);
    matrix_rotateXYZ(vtm, &U, &vup2, &vpn2); // orient the view coordinates
    // debug
    printf("After rotation\n");
    matrix_print(vtm, stdout);
    matrix_translate(vtm, 0, 0, view->d); // Translate by d to move COP to origin
    printf("After translating COP to origin\n");
    matrix_print(vtm, stdout);
    b2 = view->d + view->b;                                                                            // Update back clip distance
    matrix_scale(vtm, (2.0 * view->d) / (b2 * view->du), (2.0 * view->d) / (b2 * view->dv), 1.0 / b2); // Scale to canonical view volume
    // debug
    printf("After scaling to cvv\n");
    matrix_print(vtm, stdout);
    d2 = view->d / b2; // Project screen onto view plane
    matrix_perspective(vtm, d2);
    printf("After perspective\n");
    matrix_print(vtm, stdout);
    matrix_scale2D(vtm, -view->screenx / (2 * d2), -view->screeny / (2 * d2)); // scale to image size, flip axes
    // debug
    printf("After scale to image coords\n");
    matrix_print(vtm, stdout);
    matrix_translate2D(vtm, view->screenx / 2, view->screeny / 2); // translate image to center

    // debug
    printf("After translate to screen\n");
    matrix_print(vtm, stdout);
}
