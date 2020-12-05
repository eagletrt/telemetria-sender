#include "misc_utils.test.h"

/* EXPORTED SUITE */

MunitSuite* get_misc_utils_suite() {
  MunitSuite* sub_suites = (MunitSuite*) malloc(8 * sizeof(MunitSuite));
  sub_suites[0] = *get_misc_utils_clone_strings_array_suite();
  sub_suites[1] = *get_misc_utils_free_strings_array_suite();
  sub_suites[2] = *get_misc_utils_print_strings_array_suite();
  sub_suites[3] = *get_misc_utils_print_int_array_suite();
  sub_suites[4] = *get_misc_utils_print_double_array_suite();
  sub_suites[5] = *get_misc_utils_digit_counts_suite();
  sub_suites[6] = *get_misc_utils_get_current_timestamp_suite();
  sub_suites[7] = (MunitSuite) { NULL, NULL, NULL, 1, MUNIT_TEST_OPTION_NONE };

  MunitSuite* suite = (MunitSuite*) malloc(sizeof(MunitSuite));
  suite->prefix = "/misc-utils";
  suite->tests = NULL;
  suite->suites = sub_suites;
  suite->iterations = 1;
  suite->options = MUNIT_SUITE_OPTION_NONE;

  return suite;
}