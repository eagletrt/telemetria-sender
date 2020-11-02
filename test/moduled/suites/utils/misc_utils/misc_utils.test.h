#ifndef TELEMETRY_TEST_MISC_UTILS
#define TELEMETRY_TEST_MISC_UTILS

/** IMPORTS **/

#include "../../../libs/munit/munit.h"
#include "./clone_strings_array/clone_strings_array.test.h"
#include "./free_strings_array/free_strings_array.test.h"
#include "./print_strings_array/print_strings_array.test.h"
#include "./print_int_array/print_int_array.test.h"
#include "./print_double_array/print_double_array.test.h"

/* EXPORTED SUITE */

MunitSuite* get_misc_utils_suite();

#endif