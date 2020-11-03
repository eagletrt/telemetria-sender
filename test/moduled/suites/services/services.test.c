#include "services.test.h"

condition_t condition;

MunitSuite* get_services_suite() {
    MunitSuite *sub_suites = (MunitSuite *)malloc(0 * sizeof(MunitSuite));
    //sub_suites[0] = *get_config_service_suite();
    //sub_suites[1] = *get_mongo_service_suite();


    MunitSuite* suite = (MunitSuite*) malloc(sizeof(MunitSuite));
    suite->prefix = "/services";
    suite->tests = NULL;
    suite->suites = sub_suites;
    suite->iterations = 1;
    suite->options = MUNIT_SUITE_OPTION_NONE;

    return suite;
}