/*
Uses Perlin noise to build topography on
*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "../include/Graphics.h"
#define M_PI 3.14159265358979323846

int main(int argc, char *argv[])
{
	Color blue, white, brown, sandstone, grey, lightBlue;
	DrawState *ds;
	Lighting *light;
	Module *scene;
	View3D view;
	Matrix VTM, GTM;
	int rows = 300, cols = 400;
	Image *src = image_create(rows, cols);

	color_set(&grey, .2, .2, .2);
	color_set(&brown, 0.55, 0.35, 0.0);
	color_set(&white, 1.0, 1.0, 1.0);
	color_set(&sandstone, .89, .80, .69);
	color_set(&blue, 0.0, 0.0, 1.0);
	color_set(&lightBlue, 68.0 / 255.0, 85.0 / 255.0, 90.0 / 255.0);

	// set up the view
	point_set3D(&(view.vrp), 0.0, 0.6, -5.0);
	vector_set(&(view.vpn), 0.0, -0.6, 5.0);
	vector_set(&(view.vup), 0.0, 1.0, 0.0);
	view.d = 1.0;
	view.du = 1.0;
	view.dv = 1.0 * rows / cols;
	view.screeny = rows;
	view.screenx = cols;
	view.f = 0.0;
	view.b = 10.0;

	matrix_setView3D(&VTM, &view);
	matrix_identity(&GTM);

	// create a pyramid with user provided sides
	scene = module_create();

	module_bodyColor(scene, &sandstone);
	module_surfaceColor(scene, &sandstone);
	module_surfaceCoeff(scene, 2.0);
	// Put in the 3 great pyramids
	// Khufu
	module_scale(scene, 1.0, .636, 1.0);
	module_translate(scene, -0.5, 0.0, 3.0);
	module_pyramid(scene, 4);

	// Khafre
	module_identity(scene);
	module_scale(scene, .94, .623, .94);
	module_pyramid(scene, 4);

	// Menkaure
	module_identity(scene);
	module_scale(scene, .44, .453, .44);
	module_translate(scene, 0.7, 0.0, -2.5);
	module_pyramid(scene, 4);

	// Henutsen, Meritites, Hetepheres
	module_identity(scene);
	module_scale(scene, .15, .15, .15);
	module_translate(scene, 0.35, 0.0, -2.8);
	module_pyramid(scene, 4);
	module_translate(scene, 0.1, 0.0, -0.15);
	module_pyramid(scene, 4);
	module_translate(scene, 0.1, 0.0, -0.15);
	module_pyramid(scene, 4);

	// Moon
	module_identity(scene);
	module_scale(scene, 0.5, 0.5, 0.5);
	module_translate(scene, .25, 2.0, 5.0);
	module_color(scene, &white);
	module_bodyColor(scene, &white);
	module_surfaceColor(scene, &white);
	module_surfaceCoeff(scene, 1.0);
	module_sphere(scene, 40);

	ds = drawstate_create();
	// set up the drawstate
	point_copy(&(ds->viewer), &(view.vrp));
	ds->shade = ShadePhong;

	light = lighting_create();
	lighting_add(light, LightPoint, &lightBlue, NULL, &(view.vrp), 0.0, 0.0);
	lighting_add(light, LightAmbient, &grey, NULL, NULL, 0.0, 0.0);

	matrix_print(&VTM, stdout);

	module_draw(scene, &VTM, &GTM, ds, light, src);

	image_write(src, "great-pyramid.ppm");

	// clean up
	image_free(src);

	module_delete(scene);
	free(ds);
	free(light);

	return (0);
}
