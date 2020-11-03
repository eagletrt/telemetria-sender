#include "libs/munit/munit.h"
#include "suites/utils/utils.test.h"
#include "suites/services/services.test.h"

int main(int argc, char *argv[]) {
    MunitSuite* sub_suites = (MunitSuite*) malloc(3 * sizeof(MunitSuite));
    sub_suites[0] = *get_utils_suite();
    sub_suites[1] = *get_services_suite();
    sub_suites[2] = (MunitSuite) { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL };

    MunitSuite* main_test_suite = (MunitSuite*) malloc(sizeof(MunitSuite));
    main_test_suite->prefix = "/moduled-tests";
    main_test_suite->tests = NULL;
    main_test_suite->suites = sub_suites;
    main_test_suite->iterations = 1;
    main_test_suite->options = MUNIT_SUITE_OPTION_NONE;

    return munit_suite_main(main_test_suite, NULL, argc, argv);
}