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
    const int nFrames = 10;
    View3D view;
    Color White;
    Point unitSphere[Resolution * Resolution];
    Point tpt[Resolution * Resolution];
    int i, j, t;
    Matrix vtm;
    double center;
    char filename[256];

    color_set(&White, 1, 1, 1);

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

    // Put points into a temporary polygon for printing
    for (i = 0; i < Resolution * Resolution; i++)
    {
        point_copy(&(tpt[i]), &(unitSphere[i]));
        printf("Copied: %d:", i);
        point_print(&(tpt[i]), stdout);
    }

    // // grab command line argument to determine viewpoint
    // // and set up the view structure
    // if (argc > 1)
    // {
    //     float alpha = atof(argv[1]);
    //     if (alpha < 0.0 || alpha > 1.0)
    //         alpha = 0.0;

    //     point_set(&(view.vrp), 3 * alpha, 2 * alpha, -2 * alpha - (1.0 - alpha) * 3, 1.0);
    // }
    // else
    // {
    //     point_set(&(view.vrp), 3, 2, -2, 1.0);
    // }
    src = image_create(rows, cols);
    for (i = 0; i < 50; i++)
    {
        float alpha = (float)i / 50;
        point_set(&(view.vrp), 3 * alpha, 2 * alpha, -2 * alpha - (1.0 - alpha) * 3, 1.0);
        vector_set(&(view.vpn), -view.vrp.val[0], -view.vrp.val[1], -view.vrp.val[2]);

        vector_set(&(view.vup), 0, 1, 0);
        view.d = 1; // focal length
        view.du = 2;
        view.dv = view.du * (float)rows / cols;
        view.f = 0; // front clip plane
        view.b = 4; // back clip plane
        view.screenx = cols;
        view.screeny = rows;

        matrix_setView3D(&vtm, &view);
        // Create the image

        for (int i = 0; i < Resolution * Resolution; i++)
        {
            matrix_xformPoint(&vtm, &(unitSphere[i]), &(tpt[i]));
            printf("Normalizing point number %d\n", i);
            point_print(&(tpt[i]), stdout);
            point_normalize(&(tpt[i]));

            point_draw(&(tpt[i]), src, White);
        }

        printf("That's no moon, that's a space station\n");
        sprintf(filename, "deathstar-%04d.ppm", i);
        image_write(src, filename);
    }

    image_free(src);
    return (0);
}
