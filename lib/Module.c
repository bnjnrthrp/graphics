/**
 * A class representing a module of instructions for a setting or drawing
 *
 * @author Benji Northrop
 */

#include <stdlib.h>
#include "Module.h"
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
 * @param solid Integer indicating whether the cube is solid.
 */
void module_cube(Module *md, int solid)
{
    if (!md)
    {
        fprintf(stderr, "Invalid pointer provided to module_cube\n");
        exit(-1);
    }

    Point p[8];
    Point pt[4];
    Polygon side[6];
    Line line[12];
    Polygon tpoly;
    Color White;
    int i;

    Module *cube;

    // Initialize the points
    point_set(&p[0], -0.5, -0.5, -0.5, 1);
    point_set(&p[1], 0.5, -0.5, -0.5, 1);
    point_set(&p[2], 0.5, -0.5, 0.5, 1);
    point_set(&p[3], -0.5, -0.5, 0.5, 1);
    point_set(&p[4], -0.5, 0.5, -0.5, 1);
    point_set(&p[5], 0.5, 0.5, -0.5, 1);
    point_set(&p[6], 0.5, 0.5, 0.5, 1);
    point_set(&p[7], -0.5, 0.5, 0.5, 1);

    cube = module_create();
    if (solid == 0) // If solid, then make using polygons
    {
        for (i = 0; i < 6; i++)
        {
            polygon_init(&side[i]);
        }

        // Bottom
        polygon_set(&side[0], 4, &(p[0]));

        // Top
        polygon_set(&side[1], 4, &(p[4]));

        // Front
        point_copy(&pt[0], &p[0]);
        point_copy(&pt[1], &p[1]);
        point_copy(&pt[2], &p[4]);
        point_copy(&pt[3], &p[5]);

        polygon_set(&side[2], 4, pt);

        // Left side
        point_copy(&pt[0], &p[0]);
        point_copy(&pt[1], &p[3]);
        point_copy(&pt[2], &p[4]);
        point_copy(&pt[3], &p[7]);

        polygon_set(&side[3], 4, pt);

        // Right side
        point_copy(&pt[0], &p[1]);
        point_copy(&pt[1], &p[2]);
        point_copy(&pt[2], &p[5]);
        point_copy(&pt[3], &p[6]);

        polygon_set(&side[4], 4, pt);

        // Back side
        point_copy(&pt[0], &p[2]);
        point_copy(&pt[1], &p[3]);
        point_copy(&pt[2], &p[6]);
        point_copy(&pt[3], &p[7]);

        polygon_set(&side[5], 4, pt);

        for (i = 0; i < 6; i++)
        {
            module_polygon(cube, &side[i]);
        }
    }
    else
    {
        // Bottom square
        line_set(&(line[0]), p[0], p[1]);
        line_set(&(line[1]), p[1], p[2]);
        line_set(&(line[2]), p[2], p[3]);
        line_set(&(line[3]), p[3], p[0]);

        // Top square
        line_set(&(line[4]), p[4], p[5]);
        line_set(&(line[5]), p[5], p[6]);
        line_set(&(line[6]), p[6], p[7]);
        line_set(&(line[7]), p[7], p[4]);

        // Vertical edges
        line_set(&(line[8]), p[0], p[4]);
        line_set(&(line[9]), p[1], p[5]);
        line_set(&(line[10]), p[2], p[6]);
        line_set(&(line[11]), p[3], p[7]);

        for (i = 0; i < 12; i++)
        {
            module_line(cube, &line[i]);
        }
    }
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
