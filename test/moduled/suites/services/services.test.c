#include "services.test.h"

condition_t condition;

MunitSuite* get_services_suite() {
    MunitSuite *sub_suites = (MunitSuite *)malloc(3 * sizeof(MunitSuite));
    sub_suites[0] = *get_config_service_suite();
    sub_suites[1] = *get_mongo_service_suite();
    sub_suites[2] = (MunitSuite) { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL };


    MunitSuite* suite = (MunitSuite*) malloc(sizeof(MunitSuite));
    suite->prefix = "/services";
    suite->tests = NULL;
    suite->suites = sub_suites;
    suite->iterations = 1;
    suite->options = MUNIT_SUITE_OPTION_NONE;

    return suite;
}