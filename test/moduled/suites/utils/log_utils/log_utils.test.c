#include "log_utils.test.h"

/* EXPORTED SUITE */

MunitSuite* get_log_utils_suite() {
  MunitSuite* sub_suites = (MunitSuite*) malloc(1 * sizeof(MunitSuite));
  sub_suites[0] = *get_log_utils_log_info_suite();

  MunitSuite* suite = (MunitSuite*) malloc(sizeof(MunitSuite));
  suite->prefix = "/log-utils";
  suite->tests = NULL;
  suite->suites = sub_suites;
  suite->iterations = 1;
  suite->options = MUNIT_SUITE_OPTION_NONE;

  return suite;
}