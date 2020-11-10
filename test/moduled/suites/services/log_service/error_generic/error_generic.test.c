#include "error_generic.test.h"

/* HELPER FUNCTIONS SIGNATURES */

typedef struct {
  char* message;
  char* expected;
} test_error_generic_fixture;

static char* test_error_generic_params_message[];
static MunitParameterEnum test_error_generic_params[];

static void* test_error_generic_setup(const MunitParameter params[], void* user_data);
static MunitResult test_error_generic(const MunitParameter params[], void* fixture);
static void test_error_generic_tear_down(void* fixture);

/* EXPORTED SUITE */

static MunitTest tests_error_generic[] = {
  {
    "",
    test_error_generic,
    test_error_generic_setup,
    test_error_generic_tear_down,
    MUNIT_TEST_OPTION_NONE,
    test_error_generic_params
  },
  { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static MunitSuite log_service_error_generic_suite = {
  "/error-generic",
  tests_error_generic,
  NULL,
  1,
  MUNIT_SUITE_OPTION_NONE
};

MunitSuite* get_log_service_error_generic_suite() {
  return &log_service_error_generic_suite;
}

/* HELPER FUNCTIONS DEFINITIONS */

static char* test_error_generic_params_message[] = {
  "", "Valdagno", "Ich will", "#£$&/(", NULL
};
static MunitParameterEnum test_error_generic_params[] = {
  { "message", test_error_generic_params_message },
  { NULL, NULL },
};

static void* test_error_generic_setup(const MunitParameter params[], void* user_data) {
  test_error_generic_fixture* fixture = (test_error_generic_fixture*) malloc(sizeof(test_error_generic_fixture));
  fixture->message = strdup(params[0].value);
  asprintf(&fixture->expected, "\x1b[31m[ERROR]\x1b[0m\t\t%s\n", fixture->message);
  return fixture;
}

static MunitResult test_error_generic(const MunitParameter params[], void* fixture) {
    test_error_generic_fixture* typed_fixture = (test_error_generic_fixture*) fixture;
    char* message = typed_fixture->message;
    char* expected = typed_fixture->expected;

    redirect_config_t* redirect_config = init_redirect_config();
    redirect_stdout(redirect_config);
    errorGeneric(message);
    recover_stdout(redirect_config);

    munit_assert_string_equal(redirect_config->buffer, expected);

    return MUNIT_OK;
}

static void test_error_generic_tear_down(void* fixture) {
  test_error_generic_fixture* typed_fixture = (test_error_generic_fixture*) fixture;
  free(typed_fixture->expected);
  free(typed_fixture);
}