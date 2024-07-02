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
	Color blue, white, brown;
	DrawState *ds;
	Module *scene;
	View3D view;
	Matrix VTM, GTM;
	int rows = 300, cols = 400;
	Image *src = image_create(rows, cols);

	srand48(56);

	color_set(&brown, 0.55, 0.35, 0.0);
	color_set(&white, 1.0, 1.0, 1.0);
	color_set(&blue, 0.0, 0.0, 1.0);

	ds = drawstate_create();
	// set up the drawstate
	drawstate_setColor(ds, brown);

	// create a pyramid with user provided sides
	scene = module_create();

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
	module_sphere(scene, 40);

	// set up the view
	point_set3D(&(view.vrp), 0.0, 0.6, -5.0);
	vector_set(&(view.vpn), 0.0, 0.0, 1.0);
	vector_set(&(view.vup), 0.0, 1.0, 0.0);
	view.d = 1.0;
	view.du = 1.0;
	view.dv = 1.0 * rows / cols;
	view.screeny = rows;
	view.screenx = cols;
	view.f = 0.0;
	view.b = 6.0;

	matrix_setView3D(&VTM, &view);
	matrix_identity(&GTM);

	matrix_print(&VTM, stdout);

	module_draw(scene, &VTM, &GTM, ds, NULL, src);

	image_write(src, "great-pyramid.ppm");

	// clean up
	image_free(src);

	module_delete(scene);
	free(ds);

	return (0);
}
