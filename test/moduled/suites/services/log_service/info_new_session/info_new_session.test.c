#include "info_new_session.test.h"

/* HELPER FUNCTIONS SIGNATURES */

static char* test_info_new_session_params_index[];
static MunitParameterEnum test_info_new_session_params[];

static MunitResult test_info_new_session(const MunitParameter params[], void* fixture);

/* EXPORTED SUITE */

static MunitTest tests_info_new_session[] = {
  {
    "",
    test_info_new_session,
    NULL,
    NULL,
    MUNIT_TEST_OPTION_NONE,
    test_info_new_session_params
  },
  { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static MunitSuite log_service_info_new_session_suite = {
  "/info-new-session",
  tests_info_new_session,
  NULL,
  1,
  MUNIT_SUITE_OPTION_NONE
};

MunitSuite* get_log_service_info_new_session_suite() {
  return &log_service_info_new_session_suite;
}

/* HELPER FUNCTIONS DEFINITIONS */

static char* test_info_new_session_params_index[] = {
  "0", "1", "2", NULL
};
static MunitParameterEnum test_info_new_session_params[] = {
  { "index", test_info_new_session_params_index },
  { NULL, NULL },
};

static MunitResult test_info_new_session(const MunitParameter params[], void* fixture) {
    int index = atoi(params[0].value);
    condition = log_service_info_new_session_assets_conditions[index];
    const char* expected = log_service_info_new_session_assets_results[index];
    redirect_config_t* redirect_config = init_redirect_config();
    redirect_stdout(redirect_config);
    infoNewSession();
    recover_stdout(redirect_config);
    munit_assert_string_equal(redirect_config->buffer, expected);
    return MUNIT_OK;
}