#include "utils.test.h"

MunitSuite* get_utils_suite() {
    MunitSuite *sub_suites = (MunitSuite *)malloc(4 * sizeof(MunitSuite));
    sub_suites[0] = *get_misc_utils_suite();
    sub_suites[1] = *get_log_utils_suite();
    sub_suites[2] = *get_config_utils_suite();

    MunitSuite* suite = (MunitSuite*) malloc(sizeof(MunitSuite));
    suite->prefix = "/utils";
    suite->tests = NULL;
    suite->suites = sub_suites;
    suite->iterations = 1;
    suite->options = MUNIT_SUITE_OPTION_NONE;

    return suite;
}
