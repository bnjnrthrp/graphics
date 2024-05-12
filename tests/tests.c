/**
 * This program runs unit tests for the composite image lab
 *
 * @author Benji Northrop
 * CS5310
 * Summer 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include "../include/ppmIO.h"
#include "../include/alphaMask.h"

#include "testing_util.h"

/******************** TESTS ******************/

/**
 * Test the newPixel() function.
 * @returns true if the test passed, false otherwise
 */
bool testNewImage()
{
    bool passed = true;
    int rows = 12;
    int cols = 12;
    Pixel *pixel = newImage1d(rows, cols);
    passed &= ASSERT_NOT_NULL(pixel);
    for (int i = 0; i < rows * cols; i++)
    {
        passed &= ASSERT_EQUAL(0, pixel[i].r);
        passed &= ASSERT_EQUAL(0, pixel[i].g);
        passed &= ASSERT_EQUAL(0, pixel[i].b);
    }
    free(pixel);
    return passed;
}

bool testResizeImage()
{
    bool passed = true;
    int origRows = 12;
    int origCols = 12;
    Pixel *pixel = newImage1d(origRows, origCols);

    free(pixel);
}

/*************************** end tests ***********/

/**
 * Adds every testing function to the testing set.
 * Includes the name of the test, the group tag, and the function to run.
 */
TestingSet *init_testing_set()
{
    TestingSet *set = new_testing_set();
    add_test(set, "testing testNewImage, creating a new image", testNewImage);

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
