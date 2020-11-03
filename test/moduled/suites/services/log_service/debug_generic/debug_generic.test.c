#include "debug_generic.test.h"

/* HELPER FUNCTIONS SIGNATURES */

typedef struct {
  int verbose;
  char* message;
  char* expected;
} test_debug_generic_fixture;

static char* test_debug_generic_params_message[];
static char* test_debug_generic_params_verbose[];
static MunitParameterEnum test_debug_generic_params[];

static void* test_debug_generic_setup(const MunitParameter params[], void* user_data);
static MunitResult test_debug_generic(const MunitParameter params[], void* fixture);
static void test_debug_generic_tear_down(void* fixture);

/* EXPORTED SUITE */

static MunitTest tests_debug_generic[] = {
  {
    "",
    test_debug_generic,
    test_debug_generic_setup,
    test_debug_generic_tear_down,
    MUNIT_TEST_OPTION_NONE,
    test_debug_generic_params
  },
  { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static MunitSuite log_service_debug_generic_suite = {
  "/debug-generic",
  tests_debug_generic,
  NULL,
  1,
  MUNIT_SUITE_OPTION_NONE
};

MunitSuite* get_log_service_debug_generic_suite() {
  return &log_service_debug_generic_suite;
}

/* HELPER FUNCTIONS DEFINITIONS */

static char* test_debug_generic_params_message[] = {
  "", "Valdagno", "Ich will", "#£$&/(", NULL
};
static char* test_debug_generic_params_verbose[] = {
  "0", "1", NULL
};
static MunitParameterEnum test_debug_generic_params[] = {
  { "message", test_debug_generic_params_message },
  { "verbose", test_debug_generic_params_verbose },
  { NULL, NULL },
};

static void* test_debug_generic_setup(const MunitParameter params[], void* user_data) {
  test_debug_generic_fixture* fixture = (test_debug_generic_fixture*) malloc(sizeof(test_debug_generic_fixture));
  fixture->message = strdup(params[0].value);
  fixture->verbose = atoi(params[1].value);
  
  if (fixture->verbose == 1) {
    asprintf(&fixture->expected, "\e[1;30m[DEBUG]\x1b[0m\t\t%s\n", fixture->message);
  }
  else {
    fixture->expected = strdup("");
  }

  return fixture;
}

static MunitResult test_debug_generic(const MunitParameter params[], void* fixture) {
    test_debug_generic_fixture* typed_fixture = (test_debug_generic_fixture*) fixture;
    int verbose = typed_fixture->verbose;
    char* message = typed_fixture->message;
    char* expected = typed_fixture->expected;

    condition.verbose = verbose;

    redirect_config_t redirect_config = init_redirect_config();
    redirect_stdout(&redirect_config);
    debugGeneric(message);
    recover_stdout(&redirect_config);

    munit_assert_string_equal(redirect_config.buffer, expected);

    return MUNIT_OK;
}

static void test_debug_generic_tear_down(void* fixture) {
  test_debug_generic_fixture* typed_fixture = (test_debug_generic_fixture*) fixture;
  free(typed_fixture->message);
  free(typed_fixture->expected);
  free(typed_fixture);
}