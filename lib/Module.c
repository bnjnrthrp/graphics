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
            polyline_copy(&(e->obj.polyline), (Polyline *)obj);
            break;
        case ObjPolygon:
            polygon_copy(&(e->obj.polygon), (Polygon *)obj);
            break;
        case ObjMatrix:
            matrix_copy(&(e->obj.matrix), (Matrix *)obj);
            break;
        case ObjColor:
            color_copy(&(e->obj.color), (Color *)obj);
            break;
        // For the rest of these, nothing made yet so do nothing
        case ObjBodyColor:
            break;
        case ObjSurfaceColor:
            break;
        case ObjSurfaceCoeff:
            break;
        case ObjLight:
            break;
        }

    e->next = NULL; // Ensure next pointer is set to NULL
    e->type = type; // Set the type to align with the data
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
        polyline_free(&(e->obj.polyline));
        break;
    case ObjPolygon:
        polygon_free(&(e->obj.polygon));
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
    m->head == NULL;
    m->tail == NULL;
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
void module_insert(Module *md, Element *e);

/**
 * Adds a pointer to the Module sub to the tail of the module’s list.
 *
 * @param md Pointer to the parent Module.
 * @param sub Pointer to the sub Module to add.
 */
void module_module(Module *md, Module *sub);

/**
 * Adds p to the tail of the module’s list.
 *
 * @param md Pointer to the Module.
 * @param p Pointer to the Point to add.
 */
void module_point(Module *md, Point *p);

/**
 * Adds p to the tail of the module’s list.
 *
 * @param md Pointer to the Module.
 * @param p Pointer to the Line to add.
 */
void module_line(Module *md, Line *p);

/**
 * Adds p to the tail of the module’s list.
 *
 * @param md Pointer to the Module.
 * @param p Pointer to the Polyline to add.
 */
void module_polyline(Module *md, Polyline *p);

/**
 * Adds p to the tail of the module’s list.
 *
 * @param md Pointer to the Module.
 * @param p Pointer to the Polygon to add.
 */
void module_polygon(Module *md, Polygon *p);

/**
 * Object that sets the current transform to the identity, placed at the tail of the module’s list.
 *
 * @param md Pointer to the Module.
 */
void module_identity(Module *md);

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

    Element *e = element_init(ObjMatrix, &tMatrix);
}

/**
 * Matrix operand to add a scale matrix to the tail of the module’s list.
 *
 * @param md Pointer to the Module.
 * @param sx Scaling factor along the x-axis.
 * @param sy Scaling factor along the y-axis.
 */
void module_scale2D(Module *md, double sx, double sy);

/**
 * Matrix operand to add a rotation about the Z axis to the tail of the module’s list.
 *
 * @param md Pointer to the Module.
 * @param cth Cosine of the rotation angle.
 * @param sth Sine of the rotation angle.
 */
void module_rotateZ(Module *md, double cth, double sth);

/**
 * Matrix operand to add a 2D shear matrix to the tail of the module’s list.
 *
 * @param md Pointer to the Module.
 * @param shx Shear factor along the x-axis.
 * @param shy Shear factor along the y-axis.
 */
void module_shear2D(Module *md, double shx, double shy);

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
void module_draw(Module *md, Matrix *VTM, Matrix *GTM, DrawState *ds, Lighting *lighting, Image *src);

/**
 * Matrix operand to add a 3D translation to the Module.
 *
 * @param md Pointer to the Module.
 * @param tx Translation along the x-axis.
 * @param ty Translation along the y-axis.
 * @param tz Translation along the z-axis.
 */
void module_translate(Module *md, double tx, double ty, double tz);

/**
 * Matrix operand to add a 3D scale to the Module.
 *
 * @param md Pointer to the Module.
 * @param sx Scaling factor along the x-axis.
 * @param sy Scaling factor along the y-axis.
 * @param sz Scaling factor along the z-axis.
 */
void module_scale(Module *md, double sx, double sy, double sz);

/**
 * Matrix operand to add a rotation about the X-axis to the Module.
 *
 * @param md Pointer to the Module.
 * @param cth Cosine of the rotation angle.
 * @param sth Sine of the rotation angle.
 */
void module_rotateX(Module *md, double cth, double sth);

/**
 * Matrix operand to add a rotation about the Y-axis to the Module.
 *
 * @param md Pointer to the Module.
 * @param cth Cosine of the rotation angle.
 * @param sth Sine of the rotation angle.
 */
void module_rotateY(Module *md, double cth, double sth);

/**
 * Matrix operand to add a rotation that orients to the orthonormal axes ~u, ~v, ~w.
 *
 * @param md Pointer to the Module.
 * @param u Pointer to the first orthonormal vector.
 * @param v Pointer to the second orthonormal vector.
 * @param w Pointer to the third orthonormal vector.
 */
void module_rotateXYZ(Module *md, Vector *u, Vector *v, Vector *w);

/**
 * Adds a unit cube, axis-aligned and centered on zero to the Module. If solid is zero, add only lines.
 * If solid is non-zero, use polygons with surface normals defined.
 *
 * @param md Pointer to the Module.
 * @param solid Integer indicating whether the cube is solid.
 */
void module_cube(Module *md, int solid);

/**
 * Adds the foreground color value to the tail of the module’s list.
 *
 * @param md Pointer to the Module.
 * @param c Pointer to the Color.
 */
void module_color(Module *md, Color *c);

/**
 * Adds the body color value to the tail of the module’s list.
 *
 * @param md Pointer to the Module.
 * @param c Pointer to the Color.
 */
void module_bodyColor(Module *md, Color *c);

/**
 * Adds the surface color value to the tail of the module’s list.
 *
 * @param md Pointer to the Module.
 * @param c Pointer to the Color.
 */
void module_surfaceColor(Module *md, Color *c);

/**
 * Adds the specular coefficient to the tail of the module’s list.
 *
 * @param md Pointer to the Module.
 * @param coeff Specular coefficient.
 */
void module_surfaceCoeff(Module *md, float coeff);
