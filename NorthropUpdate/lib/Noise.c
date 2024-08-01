/**
 * This is the Perlin Noise algorithm, designed by Ken Perlin. Originally written in Java, it has been adapted to C
 * Source: https://mrl.cs.nyu.edu/~perlin/noise/
 *
 * @author Ken Perlin
 * Adapted to C by Benji Northrop
 */

#include <math.h>
#include <stdio.h>
#include "Noise.h"

/**
 * Computes the fade curve value for a given input.
 *
 * This function implements the fade curve for Perlin noise.
 * The fade curve smooths the input value to avoid discontinuities in the noise function's derivative, ensuring smooth transitions
 * between grid points.
 *
 * @param t The input value, typically in the range [0, 1].
 * @return The smoothed output value.
 */
float fade(float t) { return t * t * t * (t * (t * 6 - 15) + 10); }

/**
 * Performs linear interpolation between two values.
 *
 * This function calculates the linear interpolation between two values a and b
 * based on a given ratio t. The parameter t should be between 0 and 1,
 * where t = 0 returns a, and t = 1 returns b. Intermediate values of t
 * yield values proportionally between a and b.
 *
 * @param t The interpolation parameter, usually in the range [0, 1].
 * @param a The start value.
 * @param b The end value.
 * @return The interpolated value.
 */
float lerp(float t, float a, float b) { return a + t * (b - a); }

/**
 * Computes the gradient vector dot product for Perlin noise.
 *
 * This function generates a pseudo-random gradient vector from a given hash value
 * and then computes the dot product of this gradient vector with the given point (x, y, z).
 * The gradient vectors are chosen based on the hash value, which is used to index
 * a set of pre-defined vectors.
 *
 * @param hash An integer hash value used to select a gradient vector.
 * @param x The x-coordinate of the point.
 * @param y The y-coordinate of the point.
 * @param z The z-coordinate of the point.
 * @return The dot product of the selected gradient vector and the point (x, y, z).
 *
 */
float grad(int hash, float x, float y, float z)
{
    int h;
    float u, v;
    h = hash & 15;

    if (h < 8)
        u = x;
    else
        u = y;

    if (h < 4)
    {
        v = y;
    }
    else if (h == 12 || h == 14)
    {
        v = x;
    }
    else
    {
        v = z;
    }
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

/**
 * Generates Perlin noise for the given 3D coordinates.
 *
 * This function computes the Perlin noise value at a specific point (x, y, z) in 3D space.
 *
 * @param x The x-coordinate of the point.
 * @param y The y-coordinate of the point.
 * @param z The z-coordinate of the point.
 * @return The Perlin noise value at the specified coordinates, in the range [-1, 1].
 */
float perlin(float x, float y, float z)
{
    double u, v, w;
    int xi, yi, zi, A, B, AA, AB, BA, BB;

    int p[512] = {151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225,
                  140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23, 190, 6, 148,
                  247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32,
                  57, 177, 33, 88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175,
                  74, 165, 71, 134, 139, 48, 27, 166, 77, 146, 158, 231, 83, 111, 229, 122,
                  60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244, 102, 143, 54,
                  65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169,
                  200, 196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64,
                  52, 217, 226, 250, 124, 123, 5, 202, 38, 147, 118, 126, 255, 82, 85, 212,
                  207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42, 223, 183, 170, 213,
                  119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
                  129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104,
                  218, 246, 97, 228, 251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241,
                  81, 51, 145, 235, 249, 14, 239, 107, 49, 192, 214, 31, 181, 199, 106, 157,
                  184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254, 138, 236, 205, 93,
                  222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180, ///// Repeats after this line
                  151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225,
                  140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23, 190, 6, 148,
                  247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32,
                  57, 177, 33, 88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175,
                  74, 165, 71, 134, 139, 48, 27, 166, 77, 146, 158, 231, 83, 111, 229, 122,
                  60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244, 102, 143, 54,
                  65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169,
                  200, 196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64,
                  52, 217, 226, 250, 124, 123, 5, 202, 38, 147, 118, 126, 255, 82, 85, 212,
                  207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42, 223, 183, 170, 213,
                  119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
                  129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104,
                  218, 246, 97, 228, 251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241,
                  81, 51, 145, 235, 249, 14, 239, 107, 49, 192, 214, 31, 181, 199, 106, 157,
                  184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254, 138, 236, 205, 93,
                  222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180};

    // Takes each number and does bitwise AND with 255
    //  i.e. xi = the binary number when BOTH x AND 255 both have the bit 1.
    xi = (int)floor(x) & 255;
    yi = (int)floor(y) & 255;
    zi = (int)floor(z) & 255;

    // Find relative x, y, z, of point in cube
    x -= floor(x);
    y -= floor(y);
    z -= floor(z);

    // Compute the fade curves for each of x, y, z
    u = fade(x);
    v = fade(y);
    w = fade(z);

    // Hash coordinates of the 8 cube corners
    A = p[xi] + yi;
    AA = p[A] + zi;
    AB = p[A + 1] + zi;
    B = p[xi + 1] + yi;
    BA = p[B] + zi;
    BB = p[B + 1] + zi; // The 8 cube corners,

    // Add and return blended results from 8 corners of the cube
    return lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z), grad(p[BA], x - 1, y, z)), lerp(u, grad(p[AB], x, y - 1, z), grad(p[BB], x - 1, y - 1, z))),
                lerp(v, lerp(u, grad(p[AA + 1], x, y, z - 1), grad(p[BA + 1], x - 1, y, z - 1)),
                     lerp(u, grad(p[AB + 1], x, y - 1, z - 1),
                          grad(p[BB + 1], x - 1, y - 1, z - 1))));
}

/**
 * Adds Perlin noise to an image.
 *
 * This function applies Perlin noise to each pixel of the given image.
 * The noise values are computed based on the pixel's position and a scale factor.
 * The noise is then converted to greyscale and set to the pixel.
 *
 * @param dst A pointer to the image to which noise will be added.
 * @param noiseWidth The scaling factor for the noise, affecting the frequency of the noise pattern.
 */
void add_noise(Image *dst, int noiseWidth)
{
    float dx, dy, value;
    FPixel pixel;

    for (int i = 0; i < dst->rows; i++)
    {
        for (int j = 0; j < dst->cols; j++)
        {
            dx = (float)i / (float)dst->cols;
            dy = (float)j / (float)dst->rows;
            value = perlin(dx * noiseWidth, dy * noiseWidth, dst->z[i * dst->cols + j]);
            value = (value + 1) / 2;
            // Red Noise only
            // pixel.rgb[0] = 1;
            // pixel.rgb[1] = 220 * value / 255;
            // pixel.rgb[2] = 0;

            // Greyscale noise
            pixel.rgb[0] = value;
            pixel.rgb[1] = value;
            pixel.rgb[2] = value;
            image_setf(dst, i, j, pixel);
        }
    }
}
