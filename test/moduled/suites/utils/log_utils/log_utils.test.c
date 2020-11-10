#include "log_utils.test.h"

/* EXPORTED SUITE */

MunitSuite* get_log_utils_suite() {
  MunitSuite* sub_suites = (MunitSuite*) malloc(6 * sizeof(MunitSuite));
  sub_suites[0] = *get_log_utils_log_info_suite();
  sub_suites[1] = *get_log_utils_log_success_suite();
  sub_suites[2] = *get_log_utils_log_debug_suite();
  sub_suites[3] = *get_log_utils_log_error_suite();
  sub_suites[4] = *get_log_utils_log_warning_suite();
  sub_suites[5] = (MunitSuite) { NULL, NULL, NULL, 1, MUNIT_TEST_OPTION_NONE };

  MunitSuite* suite = (MunitSuite*) malloc(sizeof(MunitSuite));
  suite->prefix = "/log-utils";
  suite->tests = NULL;
  suite->suites = sub_suites;
  suite->iterations = 1;
  suite->options = MUNIT_SUITE_OPTION_NONE;

  return suite;
}