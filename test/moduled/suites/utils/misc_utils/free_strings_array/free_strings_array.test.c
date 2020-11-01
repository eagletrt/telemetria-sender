#include "free_strings_array.test.h"

/* HELPER FUNCTIONS SIGNATURES */

struct _test_free_strings_array_fixture {
    int n;
    char** strings_array;
};
typedef struct _test_free_strings_array_fixture test_free_strings_array_fixture;

static char* test_free_strings_array_params_n[];
static MunitParameterEnum test_free_strings_array_params[];

static void* test_free_strings_array_setup(const MunitParameter params[], void* user_data);
static MunitResult test_free_strings_array(const MunitParameter params[], void* fixture);

/* EXPORTED SUITE */

static MunitTest tests_free_strings_array[] = {
  {
    "",
    test_free_strings_array,
    test_free_strings_array_setup,
    NULL,
    MUNIT_TEST_OPTION_NONE,
    test_free_strings_array_params
  },
  { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static MunitSuite misc_utils_free_strings_array_suite = {
  "/free-strings-array",
  tests_free_strings_array,
  NULL,
  1,
  MUNIT_SUITE_OPTION_NONE
};

MunitSuite* get_misc_utils_free_strings_array_suite() {
  return &misc_utils_free_strings_array_suite;
}

/* HELPER FUNCTIONS DEFINITIONS */

static char* test_free_strings_array_params_n[] = {
  "0", "1", "2", "3", NULL
};
static MunitParameterEnum test_free_strings_array_params[] = {
  { "n", test_free_strings_array_params_n },
  { NULL, NULL },
};
static void* test_free_strings_array_setup(const MunitParameter params[], void* user_data) {
    test_free_strings_array_fixture* fixture = (test_free_strings_array_fixture*) malloc(sizeof(test_free_strings_array_fixture));
    fixture->n = atoi(params[0].value);

    if (fixture->n > 0) {
        fixture->strings_array = (char**) malloc(fixture->n * sizeof(char*));
        for (int i = 0; i < fixture->n; i++) {
            char *buffer = (char*) malloc(8 * sizeof(char));
            sprintf(buffer, "%d", i);

            fixture->strings_array[i] = buffer;
        }
    }
    else {
        fixture->strings_array = NULL;
    }

    return (void*) fixture;
}
static MunitResult test_free_strings_array(const MunitParameter params[], void* fixture) {
    test_free_strings_array_fixture* typed_fixture = (test_free_strings_array_fixture*) fixture;
    char** strings_array = typed_fixture->strings_array;
    int n = typed_fixture->n;

    freeStringsArray(strings_array, &n);
    munit_assert_int(n, ==, 0);

    return MUNIT_OK;
}