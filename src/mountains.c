/**
 *Demo program that builds mountains and clouds using a Perlin noise generator.
 *
 * @author Benji Northrop
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
	Color blue, white, brown;
	DrawState *ds;
	Module *scene;
	View3D view;
	Matrix VTM, GTM;
	int rows = 300, cols = 400;
	Image *src = image_create(rows, cols);
	float dx, dy, value, val1, val2, val3;
	float noiseValue[rows * cols];

	srand48(56);

	color_set(&brown, 0.55, 0.35, 0.0);
	color_set(&white, 1.0, 1.0, 1.0);
	color_set(&blue, 0.0, 0.0, 1.0);

	ds = drawstate_create();
	// set up the drawstate
	drawstate_setColor(ds, white);

	// create a pyramid with user provided sides
	scene = module_create();

	// Create a map over the image space with Perlin noise values
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			dx = (float)i / (float)cols;
			dy = (float)j / (float)rows;
			value = perlin(dx * 30, dy * 30, src->z[i * src->cols + j]);
			value = (value + 1) / 2; // Removes the negative values
			noiseValue[i * rows + j] = value;
		}
	}

	for (int i = 0; i < 30; i++)
	{
		// Somewhat random sampling of the Perlin noise values
		val1 = noiseValue[i * rows * cols / 80];
		val2 = noiseValue[(i * rows * cols + 10) / 80];
		val3 = noiseValue[(i * rows * cols + 20) / 80];
		module_identity(scene);
		module_scale(scene, (val1 + val3) / 2.0, val2, (val3 + val2) / 2.0);			// Scale by the same amount
		module_translate(scene, (0.5 - drand48()) * 5.0, 0.0, (0.5 - drand48()) * 5.0); // Round down for mountains
		module_color(scene, &brown);
		module_pyramid(scene, drand48() * 50 + 5);

		module_identity(scene);
		module_scale(scene, 0.7 * val1, .7 * val2, 0.5 * val3);							// Scale by random amount
		module_translate(scene, (0.5 - drand48()) * 5.0, 2.0, (0.5 - drand48()) * 5.0); // Raise the clouds
		module_color(scene, &blue);
		for (int j = 0; j < 3; j++)
		{
			module_translate(scene, val3, val2, val1);
			module_sphere(scene, val1 * 50); // Draw several spheres in the local area for clouds
		}
	}

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
	for (frame = 0; frame < 40; frame++)
	{
		char buffer[256];

		matrix_rotateY(&GTM, cos(M_PI / 30.0), sin(M_PI / 30.0));
		module_draw(scene, &VTM, &GTM, ds, NULL, src);

		sprintf(buffer, "mountains-frame%03d.ppm", frame);
		image_write(src, buffer);
		image_reset(src);
	}

	// clean up
	image_free(src);

	module_delete(scene);
	free(ds);

	return (0);
}
