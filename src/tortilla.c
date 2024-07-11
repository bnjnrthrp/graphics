/**
 * Demo program that builds a fractal landscape and colors it according to the altitude of the points.
 *
 * Takes 3 command arguments: (int) random seed, (int) iteration number, (double) roughness scale
 *
 * Example: ../bin/tortilla 15 6 .5
 * Default arguments are a random seed (based on time(0)), 6 iterations, roughness .5
 *
 * For best results, play with different random seed values. In my program, 15 works well
 *
 * -Iterations are currently clipped to no more than 6. Increasing this starts to result in polygon edges being dropped from the program,
 * and you'll start to get missing polygons in certain frames (indicated by the console yelling at you "bad bad bad (your edges are not coming in pairs").
 * However, the image will still draw just fine for most cases. If you want to increase the clipping and see a more smooth image, then edit the program
 * at line 41 and increase the threshold to a higher iteration amount. Anything above 10 tends to run the risk of a segmentation fault.
 *
 * The roughness scale needs to be somewhere between 0.0 and 0.5 for best results. Anything above 0.6 starts to result in very jagged terrain that
 * doesn't seem realistic. Above 1.0 will result in runaway y values. 0.5 seems to be the best case for me.
 *
 *
 *
 * @author Benji Northrop
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "../include/Graphics.h"
#define M_PI 3.14159265358979323846

int main(int argc, char *argv[])
{
	int frame, seed, iterations;
	double roughness;
	DrawState *ds;
	Module *scene;
	View3D view;
	Matrix VTM, GTM;
	int rows = 300, cols = 400;
	Image *src = image_create(rows, cols);
	Point A, B, C;

	if (argc > 1)
	{
		seed = atoi(argv[1]);
	}
	else
		seed = time(0);
	srand48(seed);

	if (argc > 2)
	{
		iterations = atoi(argv[2]);

		// To experiment with more smooth shading, increase the threshold of clipping here
		if (iterations < 0 || iterations > 6)
		{
			iterations = 6;
		}
	}
	else
	{
		iterations = 6;
	}

	if (argc > 3)
	{
		roughness = atof(argv[3]);
	}
	else
		roughness = 0.5;

	// Clip the iterations to between 0-6

	ds = drawstate_create();
	// set up the drawstate
	// ds->shade = ShadeFrame;
	ds->shade = ShadeDepth;

	// create the triangle endpoints
	point_set3D(&A, 0.0, 0.0, 0.0); // first row, constant x, even spacing in z
	point_set3D(&B, 1.0, 0.0, 0.0);
	point_set3D(&C, 0.0, 0.0, 1.0);

	// create a pyramid with user provided sides
	scene = module_create();

	module_translate(scene, -0.5, 0.0, -0.5);

	module_scale(scene, 3.0, 1.0, 3.0);
	module_terrain(scene, iterations, roughness);

	// set up the view
	point_set3D(&(view.vrp), 0.0, 3.0, -5.0);
	vector_set(&(view.vpn), 0.0, -2.0, 3.0);
	vector_set(&(view.vup), 0.0, 1.0, 0.0);
	view.d = 1.5;
	view.du = 1.0;
	view.dv = 1.0 * (float)rows / (float)cols;
	view.screeny = rows;
	view.screenx = cols;
	view.f = 0.0;
	view.b = 10.0;

	matrix_setView3D(&VTM, &view);
	matrix_identity(&GTM);

	matrix_print(&VTM, stdout);

	// Create the animation by adjusting the GTM
	for (frame = 0; frame < 40; frame++)
	{
		char buffer[256];

		matrix_rotateY(&GTM, cos(M_PI / 30.0), sin(M_PI / 30.0));
		module_draw(scene, &VTM, &GTM, ds, NULL, src);

		sprintf(buffer, "tortilla-frame%03d.ppm", frame);
		image_write(src, buffer);
		image_reset(src);
	}

	// clean up
	image_free(src);

	module_delete(scene);
	free(ds);

	return (0);
}
