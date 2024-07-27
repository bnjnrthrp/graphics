/*
  Bruce A. Maxwell
  Fall 2014

	Skeleton scanline fill algorithm

	Implementation completed by Benji Northrop
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Polygon.h"
#include "List.h"
#include "DrawState.h"

/********************
Scanline Fill Algorithm
********************/

// define the struct here, because it is local to only this file
typedef struct tEdge
{
	float x0, y0, z0;			 /* start point for the edge */
	float x1, y1, z1;			 /* end point for the edge */
	int yStart, yEnd;			 /* start row and end row */
	float xIntersect, dxPerScan; /* where the edge intersects the current scanline and how it changes */
								 /* we'll add more here later */
	float zIntersect, dzPerScan;
	Color cIntersect, dcPerScan;
	struct tEdge *next;
} Edge;

/*
	This is a comparison function that returns a value < 0 if a < b, a
	value > 0 if a > b, and 0 if a = b.  It uses the yStart field of the
	Edge structure.  It is used to sort the overall edge list.
 */
static int compYStart(const void *a, const void *b)
{
	Edge *ea = (Edge *)a;
	Edge *eb = (Edge *)b;

	return (ea->yStart - eb->yStart);
}

/*
	This is a comparison function that returns a value < 0 if a < b, a
	value > 0 if a > b, and 0 if a = b.  It uses the xIntersect field of the
	Edge structure.  It is used to sort the active edge list.
 */
static int compXIntersect(const void *a, const void *b)
{
	Edge *ea = (Edge *)a;
	Edge *eb = (Edge *)b;

	if (ea->xIntersect < eb->xIntersect)
		return (-1);
	else if (ea->xIntersect > eb->xIntersect)
		return (1);

	return (0);
}

/*
	Allocates, creates, fills out, and returns an Edge structure given
	the inputs.

	Current inputs are just the start and end location in image space.
	Eventually, the points will be 3D and we'll add color and texture
	coordinates.
 */
static Edge *makeEdgeRec(Point start, Point end, Image *src, Color c0, Color c1)
{
	Edge *edge;
	float dscan = end.val[1] - start.val[1];

	/******
				 Your code starts here
	******/
	// Null check src
	if (!src)
	{
		fprintf(stderr, "Invalid pointer to makeEdgeRec\n");
		exit(-1);
	}
	// Check if the starting row is below the image or the end row is
	// above the image and skip the edge if either is true
	if (start.val[1] > src->rows || end.val[1] < 0)
	{
		return NULL;
	}

	// allocate an edge structure and set the x0, y0, x1, y1 values
	edge = (Edge *)malloc(sizeof(Edge));
	edge->x0 = start.val[0];
	edge->y0 = start.val[1];
	edge->z0 = start.val[2];
	edge->x1 = end.val[0];
	edge->y1 = end.val[1];
	edge->z1 = end.val[2];
	// turn on an edge only if the edge starts in the top half of it or
	// the lower half of the pixel above it.  In other words, round the
	// start y value to the nearest integer and assign it to
	// edge->yStart.

	edge->yStart = (int)(edge->y0 + .5);

	// Turn off the edge if it starts in the lower half or the top half
	// of the next pixel.  In other words, round the end y value to the
	// nearest integer and subtract 1, assigning the result to
	// edge->yEnd.

	edge->yEnd = (int)(edge->y1 + .5) - 1;

	// Clip yEnd to the number of rows-1.

	if (edge->yEnd >= src->rows)
	{
		edge->yEnd = src->rows - 1;
	}

	printf("Color 1 is: (%.2f, %.2f, %.2f) and color 2 is (%.2f, %.2f, %.2f)\n", c0.c[0], c0.c[1], c0.c[2], c1.c[0], c1.c[1], c1.c[2]);
	// Calculate the slope, dxPerScan == dx / dy
	edge->dxPerScan = (edge->x1 - edge->x0) / dscan;
	edge->dzPerScan = (1.0 / edge->z1 - 1.0 / edge->z0) / dscan;
	edge->dcPerScan.c[0] = (c1.c[0] / end.val[2] - c0.c[0] / start.val[2]) / dscan;
	printf("c1.c[0] is %.2f, end.val[2] is %.2f, c0.c[0] is %.2f, start.val[2] is %.2f, dscan is %.2f\n", c1.c[0], end.val[2], c0.c[0], start.val[2], dscan);
	printf("The value is %.5f\n", (c1.c[0] / end.val[2] - c0.c[0] / start.val[2]) / dscan);
	edge->dcPerScan.c[1] = (c1.c[1] / end.val[2] - c0.c[1] / start.val[2]) / dscan;
	edge->dcPerScan.c[2] = (c1.c[2] / end.val[2] - c0.c[2] / start.val[2]) / dscan;
	printf("dc per scan is: (%.2f, %.2f, %.2f)\ndscan is %.2f\n", edge->dcPerScan.c[0], edge->dcPerScan.c[1], edge->dcPerScan.c[2], dscan);

	// Calculate xIntersect, adjusting for the fraction of the point in the pixel.
	// Scanlines go through the middle of pixels
	// Move the edge to the first scanline it crosses

	if (edge->y0 - (int)(edge->y0) <= .5)
	{
		// xIntersect = x0 + (.5 - (y0 - floor(y0)) * dx/dy
		edge->xIntersect = edge->x0 + (.5 - (edge->y0 - (int)(edge->y0))) * edge->dxPerScan;
		edge->zIntersect = (1.0 / edge->z0) + (.5 - (edge->y0 - (int)(edge->y0))) * edge->dzPerScan;
	}
	else
	{
		// xIntersect = x0 + (1.0 - y0 - floor(y0) + .5) * dx/dy
		edge->xIntersect = edge->x0 + (1.0 - (edge->y0 - (int)(edge->y0)) + .5) * edge->dxPerScan;
		edge->zIntersect = (1.0 / edge->z0) + (1.0 - (edge->y0 - (int)(edge->y0)) + .5) * edge->dzPerScan;
	}
	edge->cIntersect.c[0] = (c0.c[0] / start.val[2]);
	edge->cIntersect.c[1] = (c0.c[1] / start.val[2]);
	edge->cIntersect.c[2] = (c0.c[2] / start.val[2]);

	// adjust if the edge starts above the image
	// move the intersections down to scanline zero
	// if edge->y0 < 0
	if (edge->y0 < 0)
	{
		//   update xIntersect
		// xIntersect would be the new x when sloping from the negative y to 0. xI1 = xI0 + dx/dy * (0 - y0)
		edge->xIntersect = edge->xIntersect + (edge->dxPerScan) * (0.0 - edge->y0);
		edge->zIntersect = edge->zIntersect + (edge->dzPerScan) * (0.0 - edge->y0);
		edge->cIntersect.c[0] = edge->cIntersect.c[0] + (edge->dcPerScan.c[0]) * (0.0 - edge->y0);
		edge->cIntersect.c[1] = edge->cIntersect.c[1] + (edge->dcPerScan.c[1]) * (0.0 - edge->y0);
		edge->cIntersect.c[2] = edge->cIntersect.c[2] + (edge->dcPerScan.c[2]) * (0.0 - edge->y0);
		//   update y0
		edge->y0 = 0;
		//   update x0
		edge->x0 = edge->xIntersect; // since using Lower left index, x0 would be actually be at the xIntersect
		edge->z0 = 1.0 / edge->zIntersect;
		//   update yStart
		edge->yStart = (int)(edge->y0 + .5);
	}

	// check for really bad cases with steep slopes where xIntersect has gone beyond the end of the edge
	if ((edge->xIntersect > edge->x1 && edge->dxPerScan > 0) || (edge->xIntersect < edge->x1 && edge->dxPerScan < 0))
	{
		edge->xIntersect = edge->x1;
	}

	if ((edge->zIntersect > edge->z1 && edge->dzPerScan > 0) || (edge->zIntersect < edge->z1 && edge->dzPerScan < 0))
	{
		edge->zIntersect = edge->z1;
	}

	// return the newly created edge data structure
	return (edge);
}

/*
	Returns a list of all the edges in the polygon in sorted order by
	smallest row.
*/
static LinkedList *setupEdgeList(Polygon *p, Image *src)
{
	LinkedList *edges = NULL;
	Point v1, v2;
	Color c1, c2;
	int i;

	// create a linked list
	edges = ll_new();

	// walk around the polygon, starting with the last point
	v1 = p->vertex[p->nVertex - 1];
	printf("Trying to copy the colors into the polygon\n");
	color_copy(&c1, &(p->color[p->nVertex - 1]));
	printf("Copy the colors into the polygon\n");
	for (i = 0; i < p->nVertex; i++)
	{

		// the current point (i) is the end of the segment
		v2 = p->vertex[i];
		color_copy(&c2, &(p->color[i]));
		// if it is not a horizontal line
		if ((int)(v1.val[1] + 0.5) != (int)(v2.val[1] + 0.5))
		{
			Edge *edge;

			// if the first coordinate is smaller (top edge)
			if (v1.val[1] < v2.val[1])
				edge = makeEdgeRec(v1, v2, src, c1, c2);
			else
				edge = makeEdgeRec(v2, v1, src, c2, c1);

			// insert the edge into the list of edges if it's not null
			if (edge)
				ll_insert(edges, edge, compYStart);
			printf("Color 1 is: (%.2f, %.2f, %.2f) and color 2 is (%.2f, %.2f, %.2f)\n", c1.c[0], c1.c[1], c1.c[2], c2.c[0], c2.c[1], c2.c[2]);
		}
		v1 = v2;
	}

	// check for empty edges (like nothing in the viewport)
	if (ll_empty(edges))
	{
		ll_delete(edges, NULL);
		edges = NULL;
	}

	return (edges);
}

/*
	Draw one scanline of a polygon given the scanline, the active edges,
	a DrawState, the image, and some Lights (for Phong shading only).
 */
static void fillScan(int scan, LinkedList *active, Image *src, Color c, DrawState *ds)
{
	Edge *p1, *p2;
	int i, start, end;
	float currZ, dzPerCol;
	Color tc, currColor, dcPerCol;

	// loop over the list
	p1 = ll_head(active);
	while (p1)
	{
		// the edges have to come in pairs, draw from one to the next
		p2 = ll_next(active);
		if (!p2)
		{
			printf("bad bad bad (your edges are not coming in pairs)\n");
			break;
		}
		currZ = p1->zIntersect;
		color_copy(&currColor, &p1->cIntersect);
		dzPerCol = (p2->zIntersect - p1->zIntersect) / (double)(src->cols);
		color_set(&dcPerCol,
				  (p2->cIntersect.c[0] - p1->cIntersect.c[0]) / (double)(src->cols),
				  (p2->cIntersect.c[1] - p1->cIntersect.c[1]) / (double)(src->cols),
				  (p2->cIntersect.c[2] - p1->cIntersect.c[2]) / (double)(src->cols));
		// printf("cIntersect is: %.2f, %.2f, %.2f\n", p1->cIntersect.c[0], p1->cIntersect.c[1], p1->cIntersect.c[2]);
		// printf("dcPerCol is: %.5f, %.5f, %.5f\n", dcPerCol.c[0], dcPerCol.c[1], dcPerCol.c[2]);

		// printf("p2 intersect is %.5f, p1 intersect is %.5f, dzperCol is %.5f, currZ is %.2f\n", p2->zIntersect, p1->zIntersect, dzPerCol, currZ);
		// if the xIntersect values are the same, don't draw anything.
		// Just go to the next pair.
		if (p2->xIntersect == p1->xIntersect)
		{
			p1 = ll_next(active);
			continue;
		}

		/**** Your code goes here ****/
		// identify the starting column
		start = (int)(p1->xIntersect + .5);
		// clip to the left side of the image
		if (start < 0)
		{
			currZ += (0 - start) * dzPerCol;
			color_set(&currColor, (0 - start) * dcPerCol.c[0], (0 - start) * dcPerCol.c[1], (0 - start) * dcPerCol.c[2]);
			start = 0;
		}
		// identify the ending column
		end = (int)(p2->xIntersect + .5);
		// clip to the right side of the image
		if (end > src->cols)
		{
			end = src->cols;
		}
		// loop from start to end and color in the pixels
		for (i = start; i < end; i++)
		{
			// printf("Comparing %.5f to %.5f\n", currZ, image_getz(src, scan, i));
			if (currZ > image_getz(src, scan, i))
			{
				// printf("currZ was higher\n");
				if (ds->shade == ShadeConstant)
				{
					image_setColor(src, scan, i, ds->color);
				}
				else if (ds->shade == ShadeDepth)
				{
					float scale = 1.4;
					float depth = 1.0 - (1.0 / currZ);
					color_copy(&tc, &(ds->color));
					// for test8a
					// color_set(&tc, depth, depth, depth);
					// For cubism/all other depth scaling
					color_set(&tc, scale * (ds->color.c[0] * depth), scale * (ds->color.c[1] * depth), scale * (ds->color.c[2] * depth));
					image_setColor(src, scan, i, tc);
				}
				else if (ds->shade == ShadeGouraud)
				{
					// printf("Color is (%.2f, %.2f, %.2f)\n", currColor.c[0], currColor.c[1], currColor.c[2]);
					color_set(&tc, currColor.c[0] / (currZ), currColor.c[1] / (currZ), currColor.c[2] / (currZ));
					// printf("Drawing Gouraud shading, color: (%.5f, %.5f, %.5f)\n", tc.c[0], tc.c[1], tc.c[2]);
					image_setColor(src, scan, i, tc);
				}
				else
				{
					//	printf("An invalid drawstate was found\n");
					image_setColor(src, scan, i, ds->color);
				}
				image_setz(src, scan, i, currZ);
				// printf("Setting the image z to %.5f\n", currZ);
				// printf("new z value at %.5f\n", image_getz(src, scan, i));
			}
			else
			{
				if (i % 10 == 0)
				{
					// printf("Not drawing this pixel, current z is %.2f and image z is %.2f\n", currZ, image_getz(src, scan, i));
				}
			}
			color_set(&currColor, currColor.c[0] + dcPerCol.c[0], currColor.c[1] + dcPerCol.c[1], currColor.c[2] + dcPerCol.c[2]);
			// printf("currZ is %.5f, dzPerCol is %.5f\n", currZ, dzPerCol);
		}
		// move ahead to the next pair of edges
		p1 = ll_next(active);
	}

	return;
}

/*
	 Process the edge list, assumes the edges list has at least one entry
*/
static int processEdgeList(LinkedList *edges, Image *src, DrawState *ds)
{
	LinkedList *active = NULL;
	LinkedList *tmplist = NULL;
	LinkedList *transfer = NULL;
	Edge *current;
	Edge *tedge;
	int scan = 0;

	active = ll_new();
	tmplist = ll_new();

	// get a pointer to the first item on the list and reset the current pointer
	current = ll_head(edges);

	// start at the first scanline and go until the active list is empty
	for (scan = current->yStart; scan < src->rows; scan++)
	{

		// grab all edges starting on this row
		while (current != NULL && current->yStart == scan)
		{
			ll_insert(active, current, compXIntersect);
			current = ll_next(edges);
		}
		// current is either null, or the first edge to be handled on some future scanline

		if (ll_empty(active))
		{
			break;
		}

		// if there are active edges
		// fill out the scanline
		fillScan(scan, active, src, ds->color, ds);

		// remove any ending edges and update the rest
		for (tedge = ll_pop(active); tedge != NULL; tedge = ll_pop(active))
		{

			// keep anything that's not ending
			if (tedge->yEnd > scan)
			{
				float a = 1.0;

				// update the edge information with the dPerScan values
				tedge->xIntersect += tedge->dxPerScan;
				tedge->zIntersect += tedge->dzPerScan;
				// adjust in the case of partial overlap
				if (tedge->dxPerScan < 0.0 && tedge->xIntersect < tedge->x1)
				{
					a = (tedge->xIntersect - tedge->x1) / tedge->dxPerScan;
					tedge->xIntersect = tedge->x1;
					tedge->zIntersect = tedge->z1;
				}
				else if (tedge->dxPerScan > 0.0 && tedge->xIntersect > tedge->x1)
				{
					a = (tedge->xIntersect - tedge->x1) / tedge->dxPerScan;
					tedge->xIntersect = tedge->x1;
					tedge->zIntersect = tedge->z1;
				}

				ll_insert(tmplist, tedge, compXIntersect);
			}
		}

		transfer = active;
		active = tmplist;
		tmplist = transfer;
	}

	// get rid of the lists, but not the edge records
	ll_delete(active, NULL);
	ll_delete(tmplist, NULL);

	return (0);
}

/*
	Draws a filled polygon of the specified color into the image src.
 */
void polygon_drawFill(Polygon *p, Image *src, Color c)
{
	DrawState ds;
	drawstate_setColor(&ds, c);
	ds.shade = ShadeConstant;
	polygon_drawShade(p, src, &ds, NULL);
}

/*
	Draws a filled polygon of the specified color into the image src.
 */
void polygon_drawShade(Polygon *p, Image *src, DrawState *ds, Lighting *light)
{
	if (!p || !src || !ds)
	{
		fprintf(stderr, "Invalid pointer sent to polygon_drawShade\n");
		exit(-1);
	}
	LinkedList *edges = NULL;

	// Add the colors to the polygon
	if (ds->shade != ShadeGouraud)
	{
		Color tmp[p->nVertex];
		for (int i = 0; i < p->nVertex; i++)
		{
			printf("Adding color %d to the polygon\n", i);
			color_copy(&(tmp[i]), &(ds->color));
		}
		polygon_setColors(p, p->nVertex, tmp);
	}

	// set up the edge list
	edges = setupEdgeList(p, src);
	if (!edges)
	{
		return;
	}
	// process the edge list (should be able to take an arbitrary edge list)
	processEdgeList(edges, src, ds);
	// clean up
	ll_delete(edges, (void (*)(const void *))free);

	return;
}

/****************************************
End Scanline Fill
*****************************************/
