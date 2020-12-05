#include "get_current_timestamp.test.h"

/* HELPER FUNCTIONS SIGNATURES */

static MunitResult test_get_current_timestamp_specific(const MunitParameter params[], void* fixture);

/* EXPORTED SUITE */

static MunitTest tests_get_current_timestamp[] = {
  {
    "",
    test_get_current_timestamp_specific,
    NULL,
    NULL,
    MUNIT_TEST_OPTION_NONE,
    NULL
  },
  { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static MunitSuite misc_utils_get_current_timestamp_suite = {
  "/get_current_timestamp",
  tests_get_current_timestamp,
  NULL,
  1,
  MUNIT_SUITE_OPTION_NONE
};

MunitSuite* get_misc_utils_get_current_timestamp_suite() {
  return &misc_utils_get_current_timestamp_suite;
}

/* HELPER FUNCTIONS DEFINITIONS */

static MunitResult test_get_current_timestamp_specific(const MunitParameter params[], void* fixture) {

    time_t seconds = time(NULL);
    long long int milliseconds = getCurrentTimestamp();


    long long int expected_low = (seconds * 1000) - 1000;
    long long int expected_high = (seconds * 1000) + 1000;

    munit_assert_llong(milliseconds, >=, expected_low);
    munit_assert_llong(milliseconds, <=, expected_high);

    return MUNIT_OK;
}