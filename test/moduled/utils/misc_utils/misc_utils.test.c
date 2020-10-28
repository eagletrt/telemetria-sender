#include "misc_utils.test.h"

/* EXPORTED SUITE */

static MunitSuite suite = {
  "/misc-utils",
  tests_clone_strings_array,
  NULL,
  1,
  MUNIT_SUITE_OPTION_NONE
};
MunitSuite *misc_utils_suite = &suite;