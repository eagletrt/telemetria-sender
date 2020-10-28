#include "munit/munit.h"
#include "utils/misc_utils/misc_utils.test.h"

int main(int argc, char *argv[]) {
    MunitSuite* sub_suites = (MunitSuite*) malloc(1 * sizeof(MunitSuite));
    sub_suites[0] = *get_misc_utils_suite();

    MunitSuite* main_test_suite = (MunitSuite*) malloc(sizeof(MunitSuite));
    main_test_suite->prefix = "/moduled-tests";
    main_test_suite->tests = NULL;
    main_test_suite->suites = sub_suites;
    main_test_suite->iterations = 1;
    main_test_suite->options = MUNIT_SUITE_OPTION_NONE;

    return munit_suite_main(main_test_suite, NULL, argc, argv);
}