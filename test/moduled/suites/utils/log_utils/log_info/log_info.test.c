#include "log_info.test.h"

/* HELPER FUNCTIONS SIGNATURES */

typedef struct {
    char* str;
    char* expected;
} test_log_info_fixture;

static char* test_log_info_params_str[];
static MunitParameterEnum test_log_info_params[];

static void* test_log_info_setup(const MunitParameter params[], void* user_data);
static MunitResult test_log_info(const MunitParameter params[], void* fixture);
static void test_log_info_tear_down(void* fixture);

/* EXPORTED SUITE */

static MunitTest tests_log_info[] = {
  {
    "",
    test_log_info,
    test_log_info_setup,
    NULL,
    MUNIT_TEST_OPTION_NONE,
    test_log_info_params
  },
  { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static MunitSuite log_utils_log_info_suite = {
  "/log-info",
  tests_log_info,
  NULL,
  1,
  MUNIT_SUITE_OPTION_NONE
};

MunitSuite* get_log_utils_log_info_suite() {
  return &log_utils_log_info_suite;
}

/* HELPER FUNCTIONS DEFINITIONS */

static char* test_log_info_params_n[] = {
  "Valdagno", "generic_string", "__#&aaaa", "", NULL
};
static MunitParameterEnum test_log_info_params[] = {
  { "str", test_log_info_params_n },
  { NULL, NULL },
};
static void* test_log_info_setup(const MunitParameter params[], void* user_data) {
    test_log_info_fixture* fixture = (test_log_info_fixture*) malloc(sizeof(test_log_info_fixture));

    const int MAX_SIZE = 1000;
    fixture->str = strdup(params[0].value);
    fixture->expected = (char*) malloc(MAX_SIZE * sizeof(char));
    sprintf(fixture->expected, "\x1b[34m[INFO]\x1b[0m\t\t%s\n", fixture->str);

    return (void*) fixture;
}
static MunitResult test_log_info(const MunitParameter params[], void* fixture) {
    test_log_info_fixture* typed_fixture = (test_log_info_fixture*) fixture;
    char* str = typed_fixture->str;
    char* expected = typed_fixture->expected;

    redirect_config_t redirect_config = init_redirect_config();
    redirect_stdout(&redirect_config);
    logInfo(str);
    recover_stdout(&redirect_config);

    munit_assert_string_equal(redirect_config.buffer, expected);

    return MUNIT_OK;
}
static void test_log_info_tear_down(void* fixture) {
    test_log_info_fixture* typed_fixture = (test_log_info_fixture*) fixture;
    free(typed_fixture->str);
    free(typed_fixture->expected);
}