#include "error_parsing_config.test.h"

/* HELPER FUNCTIONS SIGNATURES */

static char* test_error_parsing_config_params_index[];
static MunitParameterEnum test_error_parsing_config_params[];

static MunitResult test_error_parsing_config(const MunitParameter params[], void* fixture);

/* EXPORTED SUITE */

static MunitTest tests_error_parsing_config[] = {
  {
    "",
    test_error_parsing_config,
    NULL,
    NULL,
    MUNIT_TEST_OPTION_NONE,
    test_error_parsing_config_params
  },
  { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static MunitSuite log_service_error_parsing_config_suite = {
  "/error-parsing-config",
  tests_error_parsing_config,
  NULL,
  1,
  MUNIT_SUITE_OPTION_NONE
};

MunitSuite* get_log_service_error_parsing_config_suite() {
  return &log_service_error_parsing_config_suite;
}

/* HELPER FUNCTIONS DEFINITIONS */

static char* test_error_parsing_config_params_index[] = {
  "0", "1", "2", NULL
};
static MunitParameterEnum test_error_parsing_config_params[] = {
  { "index", test_error_parsing_config_params_index },
  { NULL, NULL },
};

static MunitResult test_error_parsing_config(const MunitParameter params[], void* fixture) {
    int index = atoi(params[0].value);
    condition = log_service_error_parsing_config_assets_conditions[index];
    const char* expected = log_service_error_parsing_config_assets_results[index];
    redirect_config_t* redirect_config = init_redirect_config();
    redirect_stdout(redirect_config);
    errorParsingConfig();
    recover_stdout(redirect_config);
    munit_assert_string_equal(redirect_config->buffer, expected);
    return MUNIT_OK;
}