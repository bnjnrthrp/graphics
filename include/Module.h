/**
 * This class represents a module for 2D/3D modeling
 * @author Benji Northrop
 */
#ifndef MODULE_H
#define MODULE_H

#include "Bezier.h"
#include "Color.h"
#include "DrawState.h"
#include "Lighting.h"
#include "Line.h"
#include "Matrix.h"
#include "Point.h"
#include "Polyline.h"
#include "Polygon.h"

/**
 * typedef enum to support polymorphism in what type of object the Element node holds.
 */
typedef enum ObjectType
{
    ObjNone,
    ObjBezier,
    ObjLine,
    ObjPoint,
    ObjPolyline,
    ObjPolygon,
    ObjIdentity,
    ObjMatrix,
    ObjColor,
    ObjBodyColor,
    ObjSurfaceColor,
    ObjSurfaceCoeff,
    ObjLight,
    ObjModule
} ObjectType;

/**
 * This union allows polymorphism when storing a type of object in an Element node.
 */
typedef union Object
{
    BezierCurve bezierCurve;
    Point point;
    Line line;
    Polyline polyline;
    Polygon polygon;
    Matrix matrix;
    Color color;
    float coeff;
    void *module;
} Object;

/**
 * An Element is a node in the module linked list struct
 */
typedef struct Element
{
    ObjectType type;
    Object obj;
    void *next;
} Element;

/**
 * A module is a linked list that contains the procedures for creating some kind of scene, object, etc.
 */
typedef struct Module
{
    Element *head; // pointer to the head of the linked list
    Element *tail; // keep around a pointer to the last object
} Module;

Element *element_create(void);
Element *element_init(ObjectType type, void *obj);
void element_delete(Element *e);
Module *module_create(void);
void module_clear(Module *md);
void module_delete(Module *md);
void module_insert(Module *md, Element *e);
void module_module(Module *md, Module *sub);
void module_point(Module *md, Point *p);
void module_line(Module *md, Line *p);
void module_polyline(Module *md, Polyline *p);
void module_polygon(Module *md, Polygon *p);
void module_identity(Module *md);
void module_translate2D(Module *md, double tx, double ty);
void module_scale2D(Module *md, double sx, double sy);
void module_rotateZ(Module *md, double cth, double sth);
void module_shear2D(Module *md, double shx, double shy);
void module_draw(Module *md, Matrix *VTM, Matrix *GTM, DrawState *ds, Lighting *lighting, Image *src);
// 3D Module Functions
void module_translate(Module *md, double tx, double ty, double tz);
void module_scale(Module *md, double sx, double sy, double sz);
void module_rotateX(Module *md, double cth, double sth);
void module_rotateY(Module *md, double cth, double sth);
void module_rotateXYZ(Module *md, Vector *u, Vector *v, Vector *w);
void module_cube(Module *md, int solid);
void module_cylinder(Module *md, int sides);
void module_pyramid(Module *md, int sides);
void module_color(Module *md, Color *c);
void module_bodyColor(Module *md, Color *c);
void module_surfaceColor(Module *md, Color *c);
void module_surfaceCoeff(Module *md, float coeff);
void module_bezierCurve(Module *md, BezierCurve *b);
void module_bezierSurface(Module *md, BezierSurface *b, int divisions, int solid);

#endif // MODULE_H
