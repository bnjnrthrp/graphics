/*
  Bruce A. Maxwell
  Fall 2014

  Example code for drawing a single cube

  C Version
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "../include/Graphics.h"

int main(int argc, char *argv[])
{
  Image *src;
  Matrix VTM;
  Matrix GTM;
  Module *cube;
  Module *floor;
  Module *scene;
  int rows = 360;
  int cols = 640;

  Color White;
  Color Grey;
  Color DkGrey;

  DrawState *ds;
  View3D view;
  RayTracer *rt;

  Lighting *light;

  color_set(&White, 1.0, 1.0, 1.0);
  color_set(&Grey, 0.6, 0.62, 0.64);
  color_set(&DkGrey, 0.1, 0.1, 0.1);

  // initialize the image
  src = image_create(rows, cols);

  // initialize matrices
  matrix_identity(&GTM);
  matrix_identity(&VTM);

  // set the View parameters
  point_set3D(&(view.vrp), 5, 5, -7.0);
  vector_set(&(view.vpn), -5, -5, 7);
  vector_set(&(view.vup), 0.0, 1.0, 0.0);
  view.d = 2.0;
  view.du = 1.6;
  view.dv = .9;
  view.f = 0.0;
  view.b = 15;
  view.screenx = cols;
  view.screeny = rows;
  matrix_setView3D(&VTM, &view);
  printf("view vector:");
  vector_print(&view.vrp, stdout);

  // set the shading to Gouraud
  ds = drawstate_create();

  point_copy(&(ds->viewer), &(view.vrp));
  // ds->shade = ShadeGouraud;
  ds->shade = ShadePhong;
  // ds->shade = ShadeFrame;

  // print out VTM
  printf("Final VTM: \n");
  matrix_print(&VTM, stdout);

  // Make a white floor
  floor = module_create();
  module_scale(floor, 5, 0.1, 5);
  module_color(floor, &White);
  module_bodyColor(floor, &Grey);
  module_surfaceColor(floor, &DkGrey);
  module_cube(floor, 1);

  // make a simple cube module
  cube = module_create();
  module_identity(cube);
  module_scale(cube, 3, 1, 2);

  // this would color the cube in ShadeConstant mode
  module_color(cube, &White);

  // set body and surface color values for ShadeGouraud mode
  module_bodyColor(cube, &DkGrey);
  module_surfaceColor(cube, &White);

  // the example cube is blue (Y/-Y), red (Z/-Z), yellow (X/-X)
  // these colors should be the body colors
  module_cube(cube, 1);

  scene = module_create();
  module_module(scene, floor);
  module_module(scene, cube);

  // manually add a light source to the Lighting structure
  // put it in the same place as the eye in world space
  light = lighting_create();
  lighting_add(light, LightAmbient, &DkGrey, NULL, NULL, 0, 0);
  lighting_add(light, LightPoint, &White, NULL, &(view.vrp), 0, 0);

  rt = rayTracer_create();

  matrix_identity(&GTM);
  // module_draw(scene, &VTM, &GTM, ds, light, src);
  module_drawRay(scene, &view, &VTM, &GTM, light, ds, rt, src);

  // write out the image
  // image_write(src, "test9aGouraund.ppm");
  image_write(src, "test9a.ppm");

  // free stuff here
  module_delete(scene);
  module_delete(cube);
  module_delete(floor);
  image_free(src);

  return (0);
}
