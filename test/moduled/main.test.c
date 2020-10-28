#include "munit/munit.h"
#include "utils/misc_utils/misc_utils.test.h"

int main(int argc, char *argv[]) {
    return munit_suite_main(misc_utils_suite, NULL, argc, argv);
}