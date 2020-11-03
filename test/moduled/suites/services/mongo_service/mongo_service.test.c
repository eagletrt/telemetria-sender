#include "mongo_service.test.h"

MunitSuite *get_mongo_service_suite()
{
    MunitSuite *sub_suites = (MunitSuite *)malloc(1 * sizeof(MunitSuite));
    sub_suites[0] = *get_mongo_services_mongo_setup_suite();


    MunitSuite *suite = (MunitSuite *)malloc(sizeof(MunitSuite));
    suite->prefix = "/mongo_service";
    suite->tests = NULL;
    suite->suites = sub_suites;
    suite->iterations = 1;
    suite->options = MUNIT_SUITE_OPTION_NONE;

    return suite;
}