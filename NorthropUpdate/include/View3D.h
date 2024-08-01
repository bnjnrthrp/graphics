#ifndef VIEW3D_H
#define VIEW3D_H

#include "Point.h"
#include "Vector.h"
#include "Matrix.h"

typedef struct View3D
{
    Point vrp;   // view ref point. 3D vector indicating origin of the view ref coordinates
    Vector vpn;  // 3D vector indicating direction the viewer is looking
    Vector vup;  // 3D vector indicating the UP direction on the view plane. Cannot be parallel to view plane normal
    double d;    // distance in the negative VPN direction at which center of projection is located
    double du;   // Extent of view plane around VRP, in world coords
    double dv;   // Extent of view plane around VRP, in world coords
    double f;    // Front clip planes expressed as distances along positive VPN. F > 0 && F < B
    double b;    // Back clip planes
    int screenx; // Size of desired image in pixels
    int screeny; // Size of desired image in pixels
} View3D;

void matrix_setView3D(Matrix *vtm, View3D *view);

#endif // VIEW3D_H
