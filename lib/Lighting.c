/**
 * Library file for dealing with light sources in the API
 *
 * @author Benji Northrop
 */

#include <stdlib.h>
#include <math.h>
#include "Lighting.h"

/**
 * Initializes a light to default values
 *
 * @param light the light to initialize
 */
void light_init(Light *light)
{
    if (!light)
    {
        fprintf(stderr, "Null pointer provided to light_init\n");
        exit(-1);
    }
    light->type = LightAmbient;
    color_set(&(light->color), 0.0, 0.0, 0.0);
    vector_set(&(light->direction), 0.0, 0.0, 0.0);
    point_set3D(&(light->position), 1.0, 1.0, 1.0);
    light->cutoff = 0.0;
    light->sharpness = 0.0;
}

/**
 * Copies a light from one to another
 */
void light_copy(Light *to, Light *from)
{
    if (!to || !from)
    {
        fprintf(stderr, "Null pointer provided to light_copy\n");
        exit(-1);
    }
    to->type = from->type;
    color_copy(&(to->color), &(from->color));
    vector_copy(&(to->direction), &(from->direction));
    point_copy(&(to->position), &(from->position));
    to->cutoff = from->cutoff;
    to->sharpness = from->sharpness;
}

/**
 * Creates a lighting struct and allocates memory for it
 */
Lighting *lighting_create(void)
{
    Lighting *lighting = (Lighting *)malloc(sizeof(Lighting));
    lighting->nLights = 0;
    lighting_init(lighting);

    return lighting;
}

/**
 * Deletes the lighting struct and its memory
 *
 * @param lights the lightning struct to delete
 */
void lighting_delete(Lighting *lights)
{
    if (!lights)
        fprintf(stderr, "Null pointer provided to light_delete\n");
    else
        free(lights);
}

/**
 * Initializes a lighting struct
 */
void lighting_init(Lighting *l)
{
    if (!l)
    {
        fprintf(stderr, "Null pointer provided to lighting_init\n");
        exit(-1);
    }
    // If there's already data in the lighting array, clear it and reset
    if (l->nLights > 0)
    {
        lighting_clear(l);
    }
}

/**
 * Clears a lighting struct and resets it to default
 */
void lighting_clear(Lighting *l)
{
    if (!l)
    {
        fprintf(stderr, "Null pointer provided to lighting_clear\n");
        exit(-1);
    }
    if (l->nLights == 0)
    {
        return;
    }
    for (int i = 0; i < l->nLights; i++)
    {
        light_init(&(l->light[i]));
    }
    l->nLights = 0;
}

/**
 * Adds a light to the lighting struct
 *
 * @param l the lighting struct to add to
 * @param type the type of light to add
 * @param c the color of the light
 * @param dir the direction of the light
 * @param pos the position of the light
 * @param cutoff the cutoff value of the light
 * @param sharpness the sharpness of the light cutoff
 */
void lighting_add(Lighting *l, LightType type, Color *c, Vector *dir, Point *pos, float cutoff, float sharpness)
{
    if (!l || !c)
    {
        fprintf(stderr, "Null pointer provided to lighting_add\n");
        exit(-1);
    }
    // Check there's room for more lights
    if (l->nLights == MAX_LIGHTS)
    {
        fprintf(stderr, "You've reached the max capacity for lights\n");
        return;
    }

    // Update the light at the nLights index (next available)
    l->light[l->nLights].type = type;
    color_copy(&(l->light[l->nLights].color), c);
    if (dir)
        vector_copy(&(l->light[l->nLights].direction), dir);
    if (pos)
        point_copy(&(l->light[l->nLights].position), pos);
    l->light[l->nLights].cutoff = cutoff;
    l->light[l->nLights].sharpness = sharpness;

    // Increment the number of lights
    l->nLights = l->nLights + 1;

    // Calculate and print the ambient light values

    // Calculate and print the ambient + point light sums
}

/**
 * Adjusts the shading of a lighting object
 * @param l the lighting struct
 * @param N the surface normal
 * @param V the view vector
 * @param p the point of the surface
 * @param Cb the body coefficient
 * @param Cs the surface coefficient
 * @param s the sharpness value
 * @param oneSided if the polygon is onesided
 * @param c the final color destination
 */
void lighting_shading(Lighting *l, Vector *N, Vector *V, Point *p, Color *Cb,
                      Color *Cs, float s, int oneSided, Color *c)
{
    if (!l || !N || !V || !p || !Cb || !Cs || !c)
    {
        fprintf(stderr, "Null pointer provided to lighting_shading\n");
        exit(-1);
    }
    // Loop through every light
    float tmpR, tmpG, tmpB;
    float ambR, ambG, ambB;

    // Store the values of the original color into temporary variables
    ambR = ambG = ambB = tmpR = tmpG = tmpB = 0.0;
    Vector Ds, L, H, negL;
    double theta, sigma, beta, t;
    for (int i = 0; i < l->nLights; i++)
    {

        // Determine type of light
        switch (l->light[i].type)
        {
        case LightAmbient:
            // printf("Adding ambient portion\n");
            tmpR += l->light[i].color.c[0] * Cb->c[0];
            tmpG += l->light[i].color.c[1] * Cb->c[1];
            tmpB += l->light[i].color.c[2] * Cb->c[2];

            // For debugging purposes
            ambR += l->light[i].color.c[0] * Cb->c[0];
            ambG += l->light[i].color.c[1] * Cb->c[1];
            ambB += l->light[i].color.c[2] * Cb->c[2];
            continue;
            break;
        case LightDirect:
            // Calculate L
            vector_set(&L, -(l->light[i].direction.val[0]), -(l->light[i].direction.val[1]), -(l->light[i].direction.val[2]));
            // Normalize L
            vector_normalize(&L);
            break;

        case LightPoint:
            // Calculate L = Ps - P
            vector_set(&L, (l->light[i].position.val[0] - p->val[0]), (l->light[i].position.val[1] - p->val[1]), (l->light[i].position.val[2] - p->val[2]));
            // Normalize L
            vector_normalize(&L);
            break;

        case LightSpot:
            // Calculate L = Ps - P
            vector_set(&L, (l->light[i].position.val[0] - p->val[0]), (l->light[i].position.val[1] - p->val[1]), (l->light[i].position.val[2] - p->val[2]));
            // Normalize L
            vector_normalize(&L);

            vector_set(&negL, -L.val[0], -L.val[1], -L.val[2]);
            vector_set(&Ds, l->light[i].direction.val[0], l->light[i].direction.val[1], l->light[i].direction.val[2]);
            t = vector_dot(&negL, &Ds);
            if (t < pow(l->light[i].cutoff, l->light[i].sharpness))
                continue;

            break;
        default:
            break;
        }
        // Universal calculations
        // Calculate theta = L * N
        theta = vector_dot(&L, N);
        // Check if light is on facing side of polygon if polygon is one-sided - skip if true
        if (oneSided && theta < 0)
            continue;

        // Normalize the view point
        vector_normalize(V);
        sigma = vector_dot(V, N);

        // Check if viewer and light source on same side of surface?
        if ((theta < 0 && sigma > 0) || (theta > 0 && sigma < 0))
            continue;

        // Calculate H = (L + V) / 2
        vector_set(&H, (L.val[0] + V->val[0]) / 2.0, (L.val[1] + V->val[1]) / 2.0, (L.val[2] + V->val[2]) / 2.0);
        // Calculate beta = H * N
        beta = vector_dot(&H, N);
        // printf("beta is: %.2f\n", beta);
        if (theta < 0 && oneSided == 0)
        {
            // negate theta and beta
            theta = -theta;
            beta = -beta;
        }
        // Calculate shading using theta, beta, n, light, color, Cb, Cs
        tmpR += (Cb->c[0] * l->light[i].color.c[0]) * theta + (l->light[i].color.c[0] * Cs->c[0]) * pow(beta, s);
        tmpG += (Cb->c[1] * l->light[i].color.c[1]) * theta + (l->light[i].color.c[1] * Cs->c[1]) * pow(beta, s);
        tmpB += (Cb->c[2] * l->light[i].color.c[2]) * theta + (l->light[i].color.c[2] * Cs->c[2]) * pow(beta, s);
    }
    // Debug only
    // printf("ambient light 0: %.2f, %.2f, %.2f\n", ambR, ambG, ambB);
    // printf("point light 1: %.2f, %.2f, %.2f\n", tmpR, tmpG, tmpB);
    color_set(c, tmpR, tmpG, tmpB);
}
