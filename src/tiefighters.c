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

// makes 3 X-wing fighters in a loose formation
void xwing_build(Module *md);
void xwing_build(Module *md)
{
    Module *scene;
    Module *engine;
    Module *wing;
    Module *wings;
    Module *laser;
    Module *body;
    Polygon p;
    Point pt[4];
    Vector n[4];
    Color Flame, Red, Grey, Dark, White;
    float bodyWidth = 2.0;
    int i;

    if (!md)
    {
        return;
    }

    color_set(&White, 1.0, 1.0, 1.0);
    color_set(&Flame, 1.0, 0.7, 0.2);
    color_set(&Red, 1.0, 0.2, 0.1);
    color_set(&Grey, 0.6, 0.65, 0.67);
    color_set(&Dark, 0.2, 0.2, 0.2);

    // engine
    engine = module_create();
    module_scale(engine, 1.3, 6, 1.3);
    module_rotateX(engine, 0, 1);
    module_cylinder(engine, 10);
    module_scale(engine, .8, .8, 1);
    module_color(engine, &Flame);
    module_cylinder(engine, 10);

    // laser
    laser = module_create();
    module_scale(laser, 0.5, 5, 0.5);
    module_rotateX(laser, 0, 1);
    module_cylinder(laser, 6);
    module_scale(laser, 0.4, 0.4, 1.0);
    module_translate(laser, 0, 0, 4.5);
    module_color(laser, &Red);
    module_cylinder(laser, 10);

    // wing
    wing = module_create();
    polygon_init(&p);
    point_set3D(&pt[0], 0.0, 0.0, 0.0);
    point_set3D(&pt[1], 0.0, 0.0, 5.0);
    point_set3D(&pt[2], 15.0, 0.0, 3.0);
    point_set3D(&pt[3], 15.0, 0.0, 0.0);
    polygon_set(&p, 4, pt);
    for (i = 0; i < 4; i++)
        vector_set(&(n[i]), 0, -1, 0);
    polygon_setNormals(&p, 4, n);
    module_polygon(wing, &p);
    module_translate(wing, 0, 0.5, 0);
    for (i = 0; i < 4; i++)
        vector_set(&(n[i]), 0, 1, 0);
    polygon_setNormals(&p, 4, n);
    module_polygon(wing, &p);

    module_identity(wing);
    module_translate(wing, 3, 1.6, -1);
    module_module(wing, engine);

    module_identity(wing);
    point_set3D(&pt[0], 15.0, 0.0, 3.0);
    point_set3D(&pt[1], 15.0, 0.0, 0.0);
    point_set3D(&pt[2], 15.0, 0.5, 0.0);
    point_set3D(&pt[3], 15.0, 0.5, 3.0);
    polygon_set(&p, 4, pt);
    for (i = 0; i < 4; i++)
        vector_set(&(n[i]), 1, 0, 0);
    polygon_setNormals(&p, 4, n);
    module_polygon(wing, &p);

    point_set3D(&pt[0], 15.0, 0.0, 0.0);
    point_set3D(&pt[1], 0.0, 0.0, 0.0);
    point_set3D(&pt[2], 0.0, 0.5, 0.0);
    point_set3D(&pt[3], 15.0, 0.5, 0.0);
    polygon_set(&p, 4, pt);
    for (i = 0; i < 4; i++)
        vector_set(&(n[i]), 0, 0, -1);
    polygon_setNormals(&p, 4, n);
    module_polygon(wing, &p);

    point_set3D(&pt[0], 15.0, 0.0, 3.0);
    point_set3D(&pt[1], 15.0, 0.5, 3.0);
    point_set3D(&pt[2], 0.0, 0.5, 5.0);
    point_set3D(&pt[3], 0.0, 0.0, 5.0);
    polygon_set(&p, 4, pt);
    for (i = 0; i < 4; i++)
        vector_set(&(n[i]), 2, 0, 15);
    polygon_setNormals(&p, 4, n);
    module_polygon(wing, &p);

    module_translate(wing, 15, 0.25, 0);
    module_module(wing, laser);

    // 4 wings
    wings = module_create();
    module_bodyColor(wings, &Grey);
    module_rotateZ(wings, cos(0.3), sin(0.3));
    module_translate(wings, bodyWidth, 0, 0);
    module_module(wings, wing);

    module_identity(wings);
    module_scale(wings, 1, -1, 1);
    module_rotateZ(wings, cos(-0.3), sin(-0.3));
    module_translate(wings, bodyWidth, 0, 0);
    module_module(wings, wing);

    module_identity(wings);
    module_scale(wings, -1, 1, 1);
    module_rotateZ(wings, cos(-0.3), sin(-0.3));
    module_translate(wings, -bodyWidth, 0, 0);
    module_module(wings, wing);

    module_identity(wings);
    module_scale(wings, -1, -1, 1);
    module_rotateZ(wings, cos(0.3), sin(0.3));
    module_translate(wings, -bodyWidth, 0, 0);
    module_module(wings, wing);

    // body
    body = module_create();
    module_bodyColor(body, &White);
    module_surfaceColor(body, &Dark);

    module_module(body, wings);

    module_scale(body, bodyWidth, bodyWidth, 8);
    module_translate(body, 0, 0, 3);
    module_cube(body, 1);

    module_identity(body);
    point_set3D(&pt[0], bodyWidth, bodyWidth, 12);
    point_set3D(&pt[1], bodyWidth, -bodyWidth, 12);
    point_set3D(&pt[2], bodyWidth * 0.5, -bodyWidth * 0.3, 35);
    point_set3D(&pt[3], bodyWidth * 0.5, bodyWidth * 0.3, 35);
    polygon_set(&p, 4, pt);
    for (i = 0; i < 4; i++)
        vector_set(&(n[i]), 23, 0, 0.5 * bodyWidth);
    polygon_setNormals(&p, 4, n);
    module_polygon(body, &p);

    point_set3D(&pt[0], -bodyWidth, bodyWidth, 12);
    point_set3D(&pt[1], -bodyWidth, -bodyWidth, 12);
    point_set3D(&pt[2], -bodyWidth * 0.5, -bodyWidth * 0.3, 35);
    point_set3D(&pt[3], -bodyWidth * 0.5, bodyWidth * 0.3, 35);
    polygon_set(&p, 4, pt);
    for (i = 0; i < 4; i++)
        vector_set(&(n[i]), -23, 0, 0.5 * bodyWidth);
    polygon_setNormals(&p, 4, n);
    module_polygon(body, &p);

    point_set3D(&pt[0], -bodyWidth, bodyWidth, 12);
    point_set3D(&pt[1], bodyWidth, bodyWidth, 12);
    point_set3D(&pt[2], bodyWidth * 0.5, bodyWidth * 0.3, 35);
    point_set3D(&pt[3], -bodyWidth * 0.5, bodyWidth * 0.3, 35);
    polygon_set(&p, 4, pt);
    for (i = 0; i < 4; i++)
        vector_set(&(n[i]), 0, 23, 0.5 * bodyWidth);
    polygon_setNormals(&p, 4, n);
    module_polygon(body, &p);

    point_set3D(&pt[0], -bodyWidth, -bodyWidth, 12);
    point_set3D(&pt[1], bodyWidth, -bodyWidth, 12);
    point_set3D(&pt[2], bodyWidth * 0.5, -bodyWidth * 0.3, 35);
    point_set3D(&pt[3], -bodyWidth * 0.5, -bodyWidth * 0.3, 35);
    polygon_set(&p, 4, pt);
    for (i = 0; i < 4; i++)
        vector_set(&(n[i]), 0, -23, 0.5 * bodyWidth);
    polygon_setNormals(&p, 4, n);
    module_polygon(body, &p);

    point_set3D(&pt[0], -bodyWidth * 0.5, bodyWidth * 0.3, 35);
    point_set3D(&pt[1], bodyWidth * 0.5, bodyWidth * 0.3, 35);
    point_set3D(&pt[2], bodyWidth * 0.5, -bodyWidth * 0.3, 35);
    point_set3D(&pt[3], -bodyWidth * 0.5, -bodyWidth * 0.3, 35);
    polygon_set(&p, 4, pt);
    for (i = 0; i < 4; i++)
        vector_set(&(n[i]), 0, 0, 1);
    polygon_setNormals(&p, 4, n);
    module_polygon(body, &p);

    // scene with three ships (body module)
    scene = module_create();
    module_scale(scene, .7, .7, .7);
    module_rotateZ(scene, cos(0.5), sin(0.5));
    module_rotateY(scene, cos(-0.1), sin(-0.1));
    module_rotateX(scene, cos(0.2), sin(0.2));
    module_translate(scene, -15, -10, -10);
    module_module(scene, body);

    module_identity(scene);
    module_scale(scene, .7, .7, .7);
    module_rotateZ(scene, cos(0.3), sin(0.3));
    module_rotateY(scene, cos(-0.05), sin(-0.05));
    module_rotateX(scene, cos(0.2), sin(0.2));
    module_translate(scene, 5, -1, -50);
    module_module(scene, body);

    module_identity(scene);
    module_scale(scene, 0.7, 0.7, 0.7);
    module_rotateZ(scene, cos(0.1), sin(0.1));
    module_rotateX(scene, cos(0.2), sin(0.2));
    module_translate(scene, -35, -3, -60);
    module_module(scene, body);

    module_module(md, scene);
    // free the polygon data
    polygon_clear(&p);
}

/**
 * A demo program that builds an animated scene of xwings in a dogfight with tie fighters. Who will win?!
 */
int main(int argc, char *argv[])
{
    const int nFrames = 50;
    View3D view;
    Matrix vtm, gtm;
    Module *tie;
    Module *sphere;
    Module *scene;
    Module *strut;
    Module *wing;
    Module *formation1;
    Module *formation2;
    Module *formation3;
    Module *xwing;
    DrawState *ds;
    Lighting *light;
    Image *src;
    Polygon p;
    Point pt[4];
    Vector N[4];
    Color Blue, Gray, Black, OffWhite, White;
    int t;
    char filename[256];

    color_set(&Blue, 0, 0, 1);
    color_set(&Gray, .5, .5, .5);
    color_set(&Black, .1, .1, .1);
    color_set(&OffWhite, .8, .8, .8);
    color_set(&White, 1, 1, 1);
    // set up the view
    point_set3D(&(view.vrp), 15, 20, 130);
    vector_set(&(view.vpn), -20, -20, -130);
    vector_set(&(view.vup), 0, 1, 0);
    view.d = 2.0;
    view.du = 1.6;
    view.dv = 0.9;
    view.f = 1;
    view.b = 200;
    view.screenx = 1024;
    view.screeny = 720;

    matrix_setView3D(&vtm, &view);
    matrix_identity(&gtm);

    sphere = module_create();
    module_scale(sphere, 3.0, 3.0, 3.0);
    module_rotateY(sphere, cos(M_PI / 8.0), sin(M_PI / 8.0));
    module_rotateX(sphere, 0, 1);
    module_color(sphere, &Gray);
    module_bodyColor(sphere, &Gray);
    module_surfaceColor(sphere, &Gray);
    module_surfaceCoeff(sphere, .8);
    module_sphere(sphere, 8);

    strut = module_create();
    module_scale(strut, .5, .3, .3);
    module_rotateY(strut, cos(-M_PI / 8.0), sin(-M_PI / 8.0));
    module_translate(strut, cos(M_PI / 8.0) + .15, 0, sin(M_PI / 8.0) + .1);
    module_color(strut, &Gray);
    module_bodyColor(strut, &Gray);
    module_surfaceColor(strut, &Gray);
    module_cube(strut, 1);
    module_module(sphere, strut);

    module_rotateY(sphere, -1, 0);
    module_module(sphere, strut);

    wing = module_create();
    polygon_init(&p);
    module_scale(wing, 7, 7, 7);
    module_rotateY(wing, cos(M_PI / 2), sin(M_PI / 2));
    module_translate(wing, 0.5, -3.5, 3.5);
    module_bodyColor(wing, &Black);
    module_surfaceColor(wing, &Black);
    module_surfaceCoeff(wing, .8);

    // Lower left
    point_set3D(&pt[0], .16, 0.0, 0.0);
    point_set3D(&pt[1], 0.0, 0.5, 0.0);
    point_set3D(&pt[2], 0.36, 0.5, 0.0);
    point_set3D(&pt[3], 0.44, 0.38, 0.0);
    polygon_set(&p, 4, pt);

    for (int i = 0; i < 3; i++)
    {
        vector_set(&N[i], 0, 0, 1);
    }
    polygon_setNormals(&p, 4, N);
    module_polygon(wing, &p);

    // Top right
    point_set3D(&pt[0], .84, 1.0, 0.0);
    point_set3D(&pt[1], 1.0, 0.5, 0.0);
    point_set3D(&pt[2], 0.64, 0.5, 0.0);
    point_set3D(&pt[3], 0.56, .62, 0.0);
    polygon_set(&p, 4, pt);
    polygon_setNormals(&p, 4, N);
    module_polygon(wing, &p);

    // Bottom middle
    point_set3D(&pt[0], .16, 0.0, 0.0);
    point_set3D(&pt[1], .44, 0.38, 0.0);
    point_set3D(&pt[2], .56, 0.38, 0.0);
    point_set3D(&pt[3], .85, 0.0, 0.0);
    polygon_set(&p, 4, pt);
    polygon_setNormals(&p, 4, N);
    module_polygon(wing, &p);

    // Top middle
    point_set3D(&pt[0], .16, 1.0, 0.0);
    point_set3D(&pt[1], .44, 0.62, 0.0);
    point_set3D(&pt[2], .56, 0.62, 0.0);
    point_set3D(&pt[3], .84, 1.0, 0.0);
    polygon_set(&p, 4, pt);
    polygon_setNormals(&p, 4, N);
    module_polygon(wing, &p);

    // Bottom Right
    point_set3D(&pt[0], .84, 0.0, 0.0);
    point_set3D(&pt[1], .56, 0.38, 0.0);
    point_set3D(&pt[2], .64, 0.5, 0.0);
    point_set3D(&pt[3], 1.0, 0.5, 0.0);
    polygon_set(&p, 4, pt);
    polygon_setNormals(&p, 4, N);
    module_polygon(wing, &p);

    // Top Left
    point_set3D(&pt[0], .16, 1.0, 0.0);
    point_set3D(&pt[1], .44, 0.62, 0.0);
    point_set3D(&pt[2], .36, 0.5, 0.0);
    point_set3D(&pt[3], 0.0, 0.5, 0.0);
    polygon_set(&p, 4, pt);
    polygon_setNormals(&p, 4, N);
    module_polygon(wing, &p);

    module_module(strut, wing);

    tie = module_create();

    module_module(tie, sphere);

    formation1 = module_create();
    formation2 = module_create();
    formation3 = module_create();
    module_module(formation1, tie);

    module_translate(formation1, -20, 3, 20);
    module_module(formation1, tie);

    module_translate(formation1, 10, 3, -5);
    module_module(formation1, tie);

    module_module(formation2, formation1);
    module_module(formation3, formation1);

    xwing = module_create();
    xwing_build(xwing);

    // Create the image and drawstate
    src = image_create(720, 1024);
    ds = drawstate_create();
    point_copy(&(ds->viewer), &(view.vrp));
    ds->shade = ShadePhong;
    drawstate_setColor(ds, OffWhite);

    for (t = 0; t < nFrames; t++)
    {
        light = lighting_create();
        lighting_add(light, LightAmbient, &OffWhite, NULL, &(view.vrp), 0.0, 0.0);
        lighting_add(light, LightPoint, &White, NULL, &(view.vrp), 0.0, 0.0);
        image_fillc(src, Black);

        scene = module_create();
        // Move the xwings
        module_identity(scene);
        module_scale(scene, .7, .7, .7);
        module_rotateZ(scene, cos(0.5) + .3 * (double)t / (double)nFrames, sin(0.5) + .3 * (double)t / (double)nFrames);
        module_rotateY(scene, cos(-0.1) - .3 * (double)t / (double)nFrames, sin(-0.1) - .3 * (double)t / (double)nFrames);
        module_rotateX(scene, cos(0.2) + .5 * (double)t / (double)nFrames, sin(0.2) + .5 * (double)t / (double)nFrames);
        module_translate(scene, -10.0 * (double)t * (.5 * t / (double)nFrames), -20.0 * (double)t / (double)nFrames, 10.0 * (double)t / (double)nFrames);
        module_module(scene, xwing);

        // Move the first form of tie fighters
        module_identity(scene);
        module_rotateZ(scene, cos(0.5) + .3 * (double)t / (double)nFrames, sin(0.5) + .3 * (double)t / (double)nFrames);
        module_rotateY(scene, cos(-0.1) + .5 * (double)t / (double)nFrames, sin(-0.1) + .5 * (double)t / (double)nFrames);
        module_rotateX(scene, cos(0.2) - .3 * (double)t / (double)nFrames, sin(0.2) - .3 * (double)t / (double)nFrames);
        module_translate(scene, -15 * (double)t * (.5 * t / (double)nFrames), -20 + -10 * (double)t / (double)nFrames, -10 + -10 * (double)t / (double)nFrames);
        module_module(scene, formation1);

        // Move the second formation of tie fighters

        module_identity(scene);
        module_rotateZ(scene, cos(0.5 - (double)t * M_PI / (double)nFrames), sin(0.5 - (double)t * M_PI / (double)nFrames));
        module_rotateY(scene, cos(-0.1) + .5 * (double)t * M_PI / (double)nFrames, sin(-0.1) + .5 * (double)t * M_PI / (double)nFrames);
        module_rotateX(scene, cos(0.2 - .7 * (double)t * M_PI / ((double)nFrames * 1.5)), sin(0.2 - .7 * (double)t * M_PI / ((double)nFrames) / 1.5));
        module_translate(scene, 10 + 15 * (double)t * (.5 * t / (double)nFrames), -30 + 5 * (double)t / (double)nFrames, -30 + 10 * (double)t / (double)nFrames);
        module_module(scene, formation2);

        // Move the third formation of tie fighters
        module_identity(scene);
        module_rotateZ(scene, cos(0.5) + .3 * (double)t / (double)nFrames, sin(0.5) + .3 * (double)t / (double)nFrames);
        module_rotateY(scene, cos(-0.1) + .5 * (double)t / (double)nFrames, sin(-0.1) + .5 * (double)t / (double)nFrames);
        module_rotateX(scene, cos(0.2) - .3 * (double)t / (double)nFrames, sin(0.2) - .3 * (double)t / (double)nFrames);
        module_translate(scene, 10 - (double)t * (.5 * t / (double)nFrames), 30 - 5 * (double)t / (double)nFrames, -100 + 10 * (double)t / (double)nFrames);
        module_module(scene, formation3);

        module_draw(scene, &vtm, &gtm, ds, light, src);

        printf("Writing file\n");
        sprintf(filename, "frame-%04d.ppm", t);
        // Write out the image
        image_write(src, filename);
        module_delete(scene);
        lighting_clear(light);
    }

    // Cleanup
    module_delete(sphere);
    module_delete(wing);
    module_delete(formation1);
    module_delete(formation2);
    module_delete(formation3);
    module_delete(xwing);
    module_delete(tie);
    module_delete(strut);

    free(ds);
    image_free(src);

    return (0);
}
