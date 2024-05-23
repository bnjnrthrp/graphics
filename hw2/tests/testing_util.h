/**
 * Contains some utility functions and DEFINEs to make testing easier in C.
 *
 * Written for CS 5008 @ Northeastern University
 *
 * @author Albert Lionelle
 */
#ifndef TESTING_UTIL_H
#define TESTING_UTIL_H

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>

#define ASSERT_TRUE(T) (T)
#define ASSERT_FALSE(T) (!(T))
#define ASSERT_EQUAL(T, E) ((T) == (E))
#define ASSERT_NOT_EQUAL(T, E) ((T) != (E))
#define ASSERT_NULL(T) ((T) == NULL)
#define ASSERT_NOT_NULL(T) ((T) != NULL)
#define ASSERT_STR_EQUAL(T, E) (strcmp((T), (E)) == 0)
#define ASSERT_STR_NOT_EQUAL(T, E) (strcmp((T), (E)) != 0)
#define ASSERT_STR_EQUAL_IGNORE_CASE(T, E) (strcasecmp((T), (E)) == 0)
#define PRINT_DEBUG(...) \
    if (DEBUG)           \
        printf("(DEBUG): " __VA_ARGS__); // This is a macro that will print if debug is true.
#define PRINT_VERBOSE(...) \
    if (VERBOSE)           \
        printf(__VA_ARGS__); // This is a macro that will print if verbose is true.

// just setting the variable names, so the rest of the code doesn't need to know them.
#define VERBOSE __verbose__testing_util__
#define DEBUG __debug__testing_util__

bool VERBOSE = false; // This is a global variable that can be used to print extra information in tests.
bool DEBUG = false;   // This is a global variable that can be used to print extra information in tests.

/** This helps keep test functions paired with names
 * Has room to add other metadata for each test, such
 * as test weight, any tags/grouping, etc (future enhancement idea)
 * **/
typedef struct test
{
    char *name;
    bool (*func)();
    struct test *next;
} Test;

/** Testing Set
 * Keep a linked list of tests, and the number of tests in the set
 **/
typedef struct testing_set
{
    Test *head;
    Test *tail;
    unsigned num_tests;
} TestingSet;

/** Creates a new testing set
 * @return a pointer to the new testing set
 * **/
TestingSet *new_testing_set()
{
    TestingSet *set = (TestingSet *)malloc(sizeof(TestingSet));
    set->head = NULL;
    set->tail = NULL;
    set->num_tests = 0;
    return set;
}

/** Frees the testing set
 * @param test_set the testing set to free
 * **/
void free_testing_set(TestingSet *test_set)
{
    Test *test = test_set->head;
    while (test != NULL)
    {
        Test *next = test->next;
        free(test->name);
        free(test);
        test = next;
    }
    free(test_set);
}

/**
 * Adds a test to the test set
 * @param test_set the test set to add the test to
 * @param name the name of the test
 * @param group_tag the group tag for the test
 * @param func the function to run for the test
 */
void add_test(TestingSet *test_set, char *name, bool (*func)())
{
    Test *test = (Test *)malloc(sizeof(Test));

    char *updated_name = (char *)malloc(sizeof(char) * (strlen(name) + 100));
    sprintf(updated_name, "%u. %s", test_set->num_tests, name);

    test->name = updated_name;
    test->func = func;
    test->next = NULL;
    if (test_set->head == NULL)
    {
        test_set->head = test;
        test_set->tail = test;
    }
    else
    {
        test_set->tail->next = test;
        test_set->tail = test;
    }
    test_set->num_tests++;
}

/**
 * Runs a test function and prints the result.
 * @param test a Test struct that pairs test meta info (name) with the test function.
 * @return True if the test passed, false otherwise.
 */
bool run_test(Test *test)
{
    bool result = test->func();
    if (result)
    {
        PRINT_VERBOSE("%s: PASSED\n", test->name);
    }
    else
    {
        printf("%s: FAILED\n", test->name); // always print if it fails.
    }
    return result;
}

/**
 * Runs the tests based on the pass in test array, and the index of the test to run.
 *
 * @param test_set The test set to run
 * @param start_index The index of the test to start running at.
 * @param end_index The index of the test to end running at.
 * @returns the number of tests passed.
 */
int run_tests(TestingSet *test_set, int start_index, int end_index)
{
    if (start_index < 0 || end_index >= test_set->num_tests)
    {
        fprintf(stderr, "Invalid start or end index\n");
        return 0;
    }
    int num_passed = 0;
    int test_num = 0;
    Test *test = test_set->head;
    while (test != NULL)
    { // loop through all the tests, until start index
        if (test_num >= start_index && test_num <= end_index)
        {
            if (run_test(test))
            {
                num_passed++;
            }
        }
        test_num++;
        if (test_num > end_index)
        { // early exit
            break;
        }
        test = test->next;
    }
    return num_passed;
}

/**
 * Runs the tests for a certain group_tag
 * @param test_set the test set to run
 * @param group_tag the group tag to run
 * @return the number of tests passed
 */
int run_tests_group(TestingSet *test_set, char *group_tag)
{
    int num_passed = 0;
    Test *test = test_set->head;
    while (test != NULL)
    { // loop through all the tests, until start index

        if (run_test(test))
        {
            num_passed++;
        }

        test = test->next;
    }
    return num_passed;
}

/*
 * Checks to see if a string is an int value for parsing arguments
 * C does not have an easy built in way to do this, this is one option.
 * @param str the string to check
 * @return true if the string is an int, false otherwise
 */
int is_int(const char *str)
{
    while (*str)
    {
        if (!isdigit(*str))
        {
            return false;
        }
        str++;
    }
    return true;
}

/**
 * Helper function to see if -v is in the argument array
 */
bool check_for_verbose(const int argc, const char *argv[])
{
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0)
        {
            VERBOSE = true;
        }
    }
    return VERBOSE; // technically not needed, but allows for options in main
}

/**
 * Helper function to see if -d is in the argument array
 */
bool check_for_debug(const int argc, const char *argv[])
{
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--debug") == 0)
        {
            DEBUG = true;
        }
    }
    return DEBUG;
}

/**
 * Helper function to see if -g is in the argument array
 * @return the group tag if found, NULL otherwise
 */
const char *check_for_group_tag(const int argc, const char *argv[])
{
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-g") == 0 || strcmp(argv[i], "--group") == 0)
        {
            if (i + 1 < argc)
                return argv[i + 1];
        }
    }
    return NULL;
}

/**
 * gets the int value in argv or default value
 * if none is found.
 *
 * @param argc the number of arguments
 * @param argv the arguments
 * @param start the index to start looking at
 * @param default_value the default value to return if none is found
 * @param location reference to the location it was found
 */
int get_int_arg(const int argc, const char *argv[], int start, int default_value, int *location)
{
    for (int i = start; i < argc; i++)
    {
        if (is_int(argv[i]))
        {
            *location = i;
            return atoi(argv[i]);
        }
    }
    return default_value;
}

#endif // TESTING_UTIL_H
