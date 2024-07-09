#ifndef SCANLINE_H
#define SCANLINE_H

#include "Polygon.h"
#include "Image.h"
#include "list.h"
#include "DrawState.h"

static int compYStart(const void *a, const void *b);
static int compXIntersect(const void *a, const void *b);
static Edge *makeEdgeRec(Point start, Point end, Image *src);
static LinkedList *setupEdgeList(Polygon *p, Image *src);
static void fillScan(int scan, LinkedList *active, Image *src, Color c);
static int processEdgeList(LinkedList *edges, Image *src, Color c, DrawState *ds);
#endif // SCANLINE_H
