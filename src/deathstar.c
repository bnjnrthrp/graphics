/**
 * A simple 3d viewing program to show a hologram model of the death star
 *
 * @author Benji Northrop
 */
#include <math.h>
#include "Graphics.h"
#define M_PI 3.14159265358979323846

/*
Program to demonstrate creating a 3D object
*/
int main(int argc, char *argv[])
{
    Image *src;
    const int rows = 250;
    const int cols = 250;
    const int Resolution = 15;
    const int nFrames = 25;
    View3D view;
    Color White, Black;
    Point unitSphere[Resolution * Resolution];
    Point tpt[Resolution * Resolution];
    Point ptt[Resolution * Resolution];
    int i, j, t;
    Matrix vtm;
    double center;
    char filename[256];

    color_set(&White, 1, 1, 1);
    color_set(&Black, 0, 0, 0);

    // Set up the unit sphere
    for (i = 0; i < Resolution; i++)
    {
        // Determine the y coordinate of each center of the circle
        center = sin(i * M_PI / (float)Resolution - .5 * M_PI);
        for (j = 0; j < Resolution; j++)
        {
            // Set points for each circle (x, z) at the height y
            point_set(&(unitSphere[i * Resolution + j]),
                      cos(j * 2.0 * M_PI / (float)Resolution) * cos(i * M_PI / (float)Resolution - .5 * M_PI),
                      center,
                      sin(j * 2.0 * M_PI / (float)Resolution) * cos(i * M_PI / (float)Resolution - .5 * M_PI),
                      1);
        }
    }
    // matrix_rotateX(&vtm, cos(M_PI / 4.0), sin(M_PI / 4.0));
    // // Put points into a temporary polygon for printing
    // for (i = 0; i < Resolution * Resolution; i++)
    // {
    //     point_copy(&(ptt[i]), &(unitSphere[i]));
    //     matrix_xformPoint(&vtm, &(ptt[i]), &(tpt[i]));
    // }

    vector_set(&(view.vup), 0, 1, 0);
    view.d = 1.0; // focal length
    view.du = 2.0;
    view.dv = 2.0;
    view.f = 0; // front clip plane
    view.b = 4; // back clip plane
    view.screenx = cols;
    view.screeny = rows;

    src = image_create(rows, cols);
    for (t = 0; t < nFrames; t++)
    {
        image_fillc(src, Black);
        // rotate the VRP to its correct position
        float alpha = (float)(t * 4.0) / 360.0;
        point_set(&(view.vrp), cos(2.0 * M_PI * alpha), 2, sin(2.0 * M_PI * alpha), 1.0);
        vector_set(&(view.vpn), -view.vrp.val[0], -view.vrp.val[1], -view.vrp.val[2]);

        matrix_setView3D(&vtm, &view);
        // Create the image

        for (int i = 0; i < Resolution * Resolution; i++)
        {
            matrix_xformPoint(&vtm, &(unitSphere[i]), &(tpt[i]));
            point_normalize(&(tpt[i]));

            point_draw(&(tpt[i]), src, White);
        }

        sprintf(filename, "sphere-%02d.ppm", t);
        image_write(src, filename);
    }
    image_free(src);

    return (0);
}
