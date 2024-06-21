/**
 * This is a demo program that builds a scene of a flyby of three formations
 * of TIE fighters.
 *
 * @author Benji Northrop
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Graphics.h"
#define M_PI 3.14159265358979323846

void unitSphere(Module *mod, int Resolution);
void unitSphere(Module *mod, int Resolution)
{
    double center;
    int i, j;
    Point unitSphere[Resolution];
    Polygon plygn;

    polygon_init(&plygn);
    for (i = 0; i < Resolution; i++)
    {
        // Determine the y coordinate of each center of the circle
        center = sin(i * M_PI / (float)Resolution - .5 * M_PI);
        for (j = 0; j < Resolution; j++)
        {
            // Set points for each circle (x, z) at the height y
            point_set(&(unitSphere[j]),
                      cos(j * 2.0 * M_PI / (float)Resolution) * cos(i * M_PI / (float)Resolution - .5 * M_PI),
                      center,
                      sin(j * 2.0 * M_PI / (float)Resolution) * cos(i * M_PI / (float)Resolution - .5 * M_PI),
                      1);
        }
        polygon_set(&plygn, Resolution, unitSphere);
        polygon_print(&plygn, stdout);
        module_polygon(mod, &plygn);
        printf("Made polygon number %d\n", i);
    }
    polygon_clear(&plygn);
}

int main(int argc, char *argv[])
{
    View3D view;
    Matrix vtm, gtm;
    Module *sphere;
    Module *scene;
    Module *strut;
    DrawState *ds;
    Image *src;
    Color Blue, Gray, Black;

    color_set(&Blue, 0, 0, 1);
    color_set(&Gray, .5, .5, .5);
    color_set(&Black, .1, .1, .1);
    // set up the view
    point_set3D(&(view.vrp), 0, 0, 10);
    vector_set(&(view.vpn), 0, 0, -10);
    vector_set(&(view.vup), 0, 1, 0);
    view.d = 1.5;
    view.du = 1.0;
    view.dv = 1.0;
    view.f = 1;
    view.b = 100;
    view.screenx = 360;
    view.screeny = 360;

    matrix_setView3D(&vtm, &view);
    matrix_identity(&gtm);

    printf("Creating sphere\n");
    sphere = module_create();
    module_scale(sphere, 2, 2, 2);
    module_rotateY(sphere, cos(M_PI / 8), sin(M_PI / 8));
    module_rotateX(sphere, 0, 1);
    module_color(sphere, &Gray);
    unitSphere(sphere, 8);

    strut = module_create();
    module_scale(strut, 1, .5, .5);
    module_translate(strut, 1, 0, 0);
    module_color(strut, &Gray);
    module_cube(strut, 0);

    // Create a scene with 2 cubes
    scene = module_create();
    module_scale(scene, .5, .5, .5);
    module_module(scene, sphere);

    // Create the image and drawstate
    src = image_create(360, 360);
    ds = drawstate_create();
    ds->shade = ShadeFrame;
    drawstate_setColor(ds, Blue);

    module_draw(scene, &vtm, &gtm, ds, NULL, src);

    // Write out the image
    image_write(src, "tiefighters.ppm");

    // Cleanup
    module_delete(scene);
    module_delete(sphere);
    free(ds);
    image_free(src);

    return (0);
}
