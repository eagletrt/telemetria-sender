#include "log_utils.h"

// ...



/* EXPORTED SUITE */

// to update
//static MunitTest tests_clone_strings_array[] = {

static MunitSuite log_utils_first_suite = {
  "/first_suite",
  tests_clone_strings_array,
  NULL,
  1,
  MUNIT_SUITE_OPTION_NONE
};

MunitSuite* get_log_utils_first_suite() {
  return &log_utils_first_suite;
}

//I chose to still use the sub-suites sytem in case of a future expansion,
//but I also collapsed the sub-suites under the main suite
MunitSuite* get_log_utils_suite() {
    //TODO
    MunitSuite* sub_suites = (MunitSuite*) malloc(1 * sizeof(MunitSuite));
    sub_suites[0] = *get_log_utils_first_suite();
    //No touchy touchy
    MunitSuite* suite = (MunitSuite*) malloc(sizeof(MunitSuite));
    suite->prefix = "/log-utils";
    suite->tests = NULL;
    suite->suites = sub_suites;
    suite->iterations = 1;
    suite->options = MUNIT_SUITE_OPTION_NONE;

    return suite;
}