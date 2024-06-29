/**
 * A class representing a module of instructions for a setting or drawing
 *
 * @author Benji Northrop
 */

#include <stdlib.h>
#include <math.h>
#include "Module.h"
#define M_PI 3.14159265358979323846

/**
 * Allocate and return an initialized but empty Element.
 *
 * @return Element* The new empty Element.
 */
Element *element_create()
{
    Element *e = (Element *)malloc(sizeof(Element));
    if (!e)
    {
        fprintf(stderr, "Malloc failed in element_create\n");
        exit(-1);
    }
    // Initialize the element with default empty values
    e->type = ObjNone;
    e->obj.module = NULL; // Temp use the module field to allow a pointer
    e->next = NULL;
    return e;
}

/**
 * Allocate an Element and store a duplicate of the data pointed to by obj in the Element.
 *
 * @param type The type of the object.
 * @param obj Pointer to the object to duplicate.
 * @return Element* The new Element with duplicated data.
 */
Element *element_init(ObjectType type, void *obj)
{
    // Null check
    if (!obj || type == ObjNone)
    {
        fprintf(stderr, "Null pointer provided to element_init or object type was None\n");
        exit(-1);
    }
    Element *e = element_create(); // Create and allocate the element
    if (type == ObjModule)
    {
        e->obj.module = obj; // Don't duplicate the data for a module
    }
    else
        switch (type)
        {
        case ObjBezier:
            bezierCurve_init(&(e->obj.bezierCurve));
            bezierCurve_copy(&(e->obj.bezierCurve), (BezierCurve *)obj);
            break;
        case ObjLine:
            line_copy(&(e->obj.line), (Line *)obj);
            break;
        case ObjPoint:
            point_copy(&(e->obj.point), (Point *)obj);
            break;
        case ObjPolyline:
            polyline_init(&(e->obj.polyline));
            polyline_copy(&(e->obj.polyline), obj);
            break;
        case ObjPolygon:
            polygon_init(&(e->obj.polygon));
            polygon_copy(&(e->obj.polygon), (Polygon *)obj);
            break;
        case ObjMatrix:
            matrix_copy(&(e->obj.matrix), (Matrix *)obj);
            break;
        case ObjColor:
        case ObjBodyColor:
        case ObjSurfaceColor:
            color_copy(&(e->obj.color), (Color *)obj);
            break;
        case ObjIdentity:
            matrix_identity(&e->obj.matrix);
            break;
        // For the rest of these, nothing made yet so do nothing
        case ObjSurfaceCoeff:
            e->obj.coeff = *(float *)obj;
            break;
        case ObjLight:
        case ObjNone:
        case ObjModule:
            break;
        }

    e->next = NULL; // Ensure next pointer is set to NULL
    e->type = type; // Set the type to align with the data
    return e;
}

/**
 * Free the element and the object it contains, as appropriate.
 *
 * @param e Pointer to the Element to delete.
 */
void element_delete(Element *e)
{
    // Null check
    if (!e)
    {
        fprintf(stderr, "Invalid pointer to e");
        exit(-1);
    }
    // Determine the type of object, as certain objects have allocated memory
    switch (e->type)
    {
    case ObjPolyline:
        if (&(e->obj.polyline))
            polyline_clear(&(e->obj.polyline));
        break;
    case ObjPolygon:
        if (&(e->obj.polygon))
            polygon_clear(&(e->obj.polygon));
        break;
    default:
        // For the rest, do nothing, as no mallocs occurred for these types
        // Point, Line, Matrix, Color, BodyColor, SurfaceColor, SurfaceCoeff, Light
        break;
    }
    // Free the element
    free(e);
}

/**
 * Allocate an empty module.
 *
 * @return Module* The new empty module.
 */
Module *module_create()
{
    Module *m = (Module *)malloc(sizeof(Module));
    if (!m)
    {
        fprintf(stderr, "Module malloc failed in module_create\n");
        exit(-1);
    }
    m->head = NULL;
    m->tail = NULL;
    return m;
}

/**
 * Clear the module’s list of Elements, freeing memory as appropriate.
 *
 * @param md Pointer to the Module to clear.
 */
void module_clear(Module *md)
{
    Element *curr, *tmp;

    if (!md) // Null check
    {
        fprintf(stderr, "Null pointer provided to module_clear\n");
        exit(-1);
    }

    if (!md->head) // If the module is already empty
    {
        return;
    }

    curr = md->head; // Start at the head of the list
    while (curr != NULL)
    {
        tmp = curr->next;     // Temp hold the next node
        element_delete(curr); // Delete the current node
        curr = tmp;           // Progress forward in the module list
    }
    md->head = NULL; // Reset the head and tail
    md->tail = NULL;
}

/**
 * Free all of the memory associated with a module, including the memory pointed to by md.
 *
 * @param md Pointer to the Module to delete.
 */
void module_delete(Module *md)
{
    if (!md) // Null check
    {
        fprintf(stderr, "Null pointer provided to module_delete\n");
        exit(-1);
    }

    module_clear(md); // Clear the internal data of the module
    free(md);         // free the module itself
}

/**
 * Generic insert of an element into the module at the tail of the list.
 *
 * @param md Pointer to the Module.
 * @param e Pointer to the Element to insert.
 */
void module_insert(Module *md, Element *e)
{
    if (!md || !e) // Null check
    {
        fprintf(stderr, "Null pointer provided to module_insert\n");
        exit(-1);
    }
    // check if this is the first module, then add
    if (md->head == NULL && md->tail == NULL)
    {
        md->head = e;
        md->tail = e;
    }
    else // Not the first of the list
    {
        md->tail->next = e;
        md->tail = e;
    }
}

/**
 * Adds a pointer to the Module sub to the tail of the module’s list.
 *
 * @param md Pointer to the parent Module.
 * @param sub Pointer to the sub Module to add.
 */
void module_module(Module *md, Module *sub)
{
    if (!md || !sub) // Null check
    {
        fprintf(stderr, "Null pointer provided to module_module\n");
        exit(-1);
    }

    Element *e = element_init(ObjModule, sub);
    module_insert(md, e);
}

/**
 * Adds p to the tail of the module’s list.
 *
 * @param md Pointer to the Module.
 * @param p Pointer to the Point to add.
 */
void module_point(Module *md, Point *p)
{
    if (!md || !p) // Null check
    {
        fprintf(stderr, "Null pointer provided to module_point\n");
        exit(-1);
    }

    Element *e = element_init(ObjPoint, p);
    module_insert(md, e);
}

/**
 * Adds p to the tail of the module’s list.
 *
 * @param md Pointer to the Module.
 * @param p Pointer to the Line to add.
 */
void module_line(Module *md, Line *p)
{
    if (!md || !p) // Null check
    {
        fprintf(stderr, "Null pointer provided to module_line\n");
        exit(-1);
    }

    Element *e = element_init(ObjLine, p);
    module_insert(md, e);
}

/**
 * Adds p to the tail of the module’s list.
 *
 * @param md Pointer to the Module.
 * @param p Pointer to the Polyline to add.
 */
void module_polyline(Module *md, Polyline *p)
{
    if (!md || !p) // Null check
    {
        fprintf(stderr, "Null pointer provided to module_polyline\n");
        exit(-1);
    }

    Element *e = element_init(ObjPolyline, p);
    module_insert(md, e);
}

/**
 * Adds p to the tail of the module’s list.
 *
 * @param md Pointer to the Module.
 * @param p Pointer to the Polygon to add.
 */
void module_polygon(Module *md, Polygon *p)
{
    if (!md || !p) // Null check
    {
        fprintf(stderr, "Null pointer provided to module_polygon\n");
        exit(-1);
    }

    Element *e = element_init(ObjPolygon, p);
    module_insert(md, e);
}

/**
 * Object that sets the current transform to the identity, placed at the tail of the module’s list.
 *
 * @param md Pointer to the Module.
 */
void module_identity(Module *md)
{
    if (!md) // Null check
    {
        fprintf(stderr, "Null pointer provided to module_identity\n");
        exit(-1);
    }
    Matrix m;
    matrix_identity(&m);
    Element *e = element_init(ObjIdentity, &m);
    module_insert(md, e);
}

/**
 * Add a translation matrix to the tail of the module’s list.
 *
 * @param md Pointer to the Module.
 * @param tx Translation along the x-axis.
 * @param ty Translation along the y-axis.
 */
void module_translate2D(Module *md, double tx, double ty)
{
    if (!md) // Null check
    {
        fprintf(stderr, "Null pointer provided to module_translate2D\n");
        exit(-1);
    }
    Matrix tMatrix;
    matrix_identity(&tMatrix);            // Initialize the matrix and set to identity
    matrix_translate2D(&tMatrix, tx, ty); // Add the translation

    Element *e = element_init(ObjMatrix, &tMatrix); // Initialize the Element
    module_insert(md, e);                           // Add to the module
}

/**
 * Matrix operand to add a scale matrix to the tail of the module’s list.
 *
 * @param md Pointer to the Module.
 * @param sx Scaling factor along the x-axis.
 * @param sy Scaling factor along the y-axis.
 */
void module_scale2D(Module *md, double sx, double sy)
{
    if (!md) // Null check
    {
        fprintf(stderr, "Null pointer provided to module_scale2D\n");
        exit(-1);
    }
    Matrix sMatrix;
    matrix_identity(&sMatrix);
    matrix_scale2D(&sMatrix, sx, sy);

    Element *e = element_init(ObjMatrix, &sMatrix); // Initialize the Element
    module_insert(md, e);                           // Add to the module
}

/**
 * Matrix operand to add a rotation about the Z axis to the tail of the module’s list.
 *
 * @param md Pointer to the Module.
 * @param cth Cosine of the rotation angle.
 * @param sth Sine of the rotation angle.
 */
void module_rotateZ(Module *md, double cth, double sth)
{
    if (!md)
    {
        fprintf(stderr, "Null pointer provided to module_rotateZ\n");
        exit(-1);
    }
    Matrix m;
    matrix_identity(&m);
    matrix_rotateZ(&m, cth, sth);

    Element *e = element_init(ObjMatrix, &m);
    module_insert(md, e);
}

/**
 * Matrix operand to add a 2D shear matrix to the tail of the module’s list.
 *
 * @param md Pointer to the Module.
 * @param shx Shear factor along the x-axis.
 * @param shy Shear factor along the y-axis.
 */
void module_shear2D(Module *md, double shx, double shy)
{
    if (!md)
    {
        fprintf(stderr, "Null pointer provided to module_shear2D\n");
        exit(-1);
    }
    Matrix m;
    matrix_identity(&m);
    matrix_shear2D(&m, shx, shy);

    Element *e = element_init(ObjMatrix, &m);
    module_insert(md, e);
}

/**
 * Draw the module into the image using the given view transformation matrix [VTM], Lighting, and DrawState.
 *
 * @param md Pointer to the Module.
 * @param VTM Pointer to the view transformation matrix.
 * @param GTM Pointer to the global transformation matrix.
 * @param ds Pointer to the DrawState.
 * @param lighting Pointer to the Lighting structure.
 * @param src Pointer to the Image.
 */
void module_draw(Module *md, Matrix *VTM, Matrix *GTM, DrawState *ds, Lighting *lighting, Image *src)
{
    if (!md || !VTM || !GTM || !ds || !src)
    {
        fprintf(stderr, "Null pointer provided to module_draw\n");
        exit(-1);
    }
    Matrix LTM;
    matrix_identity(&LTM);

    Element *e = md->head;
    while (e) // Iterate through the list until you get to NULL (end of list)
    {
        switch (e->type)
        {
        case ObjBezier:
            Point cp[4], cpt[4];
            BezierCurve b;
            bezierCurve_init(&b);
            // Iterate through the control point list
            for (int i = 0; i < 4; i++)
            {
                point_copy(&(cp[i]), &(e->obj.bezierCurve.cp[i]));
                // Transform the point by each matrix, but alternate the destination to avoid
                // using the same point for cp and q
                matrix_xformPoint(&LTM, &(cp[i]), &(cpt[i]));
                matrix_xformPoint(GTM, &(cpt[i]), &(cp[i]));
                matrix_xformPoint(VTM, &(cp[i]), &(cpt[i]));
                point_normalize(&(cpt[i]));
            }
            bezierCurve_set(&b, cpt);
            bezierCurve_draw(&b, src, ds->color);
            break;
        case ObjColor:
            drawstate_setColor(ds, e->obj.color);
            break;
        case ObjBodyColor:
            drawstate_setBody(ds, e->obj.color);
            break;
        case ObjSurfaceColor:
            drawstate_setSurface(ds, e->obj.color);
            break;
        case ObjSurfaceCoeff:
            drawstate_setSurfaceCoeff(ds, e->obj.coeff);
        case ObjPoint:
            Point p, pt;
            point_copy(&p, &(e->obj.point));
            // Transform the point by each matrix, but alternate the destination to avoid
            // using the same point for p and q
            matrix_xformPoint(&LTM, &p, &pt);
            matrix_xformPoint(GTM, &pt, &p);
            matrix_xformPoint(VTM, &p, &pt);
            point_normalize(&pt);
            point_draw(&pt, src, ds->color);
            break;
        case ObjLine:
            Line line;
            line_copy(&line, &(e->obj.line));
            matrix_xformLine(&LTM, &line);
            matrix_xformLine(GTM, &line);
            matrix_xformLine(VTM, &line);
            line_normalize(&line);
            line_draw(&line, src, ds->color);
            break;
        case ObjPolygon:
        {
            Polygon plygn;
            polygon_init(&plygn);
            polygon_copy(&plygn, &(e->obj.polygon));
            matrix_xformPolygon(&LTM, &plygn);
            matrix_xformPolygon(GTM, &plygn);
            matrix_xformPolygon(VTM, &plygn);
            polygon_normalize(&plygn);
            if (ds->shade == ShadeFrame)
            {
                polygon_draw(&plygn, src, ds->color);
            }
            else if (ds->shade == ShadeConstant)
            {
                polygon_drawFill(&plygn, src, ds->color);
            }
            else
            {
                printf("Found something not shadeframe or constant\n");
            }
            polygon_clear(&plygn);
            break;
        }
        case ObjPolyline:
        {
            Polyline plyln;
            polyline_init(&plyln);
            polyline_copy(&plyln, &(e->obj.polyline));
            matrix_xformPolyline(&LTM, &plyln);
            matrix_xformPolyline(GTM, &plyln);
            matrix_xformPolyline(VTM, &plyln);
            polyline_normalize(&plyln);
            polyline_draw(&plyln, src, ds->color);
            polyline_clear(&plyln);
            break;
        }
        case ObjMatrix:
            matrix_multiply(&(e->obj.matrix), &LTM, &LTM);
            break;
        case ObjIdentity:
            matrix_identity(&LTM);
            break;
        case ObjModule:
        {
            Matrix TM;
            DrawState tempDS;
            drawstate_copy(&tempDS, ds);
            matrix_multiply(GTM, &LTM, &TM);
            module_draw(e->obj.module, VTM, &TM, &tempDS, NULL, src);
            break;
        }
        case ObjNone:
        case ObjLight:
            break;
        }
        e = e->next; // Forward sequence to next element
    }
}

/**
 * Matrix operand to add a 3D translation to the Module.
 *
 * @param md Pointer to the Module.
 * @param tx Translation along the x-axis.
 * @param ty Translation along the y-axis.
 * @param tz Translation along the z-axis.
 */
void module_translate(Module *md, double tx, double ty, double tz)
{
    if (!md) // Null check
    {
        fprintf(stderr, "Null pointer provided to module_translate\n");
        exit(-1);
    }
    Matrix tMatrix;
    matrix_identity(&tMatrix);              // Initialize the matrix and set to identity
    matrix_translate(&tMatrix, tx, ty, tz); // Add the translation

    Element *e = element_init(ObjMatrix, &tMatrix); // Initialize the Element
    module_insert(md, e);                           // Add to the module
}

/**
 * Matrix operand to add a 3D scale to the Module.
 *
 * @param md Pointer to the Module.
 * @param sx Scaling factor along the x-axis.
 * @param sy Scaling factor along the y-axis.
 * @param sz Scaling factor along the z-axis.
 */
void module_scale(Module *md, double sx, double sy, double sz)
{
    {
        if (!md) // Null check
        {
            fprintf(stderr, "Null pointer provided to module_scale\n");
            exit(-1);
        }
        Matrix sMatrix;
        matrix_identity(&sMatrix);
        matrix_scale(&sMatrix, sx, sy, sz);

        Element *e = element_init(ObjMatrix, &sMatrix); // Initialize the Element
        module_insert(md, e);                           // Add to the module
    }
}

/**
 * Matrix operand to add a rotation about the X-axis to the Module.
 *
 * @param md Pointer to the Module.
 * @param cth Cosine of the rotation angle.
 * @param sth Sine of the rotation angle.
 */
void module_rotateX(Module *md, double cth, double sth)
{
    if (!md)
    {
        fprintf(stderr, "Null pointer provided to module_rotateX\n");
        exit(-1);
    }
    Matrix m;
    matrix_identity(&m);
    matrix_rotateX(&m, cth, sth);

    Element *e = element_init(ObjMatrix, &m);
    module_insert(md, e);
}

/**
 * Matrix operand to add a rotation about the Y-axis to the Module.
 *
 * @param md Pointer to the Module.
 * @param cth Cosine of the rotation angle.
 * @param sth Sine of the rotation angle.
 */
void module_rotateY(Module *md, double cth, double sth)
{
    if (!md)
    {
        fprintf(stderr, "Null pointer provided to module_rotateY\n");
        exit(-1);
    }
    Matrix m;
    matrix_identity(&m);
    matrix_rotateY(&m, cth, sth);

    Element *e = element_init(ObjMatrix, &m);
    module_insert(md, e);
}

/**
 * Matrix operand to add a rotation that orients to the orthonormal axes ~u, ~v, ~w.
 *
 * @param md Pointer to the Module.
 * @param u Pointer to the first orthonormal vector.
 * @param v Pointer to the second orthonormal vector.
 * @param w Pointer to the third orthonormal vector.
 */
void module_rotateXYZ(Module *md, Vector *u, Vector *v, Vector *w)
{
    if (!md || !u || !v || !w)
    {
        fprintf(stderr, "Null pointer provided to module_rotateXYZ\n");
        exit(-1);
    }
    Matrix m;
    matrix_identity(&m);
    matrix_rotateXYZ(&m, u, v, w);

    Element *e = element_init(ObjMatrix, &m);
    module_insert(md, e);
}

/**
 * Adds a unit cube, axis-aligned and centered on zero to the Module. If solid is zero, add only lines.
 * If solid is non-zero, use polygons with surface normals defined.
 *
 * @param md Pointer to the Module.
 * @param solid Integer indicating whether the cube is solid. 0 = edges only, anything else is a solid fill
 */
void module_cube(Module *md, int solid)
{
    if (!md)
    {
        fprintf(stderr, "Invalid pointer provided to module_cube\n");
        exit(-1);
    }
    Point pt[5];

    // Bottom of cube
    point_set3D(&pt[0], -0.5, -0.5, -0.5);
    point_set3D(&pt[1], 0.5, -0.5, -0.5);
    point_set3D(&pt[2], 0.5, -0.5, 0.5);
    point_set3D(&pt[3], -0.5, -0.5, 0.5);
    point_set3D(&pt[4], -0.5, -0.5, -0.5);

    if (solid == 0)
    {
        Polyline p;
        polyline_init(&p);

        // Bottom of cube
        polyline_set(&p, 5, pt);
        module_polyline(md, &p);

        // Top of cube
        point_set3D(&pt[0], -0.5, 0.5, -0.5);
        point_set3D(&pt[1], 0.5, 0.5, -0.5);
        point_set3D(&pt[2], 0.5, 0.5, 0.5);
        point_set3D(&pt[3], -0.5, 0.5, 0.5);
        point_set3D(&pt[4], -0.5, 0.5, -0.5);
        polyline_set(&p, 5, pt);
        module_polyline(md, &p);

        // back of cube
        point_set3D(&pt[0], -0.5, -0.5, -0.5);
        point_set3D(&pt[1], 0.5, -0.5, -0.5);
        point_set3D(&pt[2], 0.5, 0.5, -0.5);
        point_set3D(&pt[3], -0.5, 0.5, -0.5);
        point_set3D(&pt[4], -0.5, -0.5, -0.5);
        polyline_set(&p, 5, pt);
        module_polyline(md, &p);

        // Front of cube
        point_set3D(&pt[0], -0.5, -0.5, 0.5);
        point_set3D(&pt[1], 0.5, -0.5, 0.5);
        point_set3D(&pt[2], 0.5, 0.5, 0.5);
        point_set3D(&pt[3], -0.5, 0.5, 0.5);
        point_set3D(&pt[4], -0.5, -0.5, 0.5);
        polyline_set(&p, 5, pt);
        module_polyline(md, &p);

        // right face of cube
        point_set3D(&pt[0], 0.5, -0.5, -0.5);
        point_set3D(&pt[1], 0.5, -0.5, 0.5);
        point_set3D(&pt[2], 0.5, 0.5, 0.5);
        point_set3D(&pt[3], 0.5, 0.5, -0.5);
        point_set3D(&pt[4], 0.5, -0.5, -0.5);
        polyline_set(&p, 5, pt);
        module_polyline(md, &p);

        // // left face of cube
        point_set3D(&pt[0], -0.5, -0.5, -0.5);
        point_set3D(&pt[1], -0.5, -0.5, 0.5);
        point_set3D(&pt[2], -0.5, 0.5, 0.5);
        point_set3D(&pt[3], -0.5, 0.5, -0.5);
        point_set3D(&pt[4], -0.5, -0.5, -0.5);
        polyline_set(&p, 5, pt);
        module_polyline(md, &p);

        polyline_clear(&p);
    }
    else
    {
        Polygon p;
        polygon_init(&p);

        // Bottom of cube
        polygon_set(&p, 4, pt);
        module_polygon(md, &p);

        // Top of cube
        point_set3D(&pt[0], -0.5, 0.5, -0.5);
        point_set3D(&pt[1], 0.5, 0.5, -0.5);
        point_set3D(&pt[2], 0.5, 0.5, 0.5);
        point_set3D(&pt[3], -0.5, 0.5, 0.5);
        polygon_set(&p, 4, pt);
        module_polygon(md, &p);

        // back of cube
        point_set3D(&pt[0], -0.5, -0.5, -0.5);
        point_set3D(&pt[1], 0.5, -0.5, -0.5);
        point_set3D(&pt[2], 0.5, 0.5, -0.5);
        point_set3D(&pt[3], -0.5, 0.5, -0.5);
        polygon_set(&p, 4, pt);
        module_polygon(md, &p);

        // Front of cube
        point_set3D(&pt[0], -0.5, -0.5, 0.5);
        point_set3D(&pt[1], 0.5, -0.5, 0.5);
        point_set3D(&pt[2], 0.5, 0.5, 0.5);
        point_set3D(&pt[3], -0.5, 0.5, 0.5);
        polygon_set(&p, 4, pt);
        module_polygon(md, &p);

        // right face of cube
        point_set3D(&pt[0], 0.5, -0.5, -0.5);
        point_set3D(&pt[1], 0.5, -0.5, 0.5);
        point_set3D(&pt[2], 0.5, 0.5, 0.5);
        point_set3D(&pt[3], 0.5, 0.5, -0.5);
        polygon_set(&p, 4, pt);
        module_polygon(md, &p);

        // // left face of cube
        point_set3D(&pt[0], -0.5, -0.5, -0.5);
        point_set3D(&pt[1], -0.5, -0.5, 0.5);
        point_set3D(&pt[2], -0.5, 0.5, 0.5);
        point_set3D(&pt[3], -0.5, 0.5, -0.5);
        polygon_set(&p, 4, pt);
        module_polygon(md, &p);

        polygon_clear(&p);
    }
}

/**
 * Builds a cylinder using Bezier curves and adds it to the module
 */
void module_cylinder(Module *md, int sides)
{
    if (!md)
    {
        fprintf(stderr, "Invalid pointer provided to module_cylinder\n");
    }

    Polygon p;
    Point xtop, xbot;
    double x1, x2, z1, z2;
    int i;

    polygon_init(&p);
    point_set3D(&xtop, 0, 1.0, 0.0);
    point_set3D(&xbot, 0, 0.0, 0.0);

    // make a fan for the top and bottom sides
    // and quadrilaterals for the sides
    for (i = 0; i < sides; i++)
    {
        Point pt[4];

        x1 = cos(i * M_PI * 2.0 / sides);
        z1 = sin(i * M_PI * 2.0 / sides);
        x2 = cos(((i + 1) % sides) * M_PI * 2.0 / sides);
        z2 = sin(((i + 1) % sides) * M_PI * 2.0 / sides);

        point_copy(&pt[0], &xtop);
        point_set3D(&pt[1], x1, 1.0, z1);
        point_set3D(&pt[2], x2, 1.0, z2);

        polygon_set(&p, 3, pt);
        module_polygon(md, &p);

        point_copy(&pt[0], &xbot);
        point_set3D(&pt[1], x1, 0.0, z1);
        point_set3D(&pt[2], x2, 0.0, z2);

        polygon_set(&p, 3, pt);
        module_polygon(md, &p);

        point_set3D(&pt[0], x1, 0.0, z1);
        point_set3D(&pt[1], x2, 0.0, z2);
        point_set3D(&pt[2], x2, 1.0, z2);
        point_set3D(&pt[3], x1, 1.0, z1);

        polygon_set(&p, 4, pt);
        module_polygon(md, &p);
    }

    polygon_clear(&p);
}

/**
 * Adds the foreground color value to the tail of the module’s list.
 *
 * @param md Pointer to the Module.
 * @param c Pointer to the Color.
 */
void module_color(Module *md, Color *c)
{
    if (!md || !c)
    {
        fprintf(stderr, "Invalid pointer to module_color\n");
        exit(-1);
    }
    Element *e = element_init(ObjColor, c);
    module_insert(md, e);
}

/**
 * Adds the body color value to the tail of the module’s list.
 *
 * @param md Pointer to the Module.
 * @param c Pointer to the Color.
 */
void module_bodyColor(Module *md, Color *c)
{
    if (!md || !c)
    {
        fprintf(stderr, "Invalid pointer to module_bodyColor\n");
        exit(-1);
    }
    Element *e = element_init(ObjBodyColor, c);
    module_insert(md, e);
}

/**
 * Adds the surface color value to the tail of the module’s list.
 *
 * @param md Pointer to the Module.
 * @param c Pointer to the Color.
 */
void module_surfaceColor(Module *md, Color *c)
{
    if (!md || !c)
    {
        fprintf(stderr, "Invalid pointer to module_surfaceColor\n");
        exit(-1);
    }
    Element *e = element_init(ObjSurfaceColor, c);
    module_insert(md, e);
}

/**
 * Adds the specular coefficient to the tail of the module’s list.
 *
 * @param md Pointer to the Module.
 * @param coeff Specular coefficient.
 */
void module_surfaceCoeff(Module *md, float coeff)
{
    {
        if (!md)
        {
            fprintf(stderr, "Invalid pointer to module_surfaceCoeff\n");
            exit(-1);
        }
        Element *e = element_init(ObjSurfaceCoeff, &coeff);
        module_insert(md, e);
    }
}

/**
 * Adds a Bezier Curve module to the list.
 *
 * @param md The Module to add the lines to.
 * @param b The BezierCurve to subdivide.
 * @param divisions The number of times to subdivide the Bezier curve.
 */
void module_bezierCurve(Module *md, BezierCurve *b)
{
    if (!md || !b)
    {
        fprintf(stderr, "Null pointer provided to module_bezierCurve\n");
        exit(-1);
    }
    Element *e = element_init(ObjBezier, b);
    module_insert(md, e);
}

/**
 * Uses the de Casteljau algorithm to subdivide the Bezier surface divisions times, then draws either the lines connecting the control points
 * if solid is 0, or draws filled triangles using the four corner control points.
 *
 * @param md The Module to add the lines or triangles to.
 * @param b The BezierSurface to subdivide.
 * @param divisions The number of times to subdivide the Bezier surface.
 * @param solid If 0, draws lines; if 1, draws triangles.
 */
void module_bezierSurface(Module *md, BezierSurface *b, int divisions, int solid)
{
    if (!md || !b)
    {
        fprintf(stderr, "Null pointer provided to module_bezierSurface\n");
        exit(-1);
    }

    // Convert each control point into a 4x4 grid of Bezier curves
    BezierCurve x[4], y[4];
    Point p[4];
    Line lineX[3];
    Line lineY[3];
    int i, j;

    // Horizontal curves
    for (i = 0; i < 4; i++)
    {
        // Horizontal Curves
        bezierCurve_set(&x[i], &(b->cp[i * 4]));

        // Vertical Curves
        point_copy(&p[0], &b->cp[i]);
        point_copy(&p[1], &b->cp[4 + i]);
        point_copy(&p[2], &b->cp[8 + i]);
        point_copy(&p[3], &b->cp[12 + i]);
        bezierCurve_set(&y[i], p);
    }

    // Base case: Ready to draw wire frame
    if (divisions == 0 && solid == 0)
    {
        for (i = 0; i < 4; i++)
        {
            for (j = 0; j < 3; j++)
            {
                line_set(&lineX[j], x[i].cp[j], x[i].cp[j + 1]);
                line_set(&lineY[j], y[i].cp[j], y[i].cp[j + 1]);
                module_line(md, &lineX[j]);
                module_line(md, &lineY[j]);
            }
        }
    }
    // Recursive case: subdivide the surface into 4 subsurfaces
    else
    {
        BezierSurface ul, ur, ll, lr; // Upper left, upper right, lower left, lower right
        BezierCurve bcTmp;
        Point xPt[4][8];
        Point newPt[8][8];
        Point tmp[8];
        Point ulCP[16];
        Point urCP[16];
        Point llCP[16];
        Point lrCP[16];
        int i, j;

        bezierSurface_init(&ul);
        bezierSurface_init(&ur);
        bezierSurface_init(&ll);
        bezierSurface_init(&lr);

        // Generate subsurfaces with de Casteljau's algorithm

        // Generate horizontal subcurves of the initial control points on one axis - doubles the number of curves in one axis
        for (i = 0; i < 4; i++)
        {
            casteljau(&x[i], xPt[i]);
        }
        for (i = 0; i < 8; i++)
        {
            // Set the points to create the additional curves in the opposite direction, then perform casteljau's
            bezierCurve_init(&bcTmp);
            for (j = 0; j < 8; j++)
            {
                point_copy(&tmp[0], &xPt[0][i]);
                point_copy(&tmp[1], &xPt[1][i]);
                point_copy(&tmp[2], &xPt[2][i]);
                point_copy(&tmp[3], &xPt[3][i]);
                bezierCurve_set(&bcTmp, tmp);
            }
            casteljau(&bcTmp, newPt[i]);
        }
        // Iterate through each column of points and put them into their final respective Bezier surface
        for (i = 0; i < 4; i++)
        {
            for (j = 0; j < 4; j++)
            {
                point_copy(&(ulCP[i * 4 + j]), &newPt[j][i]);
                point_copy(&(urCP[i * 4 + j]), &newPt[j + 4][i]);
                point_copy(&(llCP[i * 4 + j]), &newPt[j][i + 4]);
                point_copy(&(lrCP[i * 4 + j]), &newPt[j + 4][i + 4]);
            }
        }
        bezierSurface_set(&ul, ulCP);
        bezierSurface_set(&ur, urCP);
        bezierSurface_set(&ll, llCP);
        bezierSurface_set(&lr, lrCP);

        // Recursively call module_bezier on the 4 subsurfaces with 1 less division
        module_bezierSurface(md, &ul, divisions - 1, solid);
        module_bezierSurface(md, &ur, divisions - 1, solid);
        module_bezierSurface(md, &ll, divisions - 1, solid);
        module_bezierSurface(md, &lr, divisions - 1, solid);
    }
}
