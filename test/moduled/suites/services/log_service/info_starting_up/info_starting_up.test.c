#include "info_starting_up.test.h"

/* HELPER FUNCTIONS SIGNATURES */

static MunitResult test_info_starting_up(const MunitParameter params[], void* fixture);

/* EXPORTED SUITE */

static MunitTest tests_info_starting_up[] = {
  {
    "",
    test_info_starting_up,
    NULL,
    NULL,
    MUNIT_TEST_OPTION_NONE,
    NULL
  },
  { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static MunitSuite log_service_info_starting_up_suite = {
  "/info-starting-up",
  tests_info_starting_up,
  NULL,
  1,
  MUNIT_SUITE_OPTION_NONE
};

MunitSuite* get_log_service_info_starting_up_suite() {
  return &log_service_info_starting_up_suite;
}

/* HELPER FUNCTIONS DEFINITIONS */

static MunitResult test_info_starting_up(const MunitParameter params[], void* fixture) {
  const char* expected = "\x1b[34m[INFO]\x1b[0m\t\tStarting telemetry up\n";

  redirect_config_t* redirect_config = init_redirect_config();
  redirect_stdout(redirect_config);
  infoStartingUp();
  recover_stdout(redirect_config);

  munit_assert_string_equal(redirect_config->buffer, expected);
  return MUNIT_OK;
}