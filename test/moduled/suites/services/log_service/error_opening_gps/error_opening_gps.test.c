#include "error_opening_gps.test.h"

/* HELPER FUNCTIONS SIGNATURES */

static MunitResult test_error_opening_gps(const MunitParameter params[], void* fixture);

/* EXPORTED SUITE */

static MunitTest tests_error_opening_gps[] = {
  {
    "",
    test_error_opening_gps,
    NULL,
    NULL,
    MUNIT_TEST_OPTION_NONE,
    NULL
  },
  { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static MunitSuite log_service_error_opening_gps_suite = {
  "/error-opening-gps",
  tests_error_opening_gps,
  NULL,
  1,
  MUNIT_SUITE_OPTION_NONE
};

MunitSuite* get_log_service_error_opening_gps_suite() {
  return &log_service_error_opening_gps_suite;
}

/* HELPER FUNCTIONS DEFINITIONS */

static MunitResult test_error_opening_gps(const MunitParameter params[], void* fixture) {
  const char* expected = "\x1b[31m[ERROR]\x1b[0m\t\tError in opening GPS port\n";

  redirect_config_t redirect_config = init_redirect_config();
  redirect_stdout(&redirect_config);
  errorOpeningGPS();
  recover_stdout(&redirect_config);

  munit_assert_string_equal(redirect_config.buffer, expected);
  return MUNIT_OK;
}