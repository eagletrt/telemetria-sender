#include "info_gps_plugged.test.h"

/* HELPER FUNCTIONS SIGNATURES */

static MunitResult test_info_gps_plugged(const MunitParameter params[], void* fixture);

/* EXPORTED SUITE */

static MunitTest tests_info_gps_plugged[] = {
  {
    "",
    test_info_gps_plugged,
    NULL,
    NULL,
    MUNIT_TEST_OPTION_NONE,
    NULL
  },
  { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static MunitSuite log_service_info_gps_plugged_suite = {
  "/info-gps-plugged",
  tests_info_gps_plugged,
  NULL,
  1,
  MUNIT_SUITE_OPTION_NONE
};

MunitSuite* get_log_service_info_gps_plugged_suite() {
  return &log_service_info_gps_plugged_suite;
}

/* HELPER FUNCTIONS DEFINITIONS */

static MunitResult test_info_gps_plugged(const MunitParameter params[], void* fixture) {
  const char* expected = "\x1b[34m[INFO]\x1b[0m\t\tGps is plugged to telemetry\n";

  redirect_config_t* redirect_config = init_redirect_config();
  redirect_stdout(redirect_config);
  infoGpsPlugged();
  recover_stdout(redirect_config);

  munit_assert_string_equal(redirect_config->buffer, expected);
  return MUNIT_OK;
}