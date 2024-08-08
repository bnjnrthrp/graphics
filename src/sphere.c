/*
	Demo code that creates a few spheres.

	Takes a single command line argument to specify the number of divisions of the sphere. (i.e. how many points to build a unit sphere)
	More divisions = more sphere-like
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
	Color blue, white, gray;
	DrawState *ds;
	Lighting *light;
	Module *scene;
	View3D view;
	Matrix VTM, GTM;
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
	color_set(&blue, 0.0, 0.0, 1.0);
	color_set(&gray, .4, .4, .4);

	ds = drawstate_create();
	// set up the drawstate
	drawstate_setColor(ds, white);
	ds->shade = ShadeGouraud;

	// create a pyramid with user provided sides
	scene = module_create();
	module_bodyColor(scene, &blue);
	module_surfaceColor(scene, &white);
	module_surfaceCoeff(scene, 1.5);
	module_scale(scene, 0.7, 0.7, 0.7);
	module_sphere(scene, divisions);

	module_identity(scene);
	module_scale(scene, 0.7, 0.7, 0.7);
	module_translate(scene, -1.1, -1.1, 0.0);
	module_sphere(scene, 3);

	module_identity(scene);
	module_scale(scene, 0.7, 0.7, 0.7);
	module_translate(scene, 1.1, 1.1, -0.5);
	module_sphere(scene, 4);

	module_identity(scene);
	module_scale(scene, 0.7, 0.7, 0.7);
	module_translate(scene, -1.1, 1.1, -0.5);
	module_sphere(scene, 12);

	module_identity(scene);
	module_scale(scene, 0.7, 0.7, 0.7);
	module_translate(scene, 1.1, -1.1, -0.5);
	module_sphere(scene, 20);

	// set up the view
	point_set3D(&(view.vrp), 0.0, 0.7, 5.0);
	vector_set(&(view.vpn), 0.0, -0.7, -5.0);
	vector_set(&(view.vup), 0.0, 1.0, 0.0);
	view.d = 1.0;
	view.du = 1.0;
	view.dv = 1.0 * rows / cols;
	view.screeny = rows;
	view.screenx = cols;
	view.f = 0.0;
	view.b = 30.0;

	matrix_setView3D(&VTM, &view);
	matrix_identity(&GTM);

	matrix_print(&VTM, stdout);

	light = lighting_create();
	lighting_add(light, LightAmbient, &gray, NULL, &(view.vrp), 0.0, 0.0);
	lighting_add(light, LightPoint, &white, NULL, &(view.vrp), 0.0, 0.0);

	// module_draw(scene, &VTM, &GTM, ds, light, src);
	// image_write(src, "spheres.ppm");
	// Create the animation by adjusting the GTM
	for (frame = 0; frame < 40; frame++)
	{
		char buffer[256];

		matrix_rotateY(&GTM, cos(M_PI / 30.0), sin(M_PI / 30.0));
		module_draw(scene, &VTM, &GTM, ds, light, src);
		sprintf(buffer, "sphere-frame%03d.ppm", frame);
		image_write(src, buffer);
		image_reset(src);
	}
	printf("Images complete\n");
	// clean up
	image_free(src);

	module_delete(scene);
	free(ds);

	return (0);
}
