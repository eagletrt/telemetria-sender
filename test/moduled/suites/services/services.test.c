#include "services.test.h"

/* MOCKED CONDITION FOR THE SUB SUITES */

condition_t condition;

/* EXPORTED SUITE */

MunitSuite* get_services_suite() {
    MunitSuite *sub_suites = (MunitSuite *)malloc(4 * sizeof(MunitSuite));
    sub_suites[0] = *get_log_service_suite();
    sub_suites[1] = *get_config_service_suite();
    sub_suites[2] = *get_mongo_service_suite();
    sub_suites[3] = (MunitSuite) { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL };

    MunitSuite* suite = (MunitSuite*) malloc(sizeof(MunitSuite));
    suite->prefix = "/services";
    suite->tests = NULL;
    suite->suites = sub_suites;
    suite->iterations = 1;
    suite->options = MUNIT_SUITE_OPTION_NONE;

    return suite;
}