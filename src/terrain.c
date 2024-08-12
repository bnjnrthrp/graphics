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

void xwing_build(Module *md);
void xwing_build(Module *md)
{
	Module *scene;
	Module *engine;
	Module *wing;
	Module *wings;
	Module *laser;
	Module *body;
	Polygon p;
	Point pt[4];
	Vector n[4];
	Color Flame, Red, Grey, Dark, White;
	float bodyWidth = 2.0;
	int i;

	if (!md)
	{
		return;
	}

	color_set(&White, 1.0, 1.0, 1.0);
	color_set(&Flame, 1.0, 0.7, 0.2);
	color_set(&Red, 1.0, 0.2, 0.1);
	color_set(&Grey, 0.6, 0.65, 0.67);
	color_set(&Dark, 0.2, 0.2, 0.2);

	// engine
	engine = module_create();
	module_scale(engine, 1.3, 6, 1.3);
	module_rotateX(engine, 0, 1);
	module_cylinder(engine, 10);
	module_scale(engine, .8, .8, 1);
	module_color(engine, &Flame);
	module_cylinder(engine, 10);

	// laser
	laser = module_create();
	module_scale(laser, 0.5, 5, 0.5);
	module_rotateX(laser, 0, 1);
	module_cylinder(laser, 6);
	module_scale(laser, 0.4, 0.4, 1.0);
	module_translate(laser, 0, 0, 4.5);
	module_color(laser, &Red);
	module_cylinder(laser, 10);

	// wing
	wing = module_create();
	polygon_init(&p);
	point_set3D(&pt[0], 0.0, 0.0, 0.0);
	point_set3D(&pt[1], 0.0, 0.0, 5.0);
	point_set3D(&pt[2], 15.0, 0.0, 3.0);
	point_set3D(&pt[3], 15.0, 0.0, 0.0);
	polygon_set(&p, 4, pt);
	for (i = 0; i < 4; i++)
		vector_set(&(n[i]), 0, -1, 0);
	polygon_setNormals(&p, 4, n);
	module_polygon(wing, &p);
	module_translate(wing, 0, 0.5, 0);
	for (i = 0; i < 4; i++)
		vector_set(&(n[i]), 0, 1, 0);
	polygon_setNormals(&p, 4, n);
	module_polygon(wing, &p);

	module_identity(wing);
	module_translate(wing, 3, 1.6, -1);
	module_module(wing, engine);

	module_identity(wing);
	point_set3D(&pt[0], 15.0, 0.0, 3.0);
	point_set3D(&pt[1], 15.0, 0.0, 0.0);
	point_set3D(&pt[2], 15.0, 0.5, 0.0);
	point_set3D(&pt[3], 15.0, 0.5, 3.0);
	polygon_set(&p, 4, pt);
	for (i = 0; i < 4; i++)
		vector_set(&(n[i]), 1, 0, 0);
	polygon_setNormals(&p, 4, n);
	module_polygon(wing, &p);

	point_set3D(&pt[0], 15.0, 0.0, 0.0);
	point_set3D(&pt[1], 0.0, 0.0, 0.0);
	point_set3D(&pt[2], 0.0, 0.5, 0.0);
	point_set3D(&pt[3], 15.0, 0.5, 0.0);
	polygon_set(&p, 4, pt);
	for (i = 0; i < 4; i++)
		vector_set(&(n[i]), 0, 0, -1);
	polygon_setNormals(&p, 4, n);
	module_polygon(wing, &p);

	point_set3D(&pt[0], 15.0, 0.0, 3.0);
	point_set3D(&pt[1], 15.0, 0.5, 3.0);
	point_set3D(&pt[2], 0.0, 0.5, 5.0);
	point_set3D(&pt[3], 0.0, 0.0, 5.0);
	polygon_set(&p, 4, pt);
	for (i = 0; i < 4; i++)
		vector_set(&(n[i]), 2, 0, 15);
	polygon_setNormals(&p, 4, n);
	module_polygon(wing, &p);

	module_translate(wing, 15, 0.25, 0);
	module_module(wing, laser);

	// 4 wings
	wings = module_create();
	module_bodyColor(wings, &Grey);
	module_rotateZ(wings, cos(0.3), sin(0.3));
	module_translate(wings, bodyWidth, 0, 0);
	module_module(wings, wing);

	module_identity(wings);
	module_scale(wings, 1, -1, 1);
	module_rotateZ(wings, cos(-0.3), sin(-0.3));
	module_translate(wings, bodyWidth, 0, 0);
	module_module(wings, wing);

	module_identity(wings);
	module_scale(wings, -1, 1, 1);
	module_rotateZ(wings, cos(-0.3), sin(-0.3));
	module_translate(wings, -bodyWidth, 0, 0);
	module_module(wings, wing);

	module_identity(wings);
	module_scale(wings, -1, -1, 1);
	module_rotateZ(wings, cos(0.3), sin(0.3));
	module_translate(wings, -bodyWidth, 0, 0);
	module_module(wings, wing);

	// body
	body = module_create();
	module_bodyColor(body, &White);
	module_surfaceColor(body, &Dark);

	module_module(body, wings);

	module_scale(body, bodyWidth, bodyWidth, 8);
	module_translate(body, 0, 0, 3);
	module_cube(body, 1);

	module_identity(body);
	point_set3D(&pt[0], bodyWidth, bodyWidth, 12);
	point_set3D(&pt[1], bodyWidth, -bodyWidth, 12);
	point_set3D(&pt[2], bodyWidth * 0.5, -bodyWidth * 0.3, 35);
	point_set3D(&pt[3], bodyWidth * 0.5, bodyWidth * 0.3, 35);
	polygon_set(&p, 4, pt);
	for (i = 0; i < 4; i++)
		vector_set(&(n[i]), 23, 0, 0.5 * bodyWidth);
	polygon_setNormals(&p, 4, n);
	module_polygon(body, &p);

	point_set3D(&pt[0], -bodyWidth, bodyWidth, 12);
	point_set3D(&pt[1], -bodyWidth, -bodyWidth, 12);
	point_set3D(&pt[2], -bodyWidth * 0.5, -bodyWidth * 0.3, 35);
	point_set3D(&pt[3], -bodyWidth * 0.5, bodyWidth * 0.3, 35);
	polygon_set(&p, 4, pt);
	for (i = 0; i < 4; i++)
		vector_set(&(n[i]), -23, 0, 0.5 * bodyWidth);
	polygon_setNormals(&p, 4, n);
	module_polygon(body, &p);

	point_set3D(&pt[0], -bodyWidth, bodyWidth, 12);
	point_set3D(&pt[1], bodyWidth, bodyWidth, 12);
	point_set3D(&pt[2], bodyWidth * 0.5, bodyWidth * 0.3, 35);
	point_set3D(&pt[3], -bodyWidth * 0.5, bodyWidth * 0.3, 35);
	polygon_set(&p, 4, pt);
	for (i = 0; i < 4; i++)
		vector_set(&(n[i]), 0, 23, 0.5 * bodyWidth);
	polygon_setNormals(&p, 4, n);
	module_polygon(body, &p);

	point_set3D(&pt[0], -bodyWidth, -bodyWidth, 12);
	point_set3D(&pt[1], bodyWidth, -bodyWidth, 12);
	point_set3D(&pt[2], bodyWidth * 0.5, -bodyWidth * 0.3, 35);
	point_set3D(&pt[3], -bodyWidth * 0.5, -bodyWidth * 0.3, 35);
	polygon_set(&p, 4, pt);
	for (i = 0; i < 4; i++)
		vector_set(&(n[i]), 0, -23, 0.5 * bodyWidth);
	polygon_setNormals(&p, 4, n);
	module_polygon(body, &p);

	point_set3D(&pt[0], -bodyWidth * 0.5, bodyWidth * 0.3, 35);
	point_set3D(&pt[1], bodyWidth * 0.5, bodyWidth * 0.3, 35);
	point_set3D(&pt[2], bodyWidth * 0.5, -bodyWidth * 0.3, 35);
	point_set3D(&pt[3], -bodyWidth * 0.5, -bodyWidth * 0.3, 35);
	polygon_set(&p, 4, pt);
	for (i = 0; i < 4; i++)
		vector_set(&(n[i]), 0, 0, 1);
	polygon_setNormals(&p, 4, n);
	module_polygon(body, &p);

	// scene with three ships (body module)
	scene = module_create();
	module_scale(scene, .7, .7, .7);
	module_rotateZ(scene, cos(0.5), sin(0.5));
	module_rotateY(scene, cos(-0.1), sin(-0.1));
	module_rotateX(scene, cos(0.2), sin(0.2));
	module_translate(scene, -15, -10, -10);
	module_module(scene, body);

	module_identity(scene);
	module_scale(scene, .7, .7, .7);
	module_rotateZ(scene, cos(0.3), sin(0.3));
	module_rotateY(scene, cos(-0.05), sin(-0.05));
	module_rotateX(scene, cos(0.2), sin(0.2));
	module_translate(scene, 5, -1, -50);
	module_module(scene, body);

	module_identity(scene);
	module_scale(scene, 0.7, 0.7, 0.7);
	module_rotateZ(scene, cos(0.1), sin(0.1));
	module_rotateX(scene, cos(0.2), sin(0.2));
	module_translate(scene, -35, -3, -60);
	module_module(scene, body);

	module_module(md, scene);
	// free the polygon data
	polygon_clear(&p);
}

int main(int argc, char *argv[])
{
	int frame, seed, iterations;
	double roughness;
	DrawState *ds;
	Module *scene;
	View3D view;
	Lighting *light;
	Matrix VTM, GTM;
	int rows = 300, cols = 400;
	Image *src = image_create(rows, cols);
	Point A, B, C;
	Color White, Grey;

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

	color_set(&White, 1, 1, 1);
	color_set(&Grey, .5, .5, .5);

	ds = drawstate_create();
	// set up the drawstate
	// ds->shade = ShadeFrame;
	ds->shade = ShadeGouraud;

	// create the triangle endpoints
	point_set3D(&A, 0.0, 0.0, 0.0); // first row, constant x, even spacing in z
	point_set3D(&B, 1.0, 0.0, 0.0);
	point_set3D(&C, 0.0, 0.0, 1.0);

	// create a pyramid with user provided sides
	scene = module_create();

	module_translate(scene, -0.5, 0.0, -0.5);

	module_scale(scene, 3.0, 1.0, 3.0);
	module_terrain(scene, ds, iterations, roughness);

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

	light = lighting_create();
	lighting_add(light, LightAmbient, &Grey, NULL, NULL, 0, 0);
	lighting_add(light, LightPoint, &White, NULL, &(view.vrp), 0, 0);

	matrix_print(&VTM, stdout);

	// Create the animation by adjusting the GTM
	for (frame = 0; frame < 40; frame++)
	{
		char buffer[256];

		matrix_rotateY(&GTM, cos(M_PI / 30.0), sin(M_PI / 30.0));
		module_draw(scene, &VTM, &GTM, ds, light, src);

		sprintf(buffer, "terrain-frame%03d.ppm", frame);
		image_write(src, buffer);
		image_reset(src);
	}

	// clean up
	image_free(src);

	module_delete(scene);
	free(ds);

	return (0);
}
