/*
  Benjamin Northrop

  Test lighting with a unit triangle
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
  int rows = 360;
  int cols = 640;
  Polygon p;
  Point pt[3];
  Vector N, V;

  Color White;
  Color Grey;
  Color DkGrey;
  Color colors[3];
  Color Cb;
  Color Cs;
  Color Red;

  DrawState *ds;
  View3D view;

  Lighting *light;

  color_set(&Red, 0.1, 0.0, 0.0);
  color_set(&White, 1.0, 1.0, 1.0);
  color_set(&Grey, 0.6, 0.62, 0.64);
  color_set(&DkGrey, 0.1, 0.1, 0.1);
  color_copy(&Cb, &Grey);
  color_copy(&Cs, &White);
  colors[0] = Grey;
  colors[1] = White;
  colors[2] = DkGrey;

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
  vector_set(&V, 5, 5, -7.0);

  // print out VTM
  printf("Final VTM: \n");
  matrix_print(&VTM, stdout);

  // make a simple triangle module
  point_set3D(&pt[0], 1.0, 100.0, 1.0);
  point_set3D(&pt[1], 1.0, 200.0, 2.0);
  point_set3D(&pt[2], 150.0, 150.0, 1.0);

  vector_calculateNormal(&N, &pt[0], &pt[1], &pt[2]);

  polygon_init(&p);
  polygon_set(&p, 3, pt);
  polygon_setColors(&p, 3, colors);

  // manually add a light source to the Lighting structure
  // put it in the same place as the eye in world space
  light = lighting_create();
  Point loc;
  point_set3D(&loc, 0, 10, 0);
  lighting_add(light, LightPoint, &Red, NULL, &loc, 0, 0);

  // set the shading to Gouraud
  ds = drawstate_create();

  drawstate_setBody(ds, Grey);
  drawstate_setSurface(ds, DkGrey);
  point_copy(&(ds->viewer), &(view.vrp));
  ds->shade = ShadeGouraud;
  //	ds->shade = ShadeFlat;

  // calculate the shading
  polygon_shade(&p, ds, light);
  polygon_drawShade(&p, src, ds, light);
  printf("drew polygon \n");

  // write out the image
  image_write(src, "test9e.ppm");

  // free stuff here
  polygon_clear(&p);
  image_free(src);

  return (0);
}
