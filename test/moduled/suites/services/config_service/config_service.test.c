#include "config_service.test.h"

MunitSuite *get_config_service_suite()
{
    MunitSuite *sub_suites = (MunitSuite *)malloc(2 * sizeof(MunitSuite));
    sub_suites[0] = *get_config_services_handle_config_suite();
    sub_suites[1] = (MunitSuite) { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL };


    MunitSuite *suite = (MunitSuite *)malloc(sizeof(MunitSuite));
    suite->prefix = "/config_service";
    suite->tests = NULL;
    suite->suites = sub_suites;
    suite->iterations = 1;
    suite->options = MUNIT_SUITE_OPTION_NONE;

    return suite;
}