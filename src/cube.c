/**
 * Demo program to build a unit cube using hierarchical graphics engine
 * @author Benji Northrop
 */

#include <stdlib.h>
#include "Graphics.h"

int main(int argc, char *argv[])
{
    View3D view;
    Matrix vtm, gtm;
    Module *cubeFrame;
    Module *cubeFill;
    Module *scene;
    DrawState *ds;
    Image *src;
    Color Blue;

    color_set(&Blue, 0, 0, 1);
    // set up the view
    point_set3D(&(view.vrp), 2, 3, 15);
    vector_set(&(view.vpn), -2, -3, -15);
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

    cubeFrame = module_create();
    module_color(cubeFrame, &Blue);
    module_scale(cubeFrame, 5, 5, 5);
    module_cube(cubeFrame, 0);

    cubeFill = module_create();
    module_color(cubeFill, &Blue);
    module_scale(cubeFill, 4, 4, 4);
    module_cube(cubeFill, 1);

    // Create a scene with 2 cubes
    scene = module_create();
    module_scale(scene, .8, .8, .8);
    module_translate(scene, -2, -2, 0);
    module_module(scene, cubeFrame);

    module_identity(scene);
    module_scale(scene, .5, .5, .5);
    module_translate(scene, 1.5, 1.5, 0);
    module_module(scene, cubeFill);

    // Create the image and drawstate
    src = image_create(360, 360);
    ds = drawstate_create();
    ds->shade = ShadeConstant;
    drawstate_setColor(ds, Blue);

    module_draw(scene, &vtm, &gtm, ds, NULL, src);

    // Write out the image
    image_write(src, "cube.ppm");

    // Cleanup
    module_delete(scene);
    module_delete(cubeFill);
    module_delete(cubeFrame);
    free(ds);
    image_free(src);

    return (0);
}
