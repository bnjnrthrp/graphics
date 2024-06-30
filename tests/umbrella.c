/*
	Bruce A Maxwell
	Fall 2014
	CS 351

	Creates a spinning animation of three pairs of 3D Bezier curves

	Accepts one command line parameter, which is the number of
	subdivisions to execute.
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
	Color blue, green, purple, red, white;
	Point p[4];
	BezierCurve bc;
	DrawState ds;
	Module *curveA;
	Module *curveB;
	Module *curveC;
	Module *curves;
	View3D view;
	Matrix VTM, GTM;
	int divisions = 4;
	int rows = 300, cols = 400;
	Image *src = image_create(rows, cols);

	// grab the command line argument, if one exists
	if (argc > 1)
	{
		int tmp = atoi(argv[1]);
		if (tmp >= 0 && tmp < 10)
			divisions = tmp;
	}
	printf("Creating umbrella with %d subdivisions\n", divisions);

	color_set(&white, 1.0, 1.0, 1.0);
	color_set(&blue, .1, .2, .8);
	color_set(&green, .2, 0.7, 0.3);
	color_set(&purple, 0.6, 0.1, 0.7);
	color_set(&red, 0.75, 0.3, 0.3);

	// set one curve
	point_set3D(&p[0], 0.0, 1.0, 0.0);
	point_set3D(&p[1], 0.2, 0.9, 0.0);
	point_set3D(&p[2], 0.8, 0.9, 0.0);
	point_set3D(&p[3], 1.0, 0.6, 0.0);
	bezierCurve_set(&bc, p);

	// put the curve into a module
	curveA = module_create();
	module_color(curveA, &blue);
	module_bezierCurve(curveA, &bc);

	// set the second curve
	point_set3D(&p[0], 1.0, 0.6, 0.0);
	point_set3D(&p[1], cos(15 * M_PI / 180), 0.5, sin(15 * M_PI / 180));
	point_set3D(&p[2], cos(30 * M_PI / 180), 0.5, sin(30 * M_PI / 180));
	point_set3D(&p[3], cos(45 * M_PI / 180), 0.6, sin(45 * M_PI / 180));
	bezierCurve_set(&bc, p);

	// put the curve into a module
	curveB = module_create();
	module_color(curveB, &green);
	module_bezierCurve(curveB, &bc);

	// set the third curve
	point_set3D(&p[0], 0.0, 1.0, 0.0);
	point_set3D(&p[1], 0.0, -0.1, 0.0);
	point_set3D(&p[2], 0.2, 0.0, 0.0);
	point_set3D(&p[3], 0.2, 0.2, 0.0);
	bezierCurve_set(&bc, p);

	// create a module with six curves
	curves = module_create();
	for (i = 0; i < 8; i++)
	{
		module_module(curves, curveA);
		module_module(curves, curveB);
		module_rotateY(curves, cos(2.0 * M_PI / 8.0), sin(2.0 * M_PI / 8.0));
	}

	curveC = module_create();
	module_color(curveC, &red);
	module_bezierCurve(curveC, &bc);
	module_module(curves, curveC);

	// set up the drawstate
	drawstate_setColor(&ds, white);

	// set up the view
	point_set3D(&(view.vrp), 0.0, 0.5, -3.0);
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
		module_draw(curves, &VTM, &GTM, &ds, NULL, src);

		sprintf(buffer, "umbrella-frame%03d.ppm", frame);
		image_write(src, buffer);
		image_reset(src);
	}

	// clean up
	image_free(src);

	module_delete(curveA);
	module_delete(curveB);
	module_delete(curveC);
	module_delete(curves);

	return (0);
}
