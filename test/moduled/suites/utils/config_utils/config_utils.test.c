#include "config_utils.test.h"

MunitSuite *get_config_utils_suite()
{
    MunitSuite *sub_suites = (MunitSuite *) malloc(5 * sizeof(MunitSuite));
    sub_suites[0] = *get_config_utils_new_config_suite();
    sub_suites[1] = *get_config_utils_delete_config_suite();
    sub_suites[2] = *get_config_utils_parse_config_file_suite();
    sub_suites[3] = *get_config_utils_print_config_suite();
    sub_suites[4] = (MunitSuite) { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL };

    MunitSuite *suite = (MunitSuite *)malloc(sizeof(MunitSuite));
    suite->prefix = "/config_utils";
    suite->tests = NULL;
    suite->suites = sub_suites;
    suite->iterations = 1;
    suite->options = MUNIT_SUITE_OPTION_NONE;

    return suite;
}