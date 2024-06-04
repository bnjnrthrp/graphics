/**
 * This program runs unit tests for the composite image lab
 *
 * @author Benji Northrop
 * CS5310
 * Summer 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include "../include/FPixel.h"
#include "../include/Image.h"
#include "testing_util.h"

#define EPSILON .0001

/******************** TESTS ******************/

/**
 * Test the newPixel() function.
 * @returns true if the test passed, false otherwise
 */
bool test_image_create()
{
    PRINT_DEBUG("Testing the image_create function\n");
    bool passed = true;
    Image *image = image_create(3, 3);
    for (int i = 0; i < image->rows * image->cols; i++)
    {
        passed &= ASSERT_EQUAL(image->data[0][i].rgb[0], 0);
        passed &= ASSERT_EQUAL(image->data[0][i].rgb[1], 0);
        passed &= ASSERT_EQUAL(image->data[0][i].rgb[2], 0);
        passed &= ASSERT_EQUAL(image->a[i], 1.0);
        PRINT_DEBUG("a channel %.1f\n", image->a[i]);
        passed &= ASSERT_EQUAL(image->z[i], 1.0);
        PRINT_DEBUG("z channel %.1f\n", image->z[i]);
    }
    passed &= ASSERT_EQUAL(image->rows, 3);
    PRINT_DEBUG("rows %d\n", image->rows);
    passed &= ASSERT_EQUAL(image->cols, 3);
    PRINT_DEBUG("cols %d\n", image->cols);
    // passed &= ASSERT_EQUAL(image->a, 1.0);
    // PRINT_DEBUG("a channel %.1f\n", image->a);
    // passed &= ASSERT_EQUAL(image->z, 1.0);
    // PRINT_DEBUG("z channel %.1f\n", image->z);
    for (int i = 0; i < MAX_FILENAME_LENGTH; i++)
    {
        passed &= ASSERT_EQUAL(image->filename[i], 0);
    }
    image_free(image);
    return passed;
}

bool test_image_init()
{
    PRINT_DEBUG("Testing the image_init function\n");
    bool passed = true;
    Image *image = (Image *)malloc(sizeof(Image)); // Setup
    image_init(image);
    passed &= ASSERT_NULL(image->data);
    passed &= ASSERT_EQUAL(image->rows, 0);
    PRINT_DEBUG("rows %d\n", image->rows);
    passed &= ASSERT_EQUAL(image->cols, 0);
    PRINT_DEBUG("cols %d\n", image->cols);

    for (int i = 0; i < MAX_FILENAME_LENGTH; i++)
    {
        passed &= ASSERT_EQUAL(image->filename[i], 0);
    }
    free(image); // Not an actual image, so free this malloc
    return passed;
}

bool test_image_alloc()
{
    PRINT_DEBUG("Testing the image_alloc function\n");
    bool passed = true;
    int rows, cols;
    rows = cols = 3;

    Image *image = image_create(rows, cols); // Setup

    PRINT_DEBUG("Checking the data in the Image\n");
    for (int i = 0; i < rows * cols; i++)
    {
        passed &= ASSERT_EQUAL(image->data[0][i].rgb[0], 0);
        passed &= ASSERT_EQUAL(image->data[0][i].rgb[1], 0);
        passed &= ASSERT_EQUAL(image->data[0][i].rgb[2], 0);
        passed &= ASSERT_EQUAL(image->a[i], 1.0);
        PRINT_DEBUG("a channel %.1f\n", image->a[i]);
        passed &= ASSERT_EQUAL(image->z[i], 1.0);
        PRINT_DEBUG("z channel %.1f\n", image->z[i]);
    }
    passed &= ASSERT_EQUAL(image->rows, 3);
    PRINT_DEBUG("rows %d\n", image->rows);
    passed &= ASSERT_EQUAL(image->cols, 3);
    PRINT_DEBUG("cols %d\n", image->cols);
    passed &= ASSERT_EQUAL(image->maxval, 1.0);
    PRINT_DEBUG("maxvalue %.1f\n", image->maxval);
    for (int i = 0; i < MAX_FILENAME_LENGTH; i++)
    {
        passed &= ASSERT_EQUAL(image->filename[i], 0);
    }
    image_free(image);
    return passed;
}

bool test_setf_getf()
{
    bool passed = true;
    PRINT_DEBUG("Creating new 3x3 image\n");
    Image *image = image_create(3, 3);

    // Setup new pixel
    PRINT_DEBUG("Creating new pixel with values 150, 150, 150\n");
    FPixel newPixel;
    newPixel.rgb[0] = (float)150 / 255;
    newPixel.rgb[1] = (float)150 / 255;
    newPixel.rgb[2] = (float)150 / 255;

    PRINT_DEBUG("Setting new pixel with values 150, 150, 150\n");
    image_setf(image, 1, 1, newPixel);

    FPixel actual = image_getf(image, 1, 1);
    float expected = (float)150 / 255;
    passed &= ASSERT_EQUAL(actual.rgb[0], expected);
    PRINT_DEBUG("actual is %.1f, expected is: %.1f\n", actual.rgb[0], (float)150 / 255);
    passed &= ASSERT_EQUAL(actual.rgb[1], expected);
    passed &= ASSERT_EQUAL(actual.rgb[2], expected);

    image_free(image);
    return passed;
}

bool test_setc_getc()
{
    bool passed = true;
    PRINT_DEBUG("Creating new 3x3 image\n");
    Image *image = image_create(3, 3);

    // Setup new pixel

    float negVal = -.5;
    float newVal = .6;
    float highVal = 1.5;

    PRINT_DEBUG("Setting pixel 1,1 with new value\n");
    image_setc(image, 1, 1, 0, negVal);
    image_setc(image, 1, 1, 1, newVal);
    image_setc(image, 1, 1, 2, highVal);

    float actualR = image_getc(image, 1, 1, 0);
    float actualG = image_getc(image, 1, 1, 1);
    float actualB = image_getc(image, 1, 1, 2);

    passed &= ASSERT_EQUAL(actualR, (float)0.0);
    PRINT_DEBUG("actual is %.1f, expected is: %.1f\n", actualR, (float)0);
    passed &= ASSERT_EQUAL(actualG, (float)0.6);
    PRINT_DEBUG("actual is %.1f, expected is: %.1f\n", actualG, 0.6);
    passed &= ASSERT_EQUAL(actualB, (float)1);
    PRINT_DEBUG("actual is %.1f, expected is: %.1f\n", actualB, (float)1);

    image_free(image);
    return passed;
}

bool test_setz_getz()
{
    bool passed = true;
    PRINT_DEBUG("Creating new 3x3 image\n");
    Image *image = image_create(3, 3);
    float actual;
    // Setup new pixel

    float negVal = -.5;
    float newVal = .6;
    float highVal = 1.5;

    PRINT_DEBUG("Setting pixel 1,1 with new value\n");
    image_setz(image, 1, 1, negVal);
    actual = image_getz(image, 1, 1);
    passed &= ASSERT_EQUAL(actual, (float)0);
    PRINT_DEBUG("actual is %.1f, expected is: %.1f\n", actual, (float)0);

    image_setz(image, 1, 1, newVal);
    actual = image_getz(image, 1, 1);
    passed &= ASSERT_EQUAL(actual, (float)0.6);
    PRINT_DEBUG("actual is %.1f, expected is: %.1f\n", actual, (float)0.6);

    image_setz(image, 1, 1, highVal);
    actual = image_getz(image, 1, 1);
    passed &= ASSERT_EQUAL(actual, (float)1);
    PRINT_DEBUG("actual is %.1f, expected is: %.1f\n", actual, (float)1.0);

    image_free(image);
    return passed;
}

bool test_seta_geta()
{
    bool passed = true;
    PRINT_DEBUG("Creating new 3x3 image\n");
    Image *image = image_create(3, 3);
    float actual;
    // Setup new pixel

    float negVal = -.5;
    float newVal = .6;
    float highVal = 1.5;

    PRINT_DEBUG("Setting pixel 1,1 with new value\n");
    image_seta(image, 1, 1, negVal);
    actual = image_geta(image, 1, 1);
    passed &= ASSERT_EQUAL(actual, (float)0);
    PRINT_DEBUG("actual is %.1f, expected is: %.1f\n", actual, (float)0);

    image_seta(image, 1, 1, newVal);
    actual = image_geta(image, 1, 1);
    passed &= ASSERT_EQUAL(actual, (float)0.6);
    PRINT_DEBUG("actual is %.1f, expected is: %.1f\n", actual, (float)0.6);

    image_seta(image, 1, 1, highVal);
    actual = image_geta(image, 1, 1);
    passed &= ASSERT_EQUAL(actual, (float)1);
    PRINT_DEBUG("actual is %.1f, expected is: %.1f\n", actual, (float)1.0);

    image_free(image);
    return passed;
}

bool test_utility()
{
    bool passed = true;
    PRINT_DEBUG("Creating new 3x3 image\n");
    Image *image = image_create(3, 3);
    int size = image->rows * image->cols;
    // Create and fill RGB with basic pixel
    FPixel newPixel;
    newPixel.rgb[0] = (float).5;
    newPixel.rgb[1] = (float).5;
    newPixel.rgb[2] = (float).5;

    image_fill(image, newPixel);
    for (int i = 0; i < size; i++)
    {
        passed &= ASSERT_EQUAL(image->data[0][i].rgb[0], (float).5);
        passed &= ASSERT_EQUAL(image->data[0][i].rgb[1], (float).5);
        passed &= ASSERT_EQUAL(image->data[0][i].rgb[2], (float).5);
    }
    // Fill RGB with middle values
    image_fillrgb(image, .25, .25, .25);
    for (int i = 0; i < size; i++)
    {
        passed &= ASSERT_EQUAL(image->data[0][i].rgb[0], (float).25);
        passed &= ASSERT_EQUAL(image->data[0][i].rgb[1], (float).25);
        passed &= ASSERT_EQUAL(image->data[0][i].rgb[2], (float).25);
    }

    // Fill a and z with middle values
    image_filla(image, .5);
    {
        for (int i = 0; i < size; i++)
        {
            passed &= ASSERT_EQUAL(image->a[i], (float).5);
        }
    }

    image_fillz(image, .25);
    {
        for (int i = 0; i < size; i++)
        {
            passed &= ASSERT_EQUAL(image->z[i], (float).25);
        }
    }

    // Reset all

    image_reset(image);
    for (int i = 0; i < size; i++)
    {
        passed &= ASSERT_EQUAL(image->data[0][i].rgb[0], 0);
        passed &= ASSERT_EQUAL(image->data[0][i].rgb[1], 0);
        passed &= ASSERT_EQUAL(image->data[0][i].rgb[2], 0);
        passed &= ASSERT_EQUAL(image->a[i], 1);
        passed &= ASSERT_EQUAL(image->z[i], 1);
    }
    image_free(image);
    return passed;
}

/*************************** end tests ***********/

/**
 * Adds every testing function to the testing set.
 * Includes the name of the test, the group tag, and the function to run.
 */
TestingSet *
init_testing_set()
{
    TestingSet *set = new_testing_set();
    add_test(set, "testing image_create, creating a new image", test_image_create);
    add_test(set, "testing image_init, initializing a new image", test_image_init);
    add_test(set, "testing image_alloc, allocating a new image", test_image_alloc);
    add_test(set, "testing setf_getf, setters and getters", test_setf_getf);
    add_test(set, "testing setc_getc, setters and getters", test_setc_getc);
    add_test(set, "testing setz_getz, setters and getters", test_setz_getz);
    add_test(set, "testing seta_geta, setters and getters", test_seta_geta);
    add_test(set, "testing test_utility, utility functions", test_utility);

    return set;
}

/**
 * Runs the tests for the linked list implementation.
 *
 * @param start_test the test to start running from
 * @param end_test the test to stop running at
 */
int main(const int argc, const char *argv[])
{
    TestingSet *set = init_testing_set(); // initialize the testing set

    // now proccess args and run the test program
    check_for_verbose(argc, argv); // check for verbose flag and set the global variable found in testing_util.h
    check_for_debug(argc, argv);   // check for debug flag and set the global variable found in testing_util.h

    int test_start_loc = 1;                                                                          // the index to start looking for tests at, but may change if -v is there
    int start_test = get_int_arg(argc, argv, 1, 0, &test_start_loc);                                 // get the start test, default to 0
    int end_test = get_int_arg(argc, argv, test_start_loc + 1, set->num_tests - 1, &test_start_loc); // get the end test, default to 0
    PRINT_VERBOSE("Running tests %d through %d\n", start_test, end_test);
    run_tests(set, start_test, end_test);

    free_testing_set(set);
    return 0;
}
