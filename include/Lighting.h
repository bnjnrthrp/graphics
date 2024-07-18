#ifndef LIGHTING_H

#define LIGHTING_H

#define MAX_LIGHTS 64

typedef enum LightType
{
    LightNone,
    LightAmbient,
    LightDirect,
    LightPoint,
    LightSpot,
} LightType;

typedef struct Light
{
    LightType type;
    Color color;
    Vector direction;
    Point position;
    float cutoff;    // Stores the cosine of the cutoff angle of a spotlight
    float sharpness; // Coefficient of the falloff function (power of cosine)
} Light;

typedef struct Lighting
{
    int nLights;
    Light light[MAX_LIGHTS];
} Lighting;
#endif // LIGHTING_H
