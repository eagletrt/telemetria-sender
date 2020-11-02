#include "digit_counts.test.h"

/* HELPER FUNCTIONS SIGNATURES */

typedef struct {
    int n;
    int expected;
} test_digit_counts_fixture;

// specific numbers

static char* test_digit_counts_params_n_of_digits_specific[];
static MunitParameterEnum test_digit_counts_params_specific[];

static void* test_digit_counts_setup_specific(const MunitParameter params[], void* user_data);
static MunitResult test_digit_counts_specific(const MunitParameter params[], void* fixture);

// generated random numbers

static char* test_digit_counts_params_n_of_digits_random[];
static MunitParameterEnum test_digit_counts_params_random[];

static void* test_digit_counts_setup_random(const MunitParameter params[], void* user_data);
static MunitResult test_digit_counts_random(const MunitParameter params[], void* fixture);

/* EXPORTED SUITE */

static MunitTest tests_digit_counts[] = {
  {
    "/specific-numbers",
    test_digit_counts_specific,
    test_digit_counts_setup_specific,
    NULL,
    MUNIT_TEST_OPTION_NONE,
    test_digit_counts_params_specific
  },
  {
    "/generated-random-numbers",
    test_digit_counts_random,
    test_digit_counts_setup_random,
    NULL,
    MUNIT_TEST_OPTION_NONE,
    test_digit_counts_params_random
  },
  { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static MunitSuite misc_utils_digit_counts_suite = {
  "/digit_counts",
  tests_digit_counts,
  NULL,
  1,
  MUNIT_SUITE_OPTION_NONE
};

MunitSuite* get_misc_utils_digit_counts_suite() {
  return &misc_utils_digit_counts_suite;
}

/* HELPER FUNCTIONS DEFINITIONS */

// specific numbers

static char* test_digit_counts_params_n_of_digits_specific[] = {
  "0", "-23", "-455", "232323", NULL
};
static MunitParameterEnum test_digit_counts_params_specific[] = {
  { "n", test_digit_counts_params_n_of_digits_specific },
  { NULL, NULL },
};
static void* test_digit_counts_setup_specific(const MunitParameter params[], void* user_data) {
    test_digit_counts_fixture* fixture = (test_digit_counts_fixture*) malloc(sizeof(test_digit_counts_fixture));
    fixture->n = atoi(params[0].value);

    switch(fixture->n) {
      case 0:
       fixture->expected = 1;
       break;
      case -23:
       fixture->expected = 2;
       break;
      case -455:
       fixture->expected = 3;
       break;
      case 3:
       fixture->expected = 1;
       break;
      case 232323:
       fixture->expected = 6;
       break;
    }

    return (void*) fixture;
}
static MunitResult test_digit_counts_specific(const MunitParameter params[], void* fixture) {
    test_digit_counts_fixture* typed_fixture = (test_digit_counts_fixture*) fixture;
    int expected = typed_fixture->expected;
    int n = typed_fixture->n;

    int result = digitsCount(n);
    munit_assert_int(result, ==, expected);

    return MUNIT_OK;
}

// generated random numbers

static char* test_digit_counts_params_n_of_digits_random[] = {
  "1", "2", "3", "4", "5", NULL
};
static MunitParameterEnum test_digit_counts_params_random[] = {
  { "n_of_digits", test_digit_counts_params_n_of_digits_random },
  { NULL, NULL },
};
static void* test_digit_counts_setup_random(const MunitParameter params[], void* user_data) {
    test_digit_counts_fixture* fixture = (test_digit_counts_fixture*) malloc(sizeof(test_digit_counts_fixture));
    fixture->expected = atoi(params[0].value);

    int from = 1, to = 1;
    for (int i = 0; i < fixture->expected; i++) {
      to *= 10;
      from *= (i == fixture->expected - 1)
        ? 1
        : 10;
    }
    to--;

    fixture->n = munit_rand_int_range(from, to);

    return (void*) fixture;
}
static MunitResult test_digit_counts_random(const MunitParameter params[], void* fixture) {
    test_digit_counts_fixture* typed_fixture = (test_digit_counts_fixture*) fixture;
    int expected = typed_fixture->expected;
    int n = typed_fixture->n;

    int result = digitsCount(n);
    munit_assert_int(result, ==, expected);

    return MUNIT_OK;
}