/*
	Demo code that creates a few pyramids.

	Takes a single command line argument to specify the number of edges on the base of the pyramid.
	@author Benji Northrop
*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "../include/Graphics.h"
#define M_PI 3.14159265358979323846

int main(int argc, char *argv[])
{
	int frame;
	Color blue, white, sandstone;
	DrawState *ds;
	Module *scene;
	View3D view;
	Matrix VTM, GTM;
	Lighting *light;
	int divisions = 12;
	int rows = 300, cols = 400;
	Image *src = image_create(rows, cols);

	// grab the command line argument, if one exists
	if (argc > 1)
	{
		int tmp = atoi(argv[1]);
		if (tmp >= 0 && tmp < 100)
			divisions = tmp;
	}

	color_set(&white, 1.0, 1.0, 1.0);
	color_set(&sandstone, .89, .80, .69);
	color_set(&blue, 0.0, 0.0, 1.0);

	ds = drawstate_create();
	// set up the drawstate
	drawstate_setColor(ds, white);

	// create a pyramid with user provided sides
	scene = module_create();
	module_translate(scene, -2.0, 0.0, .4);
	module_bodyColor(scene, &sandstone);
	module_surfaceColor(scene, &sandstone);
	module_surfaceCoeff(scene, .5);
	module_pyramid(scene, divisions);

	module_identity(scene);
	module_translate(scene, 1.0, 0.0, .4);
	module_pyramid(scene, 3);

	module_identity(scene);
	module_translate(scene, -0.5, 0.0, -1.5);
	module_pyramid(scene, 100);

	module_identity(scene);
	module_translate(scene, 0.0, 2.0, 0.0);
	module_pyramid(scene, 4);

	// set up the view
	point_set3D(&(view.vrp), 0.0, 1.3, -5.0);
	vector_set(&(view.vpn), 0.0, 0.0, 1.0);
	vector_set(&(view.vup), 0.0, 1.0, 0.0);
	view.d = 1.0;
	view.du = 1.0;
	view.dv = 1.0 * rows / cols;
	view.screeny = rows;
	view.screenx = cols;
	view.f = 0.0;
	view.b = 3.0;

	matrix_setView3D(&VTM, &view);
	matrix_identity(&GTM);

	light = lighting_create();
	lighting_add(light, LightPoint, &white, NULL, &(view.vrp), 0.0, 0.0);

	matrix_print(&VTM, stdout);

	module_draw(scene, &VTM, &GTM, ds, light, src);

	image_write(src, "pyramid.ppm");

	// // Create the animation by adjusting the GTM
	// for (frame = 0; frame < 40; frame++)
	// {
	// 	char buffer[256];

	// 	matrix_rotateY(&GTM, cos(M_PI / 30.0), sin(M_PI / 30.0));
	// 	module_draw(scene, &VTM, &GTM, ds, NULL, src);

	// 	sprintf(buffer, "pyramid-frame%03d.ppm", frame);
	// 	image_write(src, buffer);
	// 	image_reset(src);
	// }

	// clean up
	image_free(src);

	module_delete(scene);
	free(light);
	free(ds);

	return (0);
}
