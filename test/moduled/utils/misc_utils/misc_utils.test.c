#include "misc_utils.test.h"

/* EXPORTED SUITE */

MunitSuite* get_misc_utils_suite() {
  MunitSuite* misc_utils_suite = (MunitSuite*) malloc(sizeof(MunitSuite));
  misc_utils_suite->prefix = "/misc-utils";
  misc_utils_suite->tests = NULL;
  misc_utils_suite->suites = get_misc_utils_clone_strings_array_suite();
  misc_utils_suite->iterations = 1;
  misc_utils_suite->options = MUNIT_SUITE_OPTION_NONE;

  return misc_utils_suite;
}