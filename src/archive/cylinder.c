/*
	Creates a spinning animation of a cylinder.

	Original cylinder code provided by instructor Bruce Maxwell
*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "../include/Graphics.h"
#define M_PI 3.14159265358979323846

int main(int argc, char *argv[])
{
	int i, frame;
	Color blue, white;
	DrawState *ds;
	Module *cylinder;
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

	ds = drawstate_create();
	// set up the drawstate
	drawstate_setColor(ds, white);

	// create a module with 12 section cylinder
	scene = module_create();
	module_translate(scene, -2.0, 0.0, .4);
	module_cylinder(scene, divisions);

	module_identity(scene);
	module_translate(scene, 1.0, 0.0, .4);
	module_cylinder(scene, 4);

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

	matrix_print(&VTM, stdout);

	// Create the animation by adjusting the GTM
	for (frame = 0; frame < 10; frame++)
	{
		char buffer[256];

		matrix_rotateY(&GTM, cos(M_PI / 30.0), sin(M_PI / 30.0));
		module_draw(scene, &VTM, &GTM, ds, NULL, src);

		sprintf(buffer, "cylinder-80-frame%03d.ppm", frame);
		image_write(src, buffer);
		image_reset(src);
	}

	// clean up
	image_free(src);

	module_delete(scene);
	free(ds);

	return (0);
}
