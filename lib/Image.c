/**
 * This file represents an image, which contains a size (rows and columns), data (pixels), and other
 * various attributes.
 *
 * @author Benji Northrop
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../include/Image.h"

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
        return NULL;
    }

    image_init(src);

    if (rows <= 0 || cols <= 0) // If provided data invalid, return NULL
    {
        return NULL;
    }

    int alloc = image_alloc(src, rows, cols);
    if (alloc != 0)
    {
        fprintf(stderr, "Something failed in the alloc\n");
        return NULL;
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
    src->a = 0;
    src->z = 0;
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
    if (rows <= 0 || cols <= 0)
    {
        fprintf(stderr, "invalid rows/cols input\n");
        return 1;
    }

    // Set the data fields to defaults.
    src->rows = rows;
    src->cols = cols;
    src->a = 1.0;
    src->z = 1.0;
    src->maxval = 1.0;

    // Allocate the row pointers
    src->data = (FPixel **)malloc(sizeof(FPixel *) * rows);
    if (src->data == NULL)
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
    for (int i = 1; i < rows - 1; i++)
    {
        src->data[i] = &(src->data[0][i * cols]);
    }

    // Initialize the pixel values in the array
    for (int i = 0; i < rows * cols; i++)
    {
        src->data[0][i].rgb[0] = 0;
        src->data[0][i].rgb[1] = 0;
        src->data[0][i].rgb[2] = 0;
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
    image_dealloc(src);
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
    free(src->data[0]);
    // Free the array of pointers
    free(src->data);

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
    char tag[40];
    Image *image;
    FILE *fp;
    int read, num[3], curchar, rows, cols, colors;

    if (filename != NULL && strlen(filename))
    {
        fp = fopen(filename, "r");
    }
    else
    {
        fp = stdin;
    }

    if (fp)
    {
        fscanf(fp, "%s\n", tag);

        // Read the "magic number" at the beginning of the ppm
        if (strncmp(tag, "P6", 40) != 0)
        {
            fprintf(stderr, "not a ppm!\n");
            exit(1);
        }

        // Read the rows, columns, and color depth output by cqcam
        // need to read in three numbers and skip any lines that start with a #
        read = 0;
        while (read < 3)
        {
            curchar = fgetc(fp);
            if ((char)curchar == '#')
            { // skip this line
                while (fgetc(fp) != '\n')
                    /* do nothing */;
            }
            else
            {
                ungetc(curchar, fp);
                fscanf(fp, "%d", &(num[read]));
                read++;
            }
        }
        while (fgetc(fp) != '\n')
            /* pass the last newline character */;

        cols = num[0];
        rows = num[1];
        colors = num[2];

        if (cols > 0 && rows > 0)
        {
            image = image_create(rows, cols);

            if (image)
            {
                fread(image, sizeof(Image), rows * cols, fp);

                if (fp != stdin)
                {
                    fclose(fp);
                }
                return image;
            }
        }
    }

    return NULL;
};
/**
 * writes a PPM image to the given filename. Returns 0 on success.
 * Optionally, you can look at the filename extension and write different
 * file types.
 * @param src the image to write
 * @param filename the output filename
 * @return 0 if successful
 */
int image_write(Image *src, int rows, int cols, int colors, char *filename)
{
    FILE *fp;

    if (filename != NULL && strlen(filename))
        fp = fopen(filename, "w");
    else
        fp = stdout;

    if (fp)
    {
        fprintf(fp, "P6\n");
        fprintf(fp, "%d %d\n%d\n", cols, rows, colors);

        fwrite(src, sizeof(Image), rows * cols, fp);
    }

    fclose(fp);
};
