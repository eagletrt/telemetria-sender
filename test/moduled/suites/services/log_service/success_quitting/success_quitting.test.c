#include "success_quitting.test.h"

/* HELPER FUNCTIONS SIGNATURES */

static MunitResult test_success_quitting(const MunitParameter params[], void* fixture);

/* EXPORTED SUITE */

static MunitTest tests_success_quitting[] = {
  {
    "",
    test_success_quitting,
    NULL,
    NULL,
    MUNIT_TEST_OPTION_NONE,
    NULL
  },
  { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static MunitSuite log_service_success_quitting_suite = {
  "/success-quitting",
  tests_success_quitting,
  NULL,
  1,
  MUNIT_SUITE_OPTION_NONE
};

MunitSuite* get_log_service_success_quitting_suite() {
  return &log_service_success_quitting_suite;
}

/* HELPER FUNCTIONS DEFINITIONS */

static MunitResult test_success_quitting(const MunitParameter params[], void* fixture) {
  const char* expected = "\x1b[32m[SUCCESS]\x1b[0m\tQuitted telemetry\n";

  redirect_config_t* redirect_config = init_redirect_config();
  redirect_stdout(redirect_config);
  successQuitting();
  recover_stdout(redirect_config);

  munit_assert_string_equal(redirect_config->buffer, expected);
  return MUNIT_OK;
}