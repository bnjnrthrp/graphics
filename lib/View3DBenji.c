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
    vector_cross(&(view->vup), &(view->vpn), &U);                                   // U = VUP x VPN
    vector_cross(&(view->vpn), &U, &(vup2));                                        // VUP2 = VPN x U
    matrix_translate(vtm, -view->vrp.val[0], -view->vrp.val[1], -view->vrp.val[2]); // VTM1 = T(-VRP);
    vector_normalize(&U);
    vector_normalize(&vup2);
    vector_normalize(&vpn2);
    matrix_rotateXYZ(vtm, &U, &vup2, &vpn2);                                                           // orient the view coordinates
    matrix_translate(vtm, 0, 0, view->d);                                                              // Translate by d to move COP to origin
    b2 = view->d + view->b;                                                                            // Update back clip distance
    matrix_scale(vtm, (2.0 * view->d) / (b2 * view->du), (2.0 * view->d) / (b2 * view->dv), 1.0 / b2); // Scale to canonical view volume
    d2 = view->d / b2;                                                                                 // Project screen onto view plane
    matrix_perspective(vtm, d2);
    matrix_scale2D(vtm, -view->screenx / (2 * d2), -view->screeny / (2 * d2)); // scale to image size, flip axes
    matrix_translate2D(vtm, view->screenx / 2, view->screeny / 2);             // translate image to center
}

void view_calculateCOP(Point *COP, Point *VRP, double d, Vector *VPN)
{
    if (!COP || !VRP || !VPN)
    {
        fprintf(stderr, "Invalid pointer provided to view_calculateCOP\n");
        exit(-1);
    }
    // COP = VRP - d in direction of -VPN

    // BAM have to normalize the VPN for this to work properly
    Vector v;
    vector_normalize(VPN);
    vector_copy(&v, VPN);
    vector_normalize(&v);
    point_set3D(COP,
                VRP->val[0] - d * v.val[0],
                VRP->val[1] - d * v.val[1],
                VRP->val[2] - d * v.val[2]);

    printf("VPN: ");
    vector_print(VPN, stdout);
    printf("VRP: ");
    vector_print(VRP, stdout);
    printf("COP: ");
    point_print(COP, stdout);
}
