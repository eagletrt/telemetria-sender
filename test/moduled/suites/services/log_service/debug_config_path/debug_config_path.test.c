#include "debug_config_path.test.h"

/* HELPER FUNCTIONS SIGNATURES */

static char* test_debug_config_path_params_index[];
static MunitParameterEnum test_debug_config_path_params[];

static MunitResult test_debug_config_path(const MunitParameter params[], void* fixture);

/* EXPORTED SUITE */

static MunitTest tests_debug_config_path[] = {
  {
    "",
    test_debug_config_path,
    NULL,
    NULL,
    MUNIT_TEST_OPTION_NONE,
    test_debug_config_path_params
  },
  { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static MunitSuite log_service_debug_config_path_suite = {
  "/debug-config-path",
  tests_debug_config_path,
  NULL,
  1,
  MUNIT_SUITE_OPTION_NONE
};

MunitSuite* get_log_service_debug_config_path_suite() {
  return &log_service_debug_config_path_suite;
}

/* HELPER FUNCTIONS DEFINITIONS */

static char* test_debug_config_path_params_index[] = {
  "0", "1", "2", NULL
};
static MunitParameterEnum test_debug_config_path_params[] = {
  { "index", test_debug_config_path_params_index },
  { NULL, NULL },
};

static MunitResult test_debug_config_path(const MunitParameter params[], void* fixture) {
    int index = atoi(params[0].value);
    condition = log_service_debug_config_path_assets_conditions[index];
    const char* expected = log_service_debug_config_path_assets_results[index];
    redirect_config_t redirect_config = init_redirect_config();
    redirect_stdout(&redirect_config);
    debugConfigPath();
    recover_stdout(&redirect_config);
    munit_assert_string_equal(redirect_config.buffer, expected);
    return MUNIT_OK;
}