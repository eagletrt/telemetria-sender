#include "info_transition.test.h"

/* HELPER FUNCTIONS SIGNATURES */

typedef struct {
  int from;
  int to;
  char* expected;
} test_info_transition_fixture;

static char* test_info_transition_labels[];
static char* test_info_transition_params_from[];
static char* test_info_transition_params_to[];
static MunitParameterEnum test_info_transition_params[];

static void* test_info_transition_setup(const MunitParameter params[], void* user_data);
static MunitResult test_info_transition(const MunitParameter params[], void* fixture);
static void test_info_transition_tear_down(void* fixture);

/* EXPORTED SUITE */

static MunitTest tests_info_transition[] = {
  {
    "",
    test_info_transition,
    test_info_transition_setup,
    test_info_transition_tear_down,
    MUNIT_TEST_OPTION_NONE,
    test_info_transition_params
  },
  { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static MunitSuite log_service_info_transition_suite = {
  "/info-transition",
  tests_info_transition,
  NULL,
  1,
  MUNIT_SUITE_OPTION_NONE
};

MunitSuite* get_log_service_info_transition_suite() {
  return &log_service_info_transition_suite;
}

/* HELPER FUNCTIONS DEFINITIONS */

static char* test_info_transition_labels[] = {
  "first",
  "second",
  "third",
  "fourth",
  "fifth",
  "sixth"
};
static char* test_info_transition_params_from[] = {
  "0", "1", "2", "3", "4", "5", NULL
};
static char* test_info_transition_params_to[] = {
  "0", "1", "2", "3", "4", "5", NULL
};
static MunitParameterEnum test_info_transition_params[] = {
  { "from", test_info_transition_params_from },
  { "to", test_info_transition_params_to },
  { NULL, NULL }
};

static void* test_info_transition_setup(const MunitParameter params[], void* user_data) {
  test_info_transition_fixture* fixture = (test_info_transition_fixture*) malloc(sizeof(test_info_transition_fixture));
  fixture->from = atoi(params[0].value);
  fixture->to = atoi(params[1].value);
  const char* from_label = test_info_transition_labels[fixture->from];
  const char* to_label = test_info_transition_labels[fixture->to];

  if (fixture->from != fixture->to) {
    asprintf(&fixture->expected, "\x1b[34m[INFO]\x1b[0m\t\tTransition from %s to %s\n", from_label, to_label);
  }
  else {
    fixture->expected = strdup("");
  }

  return fixture;
}

static MunitResult test_info_transition(const MunitParameter params[], void* fixture) {
  test_info_transition_fixture* typed_fixture = (test_info_transition_fixture*) fixture;
  int from = typed_fixture->from;
  int to = typed_fixture->to;
  char* expected = typed_fixture->expected;

  redirect_config_t redirect_config = init_redirect_config();
  redirect_stdout(&redirect_config);
  infoTransition(from, to, test_info_transition_labels);
  recover_stdout(&redirect_config);

  munit_assert_string_equal(redirect_config.buffer, expected);
  return MUNIT_OK;
}

static void test_info_transition_tear_down(void* fixture) {
  test_info_transition_fixture* typed_fixture = (test_info_transition_fixture*) fixture;
  free(typed_fixture->expected);
  free(typed_fixture);
}