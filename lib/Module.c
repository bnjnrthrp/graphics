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
            light_init(&(e->obj.light));
            light_copy(&(e->obj.light), (Light *)obj);
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

    // if (!lighting)
    // {
    //     printf("null lighting pointer here\n");
    //     exit(-1);
    // }
    Matrix LTM;
    matrix_identity(&LTM);

    Element *e = md->head;
    while (e) // Iterate through the list until you get to NULL (end of list)
    {
        switch (e->type)
        {
        case ObjBezier:
        {
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
        }
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
        {
            Point p, pt;
            point_copy(&p, &(e->obj.point));
            // Transform the point by each matrix, but alternate the destination to avoid
            // using the same point for p and q
            matrix_xformPoint(&LTM, &p, &pt);
            matrix_xformPoint(GTM, &pt, &p);
            matrix_xformPoint(VTM, &p, &pt);
            point_normalize(&pt);
            point_draw(&pt, src, ds->color);
        }
        break;
        case ObjLine:
        {
            Line line;
            line_copy(&line, &(e->obj.line));
            matrix_xformLine(&LTM, &line);
            matrix_xformLine(GTM, &line);
            matrix_xformLine(VTM, &line);
            line_normalize(&line);
            line_draw(&line, src, ds->color);
        }
        break;
        case ObjPolygon:
        {
            Polygon plygn;
            polygon_init(&plygn);
            polygon_copy(&plygn, &(e->obj.polygon));
            matrix_xformPolygon(&LTM, &plygn);
            matrix_xformPolygon(GTM, &plygn);
            polygon_setVertex3D(&plygn, plygn.nVertex, plygn.vertex); // for Phong Shading
            polygon_setNormalsPhong(&plygn, plygn.nVertex, plygn.normal);
            if (ds->shade == ShadeGouraud)
            {
                polygon_shade(&plygn, ds, lighting);
            }
            matrix_xformPolygon(VTM, &plygn);
            polygon_normalize(&plygn);
            if (ds->shade == ShadeFrame)
            {
                polygon_draw(&plygn, src, ds->color);
            }
            else if (ds->shade == ShadeFlat)
            {
                polygon_drawFill(&plygn, src, ds->color);
            }
            else
            {
                polygon_drawShade(&plygn, src, ds, lighting);
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
            module_draw(e->obj.module, VTM, &TM, &tempDS, lighting, src);
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
        Vector N[4];
        polygon_init(&p);

        // Top of cube
        point_set3D(&pt[0], -0.5, 0.5, -0.5);
        point_set3D(&pt[1], -0.5, 0.5, 0.5);
        point_set3D(&pt[2], 0.5, 0.5, 0.5);
        point_set3D(&pt[3], 0.5, 0.5, -0.5);
        polygon_set(&p, 4, pt);
        for (int i = 0; i < 4; i++)
        {
            vector_set(&N[i], 0, 1, 0);
        }
        polygon_setNormals(&p, 4, N);
        module_polygon(md, &p);

        // // back of cube
        // point_set3D(&pt[0], -0.5, -0.5, -0.5);
        // point_set3D(&pt[1], 0.5, -0.5, -0.5);
        // point_set3D(&pt[2], 0.5, 0.5, -0.5);
        // point_set3D(&pt[3], -0.5, 0.5, -0.5);
        // polygon_set(&p, 4, pt);
        // for (int i = 0; i < 4; i++)
        // {
        //     vector_set(&N[i], 0, 0, -1);
        // }
        // polygon_setNormals(&p, 4, N);
        // // module_rotateX(md, 0.0, 1.0);
        // module_polygon(md, &p);

        // // Bottom of cube
        // point_set3D(&pt[0], -0.5, -0.5, 0.5);
        // point_set3D(&pt[1], 0.5, -0.5, 0.5);
        // point_set3D(&pt[2], 0.5, -0.5, -0.5);
        // point_set3D(&pt[3], -0.5, -0.5, -0.5);
        // polygon_set(&p, 4, pt);
        // for (int i = 0; i < 4; i++)
        // {
        //     vector_set(&N[i], 0, -1, 0);
        // }
        // polygon_setNormals(&p, 4, N);
        // // module_polygon(md, &p);
        // // module_rotateX(md, 0.0, 1.0);
        // module_polygon(md, &p);

        // // // Front of cube
        // point_set3D(&pt[0], -0.5, 0.5, 0.5);
        // point_set3D(&pt[1], 0.5, 0.5, 0.5);
        // point_set3D(&pt[2], 0.5, -0.5, 0.5);
        // point_set3D(&pt[3], -0.5, -0.5, 0.5);
        // polygon_set(&p, 4, pt);
        // for (int i = 0; i < 4; i++)
        // {
        //     vector_set(&N[i], 0, 0, 1);
        // }
        // polygon_setNormals(&p, 4, N);
        // // module_polygon(md, &p);
        // // module_rotateX(md, 0.0, 1.0);
        // module_polygon(md, &p);

        // // // right face of cube
        // point_set3D(&pt[0], 0.5, 0.5, 0.5);
        // point_set3D(&pt[1], 0.5, 0.5, -0.5);
        // point_set3D(&pt[2], 0.5, -0.5, -0.5);
        // point_set3D(&pt[3], 0.5, -0.5, 0.5);
        // polygon_set(&p, 4, pt);
        // for (int i = 0; i < 4; i++)
        // {
        //     vector_set(&N[i], 1, 0, 0);
        // }
        // polygon_setNormals(&p, 4, N);
        // // module_polygon(md, &p);
        // // module_rotateY(md, 0.0, 1.0);
        // module_polygon(md, &p);

        // // // // left face of cube
        // point_set3D(&pt[0], -0.5, 0.5, -0.5);
        // point_set3D(&pt[1], -0.5, 0.5, 0.5);
        // point_set3D(&pt[2], -0.5, -0.5, 0.5);
        // point_set3D(&pt[3], -0.5, -0.5, -0.5);
        // polygon_set(&p, 4, pt);
        // for (int i = 0; i < 4; i++)
        // {
        //     vector_set(&N[i], -1, 0, 0);
        // }
        // polygon_setNormals(&p, 4, N);
        // // module_polygon(md, &p);
        // // module_rotateY(md, -1.0, 0.0);
        // module_polygon(md, &p);

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
    Vector N[4];
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

        vector_set(&N[0], 0, 1, 0);
        vector_set(&N[1], 0, 1, 0);
        vector_set(&N[2], 0, 1, 0);
        polygon_setNormals(&p, 3, N);

        module_polygon(md, &p);

        point_copy(&pt[0], &xbot);
        point_set3D(&pt[1], x1, 0.0, z1);
        point_set3D(&pt[2], x2, 0.0, z2);
        polygon_set(&p, 3, pt);

        vector_set(&N[0], 0, -1, 0);
        vector_set(&N[1], 0, -1, 0);
        vector_set(&N[2], 0, -1, 0);
        polygon_setNormals(&p, 3, N);

        module_polygon(md, &p);

        point_set3D(&pt[0], x1, 0.0, z1);
        point_set3D(&pt[1], x2, 0.0, z2);
        point_set3D(&pt[2], x2, 1.0, z2);
        point_set3D(&pt[3], x1, 1.0, z1);
        polygon_set(&p, 4, pt);

        vector_set(&N[0], x1, 0.0, z1);
        vector_set(&N[1], x2, 0.0, z2);
        vector_set(&N[2], x2, 0.0, z2);
        vector_set(&N[3], x1, 0.0, z1);
        polygon_setNormals(&p, 4, N);

        module_polygon(md, &p);
    }

    polygon_clear(&p);
}

/**
 * This program will build a unit sphere with user provided resolution.
 */
void module_sphere(Module *md, int resolution)
{
    double center;
    int i, j;
    Point unitSphere[resolution * resolution + resolution];
    Vector unitNormals[resolution * resolution + resolution];
    Vector N[4];
    Point top, bottom;
    Vector topVector, bottomVector;
    Polygon p;
    Point pt[4];

    polygon_init(&p);
    point_set3D(&top, 0.0, 1.0, 0.0);
    point_set3D(&bottom, 0.0, -1.0, 0.0);
    vector_set(&topVector, 0.0, 1.0, 0.0);
    vector_set(&bottomVector, 0.0, -1.0, 0.0);
    // Set each point in each row going up to the top
    for (i = 0; i < resolution + 1; i++)
    {
        // Determine the y coordinate of each center of the circle
        center = sin((float)i * M_PI / (float)resolution - M_PI * 0.5); // [-1, 1]
        for (j = 0; j < resolution; j++)
        {
            // Set points for each circle (x, z) at the height y
            point_set3D(&(unitSphere[i * resolution + j]),
                        cos((float)j * 2.0 * M_PI / (float)resolution) * cos((float)i * M_PI / (float)resolution - .5 * M_PI),
                        center,
                        sin((float)j * 2.0 * M_PI / (float)resolution) * cos((float)i * M_PI / (float)resolution - .5 * M_PI));

            vector_set(&unitNormals[i * resolution + j],
                       cos((float)j * 2.0 * M_PI / (float)resolution) * cos((float)i * M_PI / (float)resolution - .5 * M_PI),
                       center,
                       sin((float)j * 2.0 * M_PI / (float)resolution) * cos((float)i * M_PI / (float)resolution - .5 * M_PI));
        }
    }

    // Builds the middle ribbons using two rows of points and lacing the triangles together
    // Does so by building a square with 4 points, then drawing 2 triangles per loop
    // i is for each level, j is for each point going around the sphere
    for (i = 0; i < resolution; i++)
    {
        for (j = 0; j < resolution - 1; j++)
        {
            point_copy(&pt[0], &unitSphere[(i + 1) * resolution + j]);     // Top left
            point_copy(&pt[1], &unitSphere[i * resolution + j]);           // Bottom left
            point_copy(&pt[2], &unitSphere[(i + 1) * resolution + j + 1]); // Top right
            point_copy(&pt[3], &unitSphere[i * resolution + j + 1]);       // Bottom right

            vector_copy(&N[0], &unitNormals[(i + 1) * resolution + j]);     // Top left
            vector_copy(&N[1], &unitNormals[i * resolution + j]);           // Bottom left
            vector_copy(&N[2], &unitNormals[(i + 1) * resolution + j + 1]); // Top right
            vector_copy(&N[3], &unitNormals[i * resolution + j + 1]);

            polygon_set(&p, 3, pt); // First triangle
            polygon_setNormals(&p, 3, N);
            module_polygon(md, &p);

            polygon_set(&p, 3, &pt[1]); // Second triangle
            polygon_setNormals(&p, 3, &N[1]);
            module_polygon(md, &p);
        }
        // Close the strip
        point_copy(&pt[0], &unitSphere[(i + 1) * resolution + resolution - 1]); // End of top row
        point_copy(&pt[1], &unitSphere[i * resolution + resolution - 1]);       // End of bottom row
        point_copy(&pt[2], &unitSphere[(i + 1) * resolution]);                  // Start of top row
        point_copy(&pt[3], &unitSphere[i * resolution]);                        // Start of bottom row

        // Copy the normals for the points
        vector_copy(&N[0], &unitNormals[(i + 1) * resolution + resolution - 1]);
        vector_copy(&N[1], &unitNormals[i * resolution + resolution - 1]);
        vector_copy(&N[2], &unitNormals[(i + 1) * resolution]);
        vector_copy(&N[3], &unitNormals[i * resolution]);

        // Set the polygons and write the module
        polygon_set(&p, 3, pt); // First triangle
        polygon_setNormals(&p, 3, N);
        module_polygon(md, &p);
        polygon_set(&p, 3, &pt[1]); // Second triangle
        polygon_setNormals(&p, 3, &N[1]);
        module_polygon(md, &p);
    }

    // Builds the top fan
    point_copy(&pt[0], &top);
    vector_copy(&N[0], &topVector);
    for (i = 0; i < resolution - 1; i++) // Final set of size resolution
    {
        point_copy(&pt[1], &unitSphere[resolution * resolution + i]); // Final set of points
        point_copy(&pt[2], &unitSphere[resolution * resolution + i + 1]);
        vector_copy(&N[1], &unitNormals[resolution * resolution + i]);
        vector_copy(&N[2], &unitNormals[resolution * resolution + i + 1]);
        polygon_set(&p, 3, pt);
        polygon_setNormals(&p, 3, N);

        module_polygon(md, &p);
    }
    // Builds the final triangle in the fan
    point_copy(&pt[1], &unitSphere[resolution * resolution + resolution - 1]);  // End of final set
    point_copy(&pt[2], &unitSphere[resolution * resolution]);                   // Start of final set
    vector_copy(&N[1], &unitNormals[resolution * resolution + resolution - 1]); // End of final set
    vector_copy(&N[2], &unitNormals[resolution * resolution]);
    polygon_set(&p, 3, pt);
    polygon_setNormals(&p, 3, pt);
    module_polygon(md, &p);

    polygon_clear(&p);
}

/**
 * Makes a unit pyramid of any size base. The height will be 1 and the number of sides is provided by the user.
 * Default will be 3 (tetrahedron)
 */
void module_pyramid(Module *md, int sides)
{
    if (!md)
    {
        fprintf(stderr, "Invalid pointer provided to module_cylinder\n");
    }
    if (sides < 3)
    {
        sides = 3;
    }

    Polygon p;
    Point top;
    Point bottom[sides];
    Vector N[sides];
    double x1, x2, z1, z2;
    int i;

    polygon_init(&p);
    point_set3D(&top, 0.0, 1.0, 0.0);

    // make a triangle for the base that touches the unit circle
    for (i = 0; i < sides; i++)
    {
        Point pt[3];

        x1 = cos(i * M_PI * 2.0 / sides);
        z1 = sin(i * M_PI * 2.0 / sides);
        x2 = cos(((i + 1) % sides) * M_PI * 2.0 / sides);
        z2 = sin(((i + 1) % sides) * M_PI * 2.0 / sides);

        point_copy(&pt[0], &top);
        point_set3D(&pt[1], x1, 0.0, z1);
        point_set3D(&pt[2], x2, 0.0, z2);

        polygon_set(&p, 3, pt);

        vector_calculateNormal(&N[0], &pt[1], &pt[0], &pt[2]);
        vector_calculateNormal(&N[1], &pt[2], &pt[1], &pt[0]);
        vector_calculateNormal(&N[2], &pt[0], &pt[2], &pt[1]);
        polygon_setNormals(&p, 3, N);

        module_polygon(md, &p);

        // Add the point to the base
        point_set3D(&bottom[i], x1, 0.0, z1);
    }
    // Build the base
    polygon_set(&p, sides, bottom);
    for (int i = 0; i < sides; i++)
    {
        vector_set(&N[i], 0, -1, 0);
    }
    polygon_setNormals(&p, sides, N);
    module_polygon(md, &p);

    polygon_clear(&p);
}

/**
 * Builds a heightmap on a 1 x 1 grid that will be subdivided a given number of times. Adds a perturbation to each midpoint as it subdivides
 *
 * @param md the module to add the terrain submap to
 * @param prevMap The 2D matrix with the previous iteration's height values
 * @param count the current iteration number
 * @param maxIterations the total number of iterations desired
 * @param roughness the roughness factor for calculating the size of the perturbations
 */
void module_buildHeightMap(Module *md, DrawState *ds, int oldRows, int oldCols, double prevMap[oldRows][oldCols], int count, int maxIterations, double roughness)
{
    int i, j, newRows, newCols;
    double x1, x2, y, z1, z2, prev, next, avgHeight;
    Point pt[4];
    Polygon p;
    Vector N[3];
    Color White, Green, Blue, Brown, LtGrey, DkGrey, LtGreen, LtBlue;

    color_set(&White, 1.0, 1.0, 1.0);
    color_set(&Blue, 0.0, 0.0, 0.7);
    color_set(&LtBlue, 0.1, .1, 0.8);
    color_set(&Green, 0.15, .5, 0.15);
    color_set(&LtGreen, 0.3, .65, 0.3);
    color_set(&Brown, 0.55, 0.35, 0.0);
    color_set(&LtGrey, .8, .8, .8);
    color_set(&DkGrey, .3, .3, .3);

    if (!md || !prevMap || maxIterations < 0)
    {
        fprintf(stderr, "Invalid pointer provided to buildHeightMap\n");
        exit(-1);
    }

    // Base case: if iterations == count, then we're ready to draw
    if (count == maxIterations)
    {
        // Initialize the polygon for drawing
        polygon_init(&p);

        // Iterate through the matrix and create the points
        for (i = 0; i < oldRows - 1; i++)
        {
            for (j = 0; j < oldCols - 1; j++)
            {
                // x value becomes the index / cols and z value is z / rows. Builds a triangle mesh 2 triangles at a time
                // y Value is determined by the matrix at that position
                x1 = (double)j / (double)oldCols;
                x2 = (double)(j + 1) / (double)oldCols;
                z1 = (double)i / (double)oldRows;
                z2 = (double)(i + 1) / (double)oldRows;
                point_set3D(&pt[0], x1, prevMap[i][j], z1);
                point_set3D(&pt[1], x2, prevMap[i][j + 1], z1);
                point_set3D(&pt[2], x1, prevMap[i + 1][j], z2);
                point_set3D(&pt[3], x2, prevMap[i + 1][j + 1], z2);

                avgHeight = (pt[0].val[1] + pt[1].val[1] + pt[2].val[1] + pt[3].val[1]) / 4.0;

                if (avgHeight > .4)
                {
                    module_color(md, &White);
                    module_bodyColor(md, &White);
                    module_surfaceColor(md, &LtGrey);
                }
                else if (avgHeight > 0.2)
                {
                    module_color(md, &Brown);
                    module_bodyColor(md, &Brown);
                    module_surfaceColor(md, &DkGrey);
                }
                else if (avgHeight > 0.0)
                {
                    module_color(md, &Green);
                    module_bodyColor(md, &Green);
                    module_surfaceColor(md, &LtGreen);
                }
                else
                {
                    module_color(md, &Blue);
                    module_bodyColor(md, &Blue);
                    module_surfaceColor(md, &LtBlue);
                }
                // Add triangles to the module
                polygon_set(&p, 3, &pt[0]);
                // Points are named in a Z shape, so calculate normals similarly
                vector_calculateNormal(&N[0], &pt[1], &pt[0], &pt[2]);
                vector_calculateNormal(&N[1], &pt[0], &pt[1], &pt[2]);
                vector_calculateNormal(&N[2], &pt[1], &pt[2], &pt[0]);
                for (int i = 0; i < 3; i++)
                {
                    vector_normalize(&N[i]);
                }
                printf("Normal is: ");
                vector_print(&N[0], stdout);
                polygon_setNormals(&p, 3, N);
                module_polygon(md, &p);

                polygon_set(&p, 3, &pt[1]);
                vector_calculateNormal(&N[0], &pt[2], &pt[1], &pt[3]);
                vector_calculateNormal(&N[1], &pt[3], &pt[2], &pt[1]);
                vector_calculateNormal(&N[2], &pt[1], &pt[3], &pt[2]);
                for (int i = 0; i < 3; i++)
                {
                    vector_normalize(&N[i]);
                }
                polygon_setNormals(&p, 3, N);
                module_polygon(md, &p);
            }
        }
        // Cleanup
        polygon_clear(&p);
    }
    else
    {
        count++; // Increase the counter for this cycle
        // Calculate the size of the expanded height map
        newRows = newCols = pow(2, count) + 1;
        // Initialize a new height map with the expanded size
        double newMap[newRows][newCols];
        for (i = 0; i < newRows; i++)
        {
            for (j = 0; j < newCols; j++)
            {
                newMap[i][j] = 0;
            }
        }

        // Copy the values from the old heightmap to the new one
        for (i = 0; i < oldRows; i++)
        {
            for (j = 0; j < oldCols; j++)
            {
                newMap[2 * i][2 * j] = prevMap[i][j];
            }
        }

        // Loop through new map now, and interpolate the height values that remain in each row first
        /**
         *  **Note - the values changing from 0 to something will have a random perturbation factor added in
         *  old map     new map         first pass      second pass
         *  1 3 1       1 0 3 0 1       1 2 3 2 1       1 2 3 2 1
         *  3 5 3   ->  0 0 0 0 0   ->  0 0 0 0 0  ->   2 3 4 3 2
         *  1 3 1       3 0 5 0 3       3 4 5 4 3       3 4 5 4 3
         *              0 0 0 0 0       0 0 0 0 0       2 3 4 3 2
         *              1 0 3 0 1       1 2 3 2 1       1 2 3 2 1
         */
        for (i = 0; i < newRows; i = i + 2)
        {
            for (j = 0; j < newCols; j++)
            {
                if (newMap[i][j] == 0)
                {
                    prev = newMap[i][j - 1];
                    next = newMap[i][j + 1];

                    // Perturb the value between these two and then insert the data
                    y = average(prev, next);
                    y = perturb(y, roughness, count);

                    newMap[i][j] = y;
                }
            }
        }

        // Repeat loop in the remaining rows to fill the zeroes, interpolate from the
        // upper and lower values and perturb
        for (i = 1; i < newRows - 1; i = i + 2)
        {
            for (j = 0; j < newCols; j++)
            {
                // Perturb the value between these two and then insert the data
                if (newMap[i][j] == 0)
                {
                    prev = newMap[i - 1][j];
                    next = newMap[i + 1][j];

                    // Perturb the value between these two and then insert the data
                    y = average(prev, next);

                    y = perturb(y, roughness, count);
                    newMap[i][j] = y;
                }
            }
        }
        // Recursively call the heightmap again, with the new count and map
        module_buildHeightMap(md, ds, newRows, newCols, newMap, count, maxIterations, roughness);
    }
}

/**
 * Builds a fractal landscape using a height map and a pseudo- diamond-square algorithm to generate the subsequent heights.
 */
void module_terrain(Module *md, DrawState *ds, int iterations, double roughness)
{
    if (!md || !ds)
    {
        fprintf(stderr, "Invalid pointer to module_terrain\n");
        exit(-1);
    }
    // Initialize the first matrix of points
    double heightMap[2][2];
    int i, j;

    // Set initial points to a random value between 0 and 1
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 2; j++)
        {
            heightMap[i][j] = drand48();
        }
    }
    // Begin the recursion loop
    module_buildHeightMap(md, ds, 2, 2, heightMap, 0, iterations, roughness);
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

/**
 * Copies a light struct and adds it to the module
 *
 * @param md the module to add the light to
 * @param light the light to add
 */
void module_addLight(Module *md, Lighting *light)
{
    if (!md || !light)
    {
        fprintf(stderr, "Null pointer sent to module_addLight\n");
        exit(-1);
    }
    Element *e = element_init(ObjLight, light);
    module_insert(md, e);
}

/**
 * Traverses the module and parses the lighting for calculations
 *
 * @param md the module to traverse
 * @param GTM the GTM
 * @param the lighting struct to keep track of found lights
 */
void module_parseLighting(Module *md, Matrix *GTM, Lighting *lighting)
{
    if (!md || !GTM || !lighting)
    {
        fprintf(stderr, "Null pointer sent to module_parseLighting\n");
        exit(-1);
    }
    Matrix LTM;

    matrix_identity(&LTM);

    Element *e = md->head;
    while (e)
    {
        if (e->type == ObjMatrix)
        {
            matrix_multiply(&(e->obj.matrix), &LTM, &LTM);
        }
        else if (e->type == ObjIdentity)
        {
            matrix_identity(&LTM);
        }
        else if (e->type == ObjLight)
        {
            Light tmp;
            Point p;
            Vector v;
            light_init(&tmp);
            light_copy(&tmp, &(e->obj.light));
            lighting_add(lighting, tmp.type, &tmp.color, &tmp.direction, &tmp.position, tmp.cutoff, tmp.sharpness);
            matrix_xformPoint(&LTM, &tmp.position, &p);
            matrix_xformPoint(GTM, &p, &tmp.position);

            matrix_xformVector(&LTM, &tmp.direction, &v);
            matrix_xformVector(GTM, &v, &tmp.direction);
        }
        e = e->next;
    }
}

/**
 * Draw the module using a ray tracer for hidden surface removal, shadows
 *
 * @param md the module to draw
 * @param GTM the global transformation matrix
 * @param lighting the lighting struct
 * @param ds the drawstate
 * @param rt the ray tracer struct
 */
void module_drawRay(Module *md, View3D *view, Matrix *VTM, Matrix *GTM, Lighting *lighting, DrawState *ds, RayTracer *rt, Image *src)
{
    if (!md || !VTM || !GTM || !lighting || !ds || !rt || !src)
    {
        fprintf(stderr, "Null pointer sent to module_drawRay\n");
        exit(-1);
    }
    int row, col;
    Vector Vij, U, dU, dUperCol, dV, dVperRow;
    Point COP, Pij, ul, ur, ll, lr; // upper left, upper right, lower left, lower right of View Plane
    Color color;

    printf("building the database\n\n");
    module_rayBuildDb(md, GTM, ds, rt); // First pass to build the polygon database
    printf("Build complete. Polygons: %d, Max: %d\n\n", rt->size, rt->max);
    polygon_print(&(rt->db[0]), stdout);

    // Calculate the COP
    view_calculateCOP(&COP, &view->vrp, view->d, &view->vpn);
    printf("COP is: ");
    vector_print(&COP, stdout);

    vector_cross(&(view->vup), &(view->vpn), &U);
    vector_normalize(&view->vpn);
    vector_normalize(&view->vup);
    vector_normalize(&U);

    // Determine 4 points of View Plane in world coordinates
    for (row = 0; row < 3; row++)
    {
        // Point = VRP - du/2 * U + dv/2 * VUP
        ul.val[row] = view->vrp.val[row] + 0.5 * U.val[row] * view->du + 0.5 * view->vup.val[row] * view->dv;
        ur.val[row] = view->vrp.val[row] - 0.5 * U.val[row] * view->du + 0.5 * view->vup.val[row] * view->dv;
        ll.val[row] = view->vrp.val[row] + 0.5 * U.val[row] * view->du - 0.5 * view->vup.val[row] * view->dv;
        lr.val[row] = view->vrp.val[row] - 0.5 * U.val[row] * view->du - 0.5 * view->vup.val[row] * view->dv;
        Pij.val[row] = view->vrp.val[row] - 0.5 * U.val[row] * view->du + .5 * view->vup.val[row] * view->dv;
    }

    // printf("U is at: ");
    // vector_print(&U, stdout);
    // printf("first pixel is at: ");
    // point_print(&Pij, stdout);
    printf("upper left: ");
    point_print(&ul, stdout);
    printf("upper right: ");
    point_print(&ur, stdout);
    printf("lower left: ");
    point_print(&ll, stdout);
    printf("lower right: ");
    point_print(&lr, stdout);

    vector_copy(&dU, &U);
    vector_multiplyScalar(&dU, view->du);
    vector_multiplyScalar(&dU, (1.0 / (float)src->cols)); // pre-scale dU to be per column
    vector_copy(&dUperCol, &dU);

    vector_copy(&dV, &view->vup);
    vector_multiplyScalar(&dV, view->dv);
    vector_multiplyScalar(&dV, (1.0 / (float)src->rows));
    vector_copy(&dVperRow, &dV);

    point_copy(&Pij, &ul);
    // printf("First point: ");
    // point_print(&Pij, stdout);

    for (row = 0; row < src->rows; row++)
    {
        for (col = 0; col < src->cols; col++)
        {

            // for (row = src->rows / 2 - 3; row < src->rows / 2 + 3; row++)
            // {
            //     for (col = src->cols / 2 - 3; col < src->cols / 2 + 3; col++)
            //     {
            // Calculate where the ray intersects view plane. We know VUP, VPN, can calculate U. Know du and dv (view)
            for (int k = 0; k < 3; k++)
            {
                Pij.val[k] = ul.val[k] - (col * dUperCol.val[k]) - (row * dVperRow.val[k]);
            }
            vector_setPoints(&Vij, &COP, &Pij); // Determine the vector of the ray going through the view plane
            vector_normalize(&Vij);
            // vector_print(&Vij, stdout);

            color = module_rayIntersect(lighting, ds, &COP, &Vij, rt, 1.0);
            image_setColor(src, row, col, color);
        }
    }

    // point_copy(&Pij, &view->vrp);
    // vector_setPoints(&Vij, &COP, &Pij);
    // color = module_rayIntersect(&COP, &Vij, rt, 1.0);
    // printf("Final point: ");
    // point_print(&Pij, stdout);
}

Color module_rayIntersect(Lighting *l, DrawState *ds, Point *pt, Vector *Vij, RayTracer *rt, float beta)
{
    if (!l || !ds || !Vij || !rt)
    {
        fprintf(stderr, "Null pointer sent to module_rayIntersect\n");
        exit(-1);
    }
    int i;
    double t, lt;
    double cutoff = .01;
    Color c;
    Color black, white;
    Polygon *p, *lp;
    Vector N, L, V;
    Point pIntersect, lIntersect;
    Color Cb = ds->body;

    color_set(&black, 0.0, 0.0, 0.0);
    color_set(&white, 1.0, 1.0, 1.0);
    // printf("Entering ray intersect\n");
    if (beta < cutoff)
    {
        // printf("cutoff reached\n");
        return black;
    }
    p = rayTracer_closestPolygon(pt, rt, Vij, &pIntersect);

    if (!p)
    { // No intersection, return background color
        // printf("No polygon\n");
        return black;
    }
    else
    {
        // printf("Polygon hit: ");
        return white;
        // polygon_print(p, stdout);
        // printf("Intersects at: ");
        // point_print(&pIntersect, stdout);
        // exit(-1);
    }

    vector_calculateNormal(&N, &(p->vertex3D[0]), &pIntersect, &(p->vertex3D[1]));
    vector_normalize(&N);
    // printf("normal is: ");
    // vector_print(&N, stdout);
    color_copy(&c, &black);

    // printf("closest polygon is: ");
    // polygon_print(p, stdout);

    for (i = 0; i < l->nLights; i++)
    {
        // Determine if ambient or point
        if (l->light[i].type == LightAmbient)
        {
            c.c[0] += l->light[i].color.c[0] * Cb.c[0];
            c.c[1] += l->light[i].color.c[1] * Cb.c[1];
            c.c[2] += l->light[i].color.c[2] * Cb.c[2];
            // printf("Adding ambient component %.2f %.2f %.2f\n", c.c[0], c.c[1], c.c[2]);
            continue; // Go to next loop
        }
        else if (l->light[i].type == LightPoint)
        {
            // Cast a ray to the light
            vector_setPoints(&L, &pIntersect, &(l->light[i].position));
            // vector_normalize(&L);
            // Calculate t
            t = ray_calculateT(&pIntersect, &(l->light[i].position), &L);
            // Intersect the ray with each polygon in the scene, put the result in lIntersect
            lp = rayTracer_closestPolygon(&pIntersect, rt, &L, &lIntersect);
            // printf("\n\nlp: ");
            // polygon_print(lp, stdout);
            lt = ray_calculateT(&pIntersect, &lIntersect, &L); // Calculate the t value to the nearest polygon
            // printf("About to enter shading calc\n");
            // If no polygon returned, or t of the polygon > t to the light, light is visible
            if (lt - .5 > 0 && lt < t)
            {
                // printf("\n\nlight blocked by: \n");
                // polygon_print(lp, stdout);
                // printf("lt: %.5f\n", lt);
                // printf("t: %.5f\n", t);
                // exit(-1);
                // If intersects between the point and the light, light is blocked
                // Otherwise, add contributions of the light
                continue;
            }
            // printf("Light is visible\n");
            // polygon_print(p, stdout);
            // printf("light intersect point: ");
            // point_print(&pIntersect, stdout);
            // exit(-1);
            vector_setPoints(&V, &(ds->viewer), &pIntersect); // get the view vector
            lighting_shadingSingle(&(l->light[i]), &N, &V, &pIntersect, &(ds->body), &(ds->surface), ds->surfaceCoeff, p->oneSided, &c);
            // printf("Adding component %.2f %.2f %.2f\n", c.c[0], c.c[1], c.c[2]);
            // printf("\n\nlight position: ");
            // point_print(&l->light[i].position, stdout);
            // printf("view intersect point: ");
            // point_print(&pIntersect, stdout);
            // printf("on polygon: ");
            // polygon_print(p, stdout);
            // printf("light vector: ");
            // vector_print(&L, stdout);
            // printf("Intersects at: ");
            // point_print(&lIntersect, stdout);
            // printf("t value: %.5f: ", ray_calculateT(&pIntersect, &lIntersect, &L));
        }
    }
    // printf("Final value at pixel %.2f, %.2f, %.2f\n", c.c[0], c.c[1], c.c[2]);
    return c;
}

/**
 * Builds a polygon database in world coordinates to use in the ray tracer draw program
 *
 * @param md the module to draw
 * @param GTM the global transformation matrix
 * @param rt the ray tracer struct that will hold the database
 */
void module_rayBuildDb(Module *md, Matrix *GTM, DrawState *ds, RayTracer *rt)
{
    if (!md || !GTM || !rt)
    {
        fprintf(stderr, "Null pointer sent to module_rayBuildDb\n");
        exit(-1);
    }

    Matrix LTM;
    matrix_identity(&LTM);

    // int i = 0;
    Element *e = md->head;
    while (e)
    {
        // printf("element number %d\n", i);
        switch (e->type)
        {
        case ObjModule:
        {
            // printf("type: Module\n");
            Matrix TM;
            DrawState tempDS;
            drawstate_copy(&tempDS, ds);
            matrix_multiply(GTM, &LTM, &TM);
            module_rayBuildDb(e->obj.module, &TM, &tempDS, rt);
            break;
        }
        case ObjMatrix:
        {
            // printf("type: Matrix\n");
            matrix_multiply(&(e->obj.matrix), &LTM, &LTM);
            break;
        }
        case ObjIdentity:
        {
            // printf("type: identity\n");
            matrix_identity(&LTM);
            break;
        }
        case ObjPolygon:
        {
            // printf("type: polygon\n");
            Polygon p;
            polygon_init(&p);
            polygon_copy(&p, &(e->obj.polygon));
            matrix_xformPolygon(&LTM, &p);
            matrix_xformPolygon(GTM, &p);
            polygon_setVertex3D(&p, p.nVertex, p.vertex); // for Phong Shading
            polygon_setNormalsPhong(&p, p.nVertex, p.normal);
            rayTracer_add(rt, &p);
            break;
        }
        default:
            printf("Other\n");
            break;
        }
        e = e->next;
        // i++;
    }
}
