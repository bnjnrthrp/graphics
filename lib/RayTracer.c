#include "RayTracer.h"
#include <stdlib.h>

#define DEFAULT 1000

RayTracer *rayTracer_create()
{
    RayTracer *rt = (RayTracer *)malloc(sizeof(RayTracer));
    rayTracer_init(rt, DEFAULT);
    return rt;
}

void rayTracer_init(RayTracer *rt, int size)
{
    if (!rt)
    {
        fprintf(stderr, "Invalid pointer sent to rayTracer_init\n");
        exit(-1);
    }
    if (rt->db)
        free(rt->db);
    if (size < 0)
        return;
    rt->db = (Polygon *)malloc(sizeof(Polygon) * size);
    rt->size = 0;
    rt->max = size;
}
void rayTracer_add(RayTracer *rt, Polygon *p)
{
    if (!rt || !p)
    {
        fprintf(stderr, "Invalid pointer provided to rayTracer_add\n");
        exit(-1);
    }

    if (rt->size >= rt->max) // Check if database is full and needs to expand
        rayTracer_expand(rt);
    polygon_copy(&(rt->db[rt->size]), p);
    // printf("Adding polygon: ");
    // polygon_print(p, stdout);
    rt->size++;
}
void rayTracer_expand(RayTracer *rt)
{
    if (!rt)
    {
        fprintf(stderr, "Invalid pointer sent to rayTracer_expand\n");
        exit(-1);
    }
    Polygon *tmp = (Polygon *)malloc(sizeof(Polygon) * rt->size * 2); // Double the size of the polygon array
    for (int i = 0; i < rt->size; i++)
    {
        polygon_copy(&tmp[i], &rt->db[i]); // Copy the polygons to the new array
    }
    free(rt->db); // Free the old polygon array
    rt->db = tmp;
    rt->max = rt->size * 2;
}
void rayTracer_dbFree(RayTracer *rt)
{
    if (!rt)
    {
        fprintf(stderr, "Invalid pointer sent to rayTracer_free\n");
        exit(-1);
    }

    if (rt->db)
    {
        free(rt->db);
    }
    free(rt);
}

/**
 * Traverses the ray tracer polygon database and finds the closest polygon given an anchor point and a vector
 *
 * @param src the start point
 * @param rt the rayTracer struct, contains the polygon database
 * @param Vij the vector from the start point towards the polygons
 * @param x the resulting intercept point on the polygon, if there's a hit
 * @return a pointer to the polygon that intersects the ray
 */
Polygon *rayTracer_closestPolygon(Point *src, RayTracer *rt, Vector *Vij, Point *x)
{
    if (!src || !Vij || !rt)
    {
        fprintf(stderr, "Invalid pointer provided to closest_polygon\n");
        exit(-1);
    }
    int i, hit;
    Polygon *closest = NULL;
    Point p0, currX;
    Vector anchor;
    double t, VdotN, currT;

    currT = 0.0;

    for (i = 0; i < rt->size; i++)
    {
        vector_normalize(Vij);
        vector_normalize(&(rt->db[i].normalPhong[0]));
        // If the polygon is nearly parallel to the ray, skip it
        VdotN = vector_dot(Vij, &(rt->db[i].normalPhong[0]));
        printf("source: ");
        point_print(src, stdout);
        printf("Vij:");
        vector_print(Vij, stdout);
        printf("Normal: ");
        vector_print(&rt->db[i].normalPhong[0], stdout);
        printf("VdotN is %.5f\n", VdotN);
        if (VdotN > -.001 && VdotN < .001)
        {
            vector_print(Vij, stdout);
            vector_print(&rt->db[i].normalPhong[0], stdout);
            printf("VdotN is %.5f\n", VdotN);
            printf("Plane is near parallel, skip\n");
            continue;
        }

        // Adjust the start by a little bit down the vector
        // printf("Original point: ");
        // point_print(src, stdout);
        point_set3D(src, src->val[0] + 0.05 * Vij->val[0], src->val[1] + 0.05 * Vij->val[1], src->val[2] + 0.05 * Vij->val[2]);
        // printf("Adjusted point: ");
        // point_print(src, stdout);

        // Calculate if the ray will intersect the polygon plane, then the polygon
        // Determine number of sides in the polygon
        if (rt->db[i].nVertex == 3)
        {
            //   printf("testing triangle\n");
            hit = ray_triIntersect(src, &(rt->db[i]), Vij, &currX);
        }
        else if (rt->db[i].nVertex == 4)
        {
            //   printf("splitting\n");
            // Split 4 sided polygon and do ray-triangle intersections on both
            Point pt1[3];
            Point pt2[3];
            Vector n1[3];
            Vector n2[3];
            Polygon p1, p2;

            polygon_init(&p1);
            polygon_init(&p2);

            polygon_copy(&p1, &rt->db[i]);
            polygon_copy(&p2, &rt->db[i]);

            point_copy(&pt1[0], &(rt->db[i].vertex3D[0]));
            point_copy(&pt1[1], &(rt->db[i].vertex3D[1]));
            point_copy(&pt1[2], &(rt->db[i].vertex3D[3]));
            vector_copy(&n1[0], &(rt->db[i].normalPhong[0]));
            vector_copy(&n1[1], &(rt->db[i].normalPhong[1]));
            vector_copy(&n1[2], &(rt->db[i].normalPhong[3]));

            point_copy(&pt2[0], &(rt->db[i].vertex3D[1]));
            point_copy(&pt2[1], &(rt->db[i].vertex3D[2]));
            point_copy(&pt2[2], &(rt->db[i].vertex3D[3]));
            vector_copy(&n2[0], &(rt->db[i].normalPhong[1]));
            vector_copy(&n2[1], &(rt->db[i].normalPhong[2]));
            vector_copy(&n2[2], &(rt->db[i].normalPhong[3]));

            polygon_set(&p1, 3, pt1);
            polygon_setVertex3D(&p1, 3, pt1);
            polygon_setNormalsPhong(&p1, 3, n1);
            polygon_set(&p2, 3, pt2);
            polygon_setVertex3D(&p2, 3, pt2);
            polygon_setNormalsPhong(&p2, 3, n2);

            // printf("sending polygons to ray_triIntersect: ");
            // polygon_print(&p1, stdout);
            // polygon_print(&p2, stdout);
            // exit(-1);
            // printf("Sending the two polygons to ray_triIntersect\n");
            hit = ray_triIntersect(src, &p1, Vij, &currX) * ray_triIntersect(src, &p2, Vij, &currX);
        }
        else
        {
            continue; // Cannot process polygons > size 4;
            /*
            // Ray Plane intersection

             */
        }
        if (hit == 0)
        {
            p0 = rt->db[i].vertex3D[0];
            vector_setPoints(&anchor, src, &p0);
            vector_normalize(&anchor);
            vector_normalize(&(rt->db[i].normalPhong[0]));
            printf("anchor is: ");
            vector_print(&anchor, stdout);
            t = vector_dot(&anchor, &(rt->db[i].normalPhong[0])) / VdotN;
            printf("t is %.5f\n", t);

            // ensure t is positive
            if (t > 0)
            {
                if (!closest)
                {
                    closest = &(rt->db[i]);
                    currT = t;
                    point_copy(x, &currX);
                    // printf("Adding closest polygon at t = %.5f: ", currT);
                    // polygon_print(closest, stdout);
                }
                else if (t < currT)
                {
                    closest = &(rt->db[i]);
                    currT = t;
                    point_copy(x, &currX);
                    // printf("Adding closest polygon at t = %.5f: ", t);
                    // polygon_print(closest, stdout);
                }
            }
            // printf("ray: %d, dist: %.5f\n", hit, currT);
            // polygon_print(closest, stdout);
        }
        else
        {
            // printf("Ray missed this polygon: ");
            // polygon_print(&rt->db[i], stdout);
        }
    }
    return closest;
}

/**
 * Determines if a ray will intersect a provided polygon. Currently only supports 3 or 4 sided polygons
 * @param src the anchor of the ray
 * @param p the polygon in question
 * @param Vij the vector from the anchor towards the polygon
 * @param x the intersection point on the polygon
 */
int ray_triIntersect(Point *src, Polygon *p, Vector *Vij, Point *x)
{
    Vector edgeAB, edgeBC, edgeCA, edgeAX, edgeBX, edgeCX, cross1, cross2, cross3, ray;
    double t, dotA, dotB, dotC;

    if (!src || !p || !Vij || !x)
    {
        fprintf(stderr, "Invalid pointer provided to ray_triIntersect\n");
        exit(-1);
    }
    Vector N;
    vector_copy(&N, &(p->normalPhong[0]));
    vector_normalize(&N);
    vector_normalize(Vij);
    // Assumes the normal is the same through the polygon.
    // polygon_print(p, stdout);
    vector_setPoints(&ray, src, &p->vertex3D[0]);
    // vector_normalize(&ray);
    // polygon_print(p, stdout);
    // printf("normal: ");
    // printf("%.2f, %.2f, %.2f\n", p->normalPhong[0].val[0], p->normalPhong[0].val[1], p->normalPhong[0].val[2]);
    // printf("Vij: ");
    // vector_print(Vij, stdout);
    t = vector_dot(&ray, &N) / vector_dot(Vij, &N);

    vector_calcParametric(src, t, Vij, x);
    printf("A: ");
    point_print(src, stdout);
    printf("t is %.5f\n", t);
    printf("V: ");
    vector_print(Vij, stdout);
    printf("x: ");
    point_print(x, stdout);

    Point intersection = {{src->val[0] + t * Vij->val[0], src->val[1] + t * Vij->val[1], src->val[2] + t * Vij->val[2]}};

    vector_setPoints(&edgeAB, &(p->vertex3D[0]), &(p->vertex3D[1]));
    vector_setPoints(&edgeBC, &(p->vertex3D[1]), &(p->vertex3D[2]));
    vector_setPoints(&edgeCA, &(p->vertex3D[2]), &(p->vertex3D[0]));
    vector_setPoints(&edgeAX, &(p->vertex3D[0]), x);
    vector_setPoints(&edgeBX, &(p->vertex3D[1]), x);
    vector_setPoints(&edgeCX, &(p->vertex3D[2]), x);

    vector_normalize(&edgeAB);
    vector_normalize(&edgeBC);
    vector_normalize(&edgeCA);
    vector_normalize(&edgeAX);
    vector_normalize(&edgeBX);
    vector_normalize(&edgeCX);

    vector_cross(&edgeAB, &edgeAX, &cross1);
    vector_cross(&edgeBC, &edgeBX, &cross2);
    vector_cross(&edgeCA, &edgeCX, &cross3);
    printf("cross1: ");
    vector_print(&cross1, stdout);
    printf("cross2: ");
    vector_print(&cross2, stdout);
    printf("cross3: ");
    vector_print(&cross3, stdout);

    dotA = vector_dot(&edgeAB, &edgeAX);
    dotB = vector_dot(&edgeBC, &edgeBX);
    dotC = vector_dot(&edgeCA, &edgeCX);

    // Check that the vector component is non-zero, else try another component
    // If all three like-components are of the same sign, then the ray intersects inside the polygon
    if ((dotA < 0 && dotB < 0 && dotC < 0) || (dotA > 0 && dotB > 0 && dotC > 0))
    {
        printf("hit: ");
        printf("%.5f, %.5f, %.5f\n", dotA, dotB, dotC);
        vector_print(Vij, stdout);
        return 0;
    }
    else
    {
        printf("Miss: ");
        printf("%.5f, %.5f, %.5f\n", dotA, dotB, dotC);
        vector_print(Vij, stdout);
        return 1;
    }
}

/**
 * Calculates the t of the parametric equation dest = A + tV
 *
 * @param src the anchor point
 * @param dest the destination point
 * @param V the vector from the anchor to the destination
 */
float ray_calculateT(Point *src, Point *dest, Vector *V)
{
    if (!src || !dest || !V)
    {
        fprintf(stderr, "Invalid pointer provided to ray_calculateT\n");
        exit(-1);
    }
    double t;
    vector_normalize(V);

    for (int i = 0; i < 3; i++)
    {
        if (V->val[i] == 0) // Avoid divide by 0
            continue;
        t = (dest->val[i] - src->val[i]) / V->val[i];

        if (t != 0.0) // Avoid calculating when points are co-planar
            return t;
    }
    return 0.0;
}
