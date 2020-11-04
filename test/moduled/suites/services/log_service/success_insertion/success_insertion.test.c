#include "success_insertion.test.h"

/* HELPER FUNCTIONS SIGNATURES */

typedef struct {
  int size;
  char* expected;
} test_success_insertion_fixture;

static char* test_success_insertion_params_size[];
static MunitParameterEnum test_success_insertion_params[];

static void* test_success_insertion_setup(const MunitParameter params[], void* user_data);
static MunitResult test_success_insertion(const MunitParameter params[], void* fixture);
static void test_success_insertion_tear_down(void* fixture);

/* EXPORTED SUITE */

static MunitTest tests_success_insertion[] = {
  {
    "",
    test_success_insertion,
    test_success_insertion_setup,
    test_success_insertion_tear_down,
    MUNIT_TEST_OPTION_NONE,
    test_success_insertion_params
  },
  { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static MunitSuite log_service_success_insertion_suite = {
  "/success-insertion",
  tests_success_insertion,
  NULL,
  1,
  MUNIT_SUITE_OPTION_NONE
};

MunitSuite* get_log_service_success_insertion_suite() {
  return &log_service_success_insertion_suite;
}

/* HELPER FUNCTIONS DEFINITIONS */

static char* test_success_insertion_params_size[] = {
  "0", "-100", "3000", NULL
};
static MunitParameterEnum test_success_insertion_params[] = {
  { "size", test_success_insertion_params_size },
  { NULL, NULL }
};

static void* test_success_insertion_setup(const MunitParameter params[], void* user_data) {
  test_success_insertion_fixture* fixture = (test_success_insertion_fixture*) malloc(sizeof(test_success_insertion_fixture));
  fixture->size = atoi(params[0].value);
  asprintf(&fixture->expected, "\x1b[32m[SUCCESS]\x1b[0m\tInserted document, size: %d B\n",fixture->size);
  return fixture;
}

static MunitResult test_success_insertion(const MunitParameter params[], void* fixture) {
  test_success_insertion_fixture* typed_fixture = (test_success_insertion_fixture*) fixture;
  int size = typed_fixture->size;
  char* expected = typed_fixture->expected;

  redirect_config_t redirect_config = init_redirect_config();
  redirect_stdout(&redirect_config);
  successInsertion(size);
  recover_stdout(&redirect_config);

  munit_assert_string_equal(redirect_config.buffer, expected);
  return MUNIT_OK;
}

static void test_success_insertion_tear_down(void* fixture) {
  test_success_insertion_fixture* typed_fixture = (test_success_insertion_fixture*) fixture;
  free(typed_fixture->expected);
  free(typed_fixture);
}