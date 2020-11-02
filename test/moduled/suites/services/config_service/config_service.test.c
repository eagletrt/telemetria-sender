#include "config_service.test.h"

MunitSuite *get_config_service_suite()
{
    MunitSuite *sub_suites = (MunitSuite *)malloc(0 * sizeof(MunitSuite));

    MunitSuite *suite = (MunitSuite *)malloc(sizeof(MunitSuite));
    suite->prefix = "/config_service";
    suite->tests = NULL;
    suite->suites = sub_suites;
    suite->iterations = 1;
    suite->options = MUNIT_SUITE_OPTION_NONE;

    return suite;
}