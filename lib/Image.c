/**
 * This file represents an image, which contains a size (rows and columns), data (pixels), and other
 * various attributes.
 *
 * @author Benji Northrop
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "Image.h"
#include "Color.h"

/**
 * Allocates an Image structure and initializes
 * the top level fields to appropriate values. Allocates space for an image of the specified size, unless
 * either rows or cols is 0.
 * @param rows the number of rows in the image
 * @param cols the number of cols in the image
 * @return a pointer to the allocated Image structure. Returns a NULL pointer if
 * the operation fails.
 */
Image *image_create(int rows, int cols)
{
    // allocate the struct
    Image *src = (Image *)malloc(sizeof(Image));
    if (src == NULL) // null check
    {
        fprintf(stderr, "Error creating the image\n");
        exit(-1);
    }

    image_init(src);

    if (rows < 0 || cols < 0) // If provided data invalid, return NULL
    {
        fprintf(stderr, "Invalid input rows/cols\n");
        exit(-1);
    }

    int alloc = image_alloc(src, rows, cols);
    if (alloc != 0)
    {
        fprintf(stderr, "Something failed in the alloc\n");
        exit(-1);
    }

    return src;
};

/**
 * Given an uninitialized Image structure, sets the rows and cols
 * fields to zero and the data field to NULL.
 * @param src the Image pointer
 */
void image_init(Image *src)
{
    src->data = NULL;
    src->rows = 0;
    src->cols = 0;
    src->a = NULL;
    src->z = NULL;
    src->maxval = 1;

    for (int i = 0; i < MAX_FILENAME_LENGTH; i++)
    {
        src->filename[i] = 0;
    };
};

/**
 * Allocates space for the image data
 * given rows and columns and initializes the image data to appropriate values,
 * such as 0.0 for RGB and 1.0 for A and Z.
 * This function should free existing memory if rows and cols are both non-zero.
 *
 * @param src pointer to the Image
 * @param rows the number of rows to allocate
 * @param cols the number of cols to allocate
 * @return 0 if the operation is successful. Returns a non-zero value if the operation fails.
 */
int image_alloc(Image *src, int rows, int cols)
{
    // Free the data if there is already something here.
    if (src->rows > 0 && src->cols > 0)
    {
        free(src->data[0]);
    }
    // Return error code if there was invalid input
    if (rows < 0 || cols < 0)
    {
        fprintf(stderr, "invalid rows/cols input\n");
        return 1;
    }

    // Set the data fields to defaults.
    src->rows = rows;
    src->cols = cols;
    src->maxval = 1.0;

    // Allocate the row pointers, a channel, and z channel arrays
    src->data = (FPixel **)malloc(sizeof(FPixel *) * rows);
    src->a = (float *)malloc(sizeof(float) * rows * cols);
    src->z = (float *)malloc(sizeof(float) * rows * cols);
    if (src->data == NULL || src->a == NULL || src->z == NULL)
    {
        fprintf(stderr, "Data allocation failed\n");
        return -1;
    }

    // Allocate the 1D array in the first row
    src->data[0] = (FPixel *)malloc(sizeof(FPixel) * rows * cols);
    if (src->data[0] == NULL)
    {
        fprintf(stderr, "1D Array allocation failed\n");
    }

    // Set the row pointers to the appropriate row
    for (int i = 1; i < rows; i++)
    {
        src->data[i] = &(src->data[0][i * cols]);
    }

    // Initialize the pixel values in the array
    for (int i = 0; i < rows * cols; i++)
    {
        src->data[0][i].rgb[0] = 0;
        src->data[0][i].rgb[1] = 0;
        src->data[0][i].rgb[2] = 0;
        src->a[i] = 1.0;
        src->z[i] = 1.0;
    }

    return 0;
};

/**
 * De-allocates image data and frees the Image structure.
 * @param src the source Image to free
 */
void image_free(Image *src)
{
    // Dealloc and free all of the internal data
    if (src->data)
    {
        image_dealloc(src);
    }
    // Free the struct
    free(src);
};

/**
 * De-allocates image data and resets the Image structure
 * fields. The function does not free the Image structure
 * @param src the source image
 */
void image_dealloc(Image *src)
{
    // Free the pixel data
    if (src->data[0])
        free(src->data[0]);
    // Free the array of pointers
    if (src->data)
        free(src->data);

    // Free the a and z channels
    if (src->a)
        free(src->a);
    if (src->z)
        free(src->z);

    // Reset the fields of the image
    image_init(src);
    return;
};

// input/output
/**
 * reads a PPM image from the given filename. An optional extension is to
 *  determine the image type from the filename and permit the use of different
 * file types. Initializes the alpha channel to 1.0 and the z channel
 * to 1.0. Returns a NULL pointer if the operation fails.
 *
 * @param filename the file name to read
 * @return image pointer if successful, otherwise NULL.
 */
Image *image_read(char *filename)
{
    Image *src = NULL;
    return src;
}
/**
 * Writes a PPM image to the given filename. Returns 0 on success.
 * Optionally, you can look at the filename extension and write different
 * file types.
 * @param src the image to write
 * @param filename the output filename
 * @return 0 if successful
 */
int image_write(Image *src, char *filename)
{
    FILE *fp;
    int rows, cols, colors;
    rows = src->rows;
    cols = src->cols;
    colors = float_to_int(src->maxval);

    if (filename != NULL && strlen(filename))
        fp = fopen(filename, "w");
    else
        fp = stdout;

    if (fp)
    {
        fprintf(fp, "P6\n");
        fprintf(fp, "%d %d\n%d\n", cols, rows, colors);

        fwrite(src->data[0], sizeof(FPixel), rows * cols, fp);
    }
    fclose(fp);
    return 0;
};

// Setters and getters
/**
 * Gets a pixel from a location
 * @param src the image
 * @param r the row index
 * @param c the col index
 * @return the Pixel at location r, c
 */
FPixel image_getf(Image *src, int r, int c)
{
    return src->data[r][c];
};

/**
 * Gets a color value from a specific pixel and channel
 * @param src the image
 * @param r the row index
 * @param c the col index
 * @param b the channel index [0,2]
 * @return the color channel in float format
 */
float image_getc(Image *src, int r, int c, int b)
{
    return src->data[r][c].rgb[b];
};

/**
 * Gets the a channel value from a specific pixel
 * @param src the image
 * @param r the row index
 * @param c the col index
 * @return the alpha channel in float format
 */
float image_geta(Image *src, int r, int c)
{
    return src->a[r * src->rows + c];
};
/**
 * Gets the z channel value from a specific pixel
 * @param src the image
 * @param r the row index
 * @param c the col index
 * @return the z channel in float format
 */
float image_getz(Image *src, int r, int c)
{
    return src->z[r * src->rows + c];
};

/**
 * sets the values of pixel (r,c) to the FPixel val.
 * If your FPixel contains just r, g, b values then this function should
 * not modify the corresponding alpha or z values.
 *
 * @param src the source image
 * @param r the row index
 * @param c the col index
 * @param val the new pixel values to use
 */
void image_setf(Image *src, int r, int c, FPixel val)
{
    for (int i = 0; i < 3; i++)
    {
        if (val.rgb[i] < 0)
        {
            src->data[r][c].rgb[i] = 0;
        }
        else if (val.rgb[i] > src->maxval)
        {
            src->data[r][c].rgb[i] = src->maxval;
        }
        else
        {
            src->data[r][c].rgb[i] = val.rgb[i];
        }
    }
};

/**
 * Sets the value of a specific color channel for a pixel
 * @param src the image
 * @param r the row index
 * @param c the col index
 * @param b the color channel index (RGB) [0, 2]
 * @param val the new value [0, 1]
 *
 */
void image_setc(Image *src, int r, int c, int b, float val)
{
    if (b < 0 || b > 2)
    {
        fprintf(stderr, "Invalid b channel index provided\n");
        return;
    }

    if (val < 0)
    {
        val = 0;
    }
    else if (val > src->maxval)
    {
        val = src->maxval;
    }
    src->data[r][c].rgb[b] = val;
};

/**
 * Sets the alpha channel value for a given position
 * @param src the image
 * @param r the row index
 * @param c the col index
 * @param val the new value in range [0, 1]
 */
void image_seta(Image *src, int r, int c, float val)
{
    if (val < 0)
    {
        val = 0;
    }
    else if (val > 1)
    {
        val = 1;
    }
    src->a[r * src->rows + c] = val;
};

/**
 * Sets the z channel value for a given position
 * @param src the image
 * @param r the row index
 * @param c the col index
 * @param val the new value in range [0, 1]
 */
void image_setz(Image *src, int r, int c, float val)
{
    if (val < 0)
    {
        val = 0;
    }
    else if (val > 1)
    {
        val = 1;
    }
    src->z[r * src->rows + c] = val;
};

// Utility
/**
 * Resets every pixel to a default value (Black, alpha value
 * of 1.0, z value of 1.0)
 * @param src the source image
 */
void image_reset(Image *src)
{
    int size = src->rows * src->cols;
    // Iterate every every pixel in the image
    for (int i = 0; i < size; i++)
    {
        // Set each channel to 0
        src->data[0][i].rgb[0] = 0;
        src->data[0][i].rgb[1] = 0;
        src->data[0][i].rgb[2] = 0;
        src->a[i] = 1.0;
        src->z[i] = 1.0;
        src->maxval = 1.0;
    }
};

/**
 * Sets every FPixel to the given value
 * @param src the source image
 * @param val the pixel data to copy into every pixel
 */
void image_fill(Image *src, FPixel val)
{
    int size = src->rows * src->cols;
    for (int i = 0; i < size; i++)
    {
        src->data[0][i].rgb[0] = val.rgb[0];
        src->data[0][i].rgb[1] = val.rgb[1];
        src->data[0][i].rgb[2] = val.rgb[2];
    }
};

/**
 * sets the (r, g, b) values of each pixel to the given color
 * @param src the image to fill
 * @param r the red channel value
 * @param g the green channel value
 * @param b the blue channel value
 */
void image_fillrgb(Image *src, float r, float g, float b)
{
    int size = src->rows * src->cols;
    // Check values and ensure within min/max
    if (r < 0)
    {
        r = 0;
    }
    else if (r > src->maxval)
    {
        r = src->maxval;
    }
    if (g < 0)
    {
        g = 0;
    }
    else if (g > src->maxval)
    {
        g = src->maxval;
    }
    if (b < 0)
    {
        b = 0;
    }
    else if (b > src->maxval)
    {
        b = src->maxval;
    }

    for (int i = 0; i < size; i++)
    {
        src->data[0][i].rgb[0] = r;
        src->data[0][i].rgb[1] = g;
        src->data[0][i].rgb[2] = b;
    }
};

/**
 * sets the alpha value of each pixel to the given value
 * @param src the image to fill
 * @param a the alpha channel to set
 */
void image_filla(Image *src, float a)
{
    int size = src->rows * src->cols;
    // Value check and clip illegal values to the respective min/max
    if (a < 0)
    {
        a = 0;
    }
    else if (a > 1.0)
    {
        a = 1.0;
    }
    for (int i = 0; i < size; i++)
    {
        src->a[i] = a;
    }
}

/**
 * sets the z value of each pixel to the given value
 * @param src the image to fill
 * @param z the z value to set
 */
void image_fillz(Image *src, float z)
{
    int size = src->rows * src->cols;
    // Value check and clip illegal values to the respective min/max
    if (z < 0)
    {
        z = 0;
    }
    else if (z > 1.0)
    {
        z = 1.0;
    }
    for (int i = 0; i < size; i++)
    {
        src->z[i] = z;
    }
};
