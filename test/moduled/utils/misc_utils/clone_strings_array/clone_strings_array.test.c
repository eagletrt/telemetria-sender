#include "clone_strings_array.test.h"

/* HELPER FUNCTIONS SIGNATURES */

struct _test_clone_strings_array_fixture {
    int n;
    char** expected;
    char** initial;
};
typedef struct _test_clone_strings_array_fixture test_clone_strings_array_fixture;

// Test n = size

static char* test_clone_strings_array_params_n_0[];
static MunitParameterEnum test_clone_strings_array_params_0[];

static void* test_clone_strings_array_setup_0(const MunitParameter params[], void* user_data);
static MunitResult test_clone_strings_array_0(const MunitParameter params[], void* fixture);
static void test_clone_strings_array_tear_down_0(void* fixture);

// Test 0 <= n < size

static char* test_clone_strings_array_params_n_1[];
static MunitParameterEnum test_clone_strings_array_params_1[];

static void* test_clone_strings_array_setup_1(const MunitParameter params[], void* user_data);
static MunitResult test_clone_strings_array_1(const MunitParameter params[], void* fixture);
static void test_clone_strings_array_tear_down_1(void* fixture);

/* EXPORTED TESTS */

MunitTest const tests_clone_strings_array[] = {
  {
    "/clone-strings-array/n-equal-to-size",
    test_clone_strings_array_0,
    test_clone_strings_array_setup_0,
    test_clone_strings_array_tear_down_0,
    MUNIT_TEST_OPTION_NONE,
    test_clone_strings_array_params_0
  },
  {
    "/clone-strings-array/n-lower-than-size",
    test_clone_strings_array_1,
    test_clone_strings_array_setup_1,
    test_clone_strings_array_tear_down_1,
    MUNIT_TEST_OPTION_NONE,
    test_clone_strings_array_params_1
  },
  { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

/* HELPER FUNCTIONS DEFINITIONS */

// Test n = size

static char* test_clone_strings_array_params_n_0[] = {
  "0", "1", "2", "3", NULL
};
static MunitParameterEnum test_clone_strings_array_params_0[] = {
  { "n", test_clone_strings_array_params_n_0 },
  { NULL, NULL },
};
static void* test_clone_strings_array_setup_0(const MunitParameter params[], void* user_data) {
    test_clone_strings_array_fixture* fixture = (test_clone_strings_array_fixture*) malloc(sizeof(test_clone_strings_array_fixture));
    fixture->n = atoi(params[0].value);

    if (fixture->n > 0) {
        fixture->expected = (char**) malloc(fixture->n * sizeof(char*));
        fixture->initial = (char**) malloc(fixture->n * sizeof(char*));
        for (int i = 0; i < fixture->n; i++) {
            char *buffer = (char*) malloc(8 * sizeof(char));
            sprintf(buffer, "%d", i);

            fixture->expected[i] = strdup(buffer);
            fixture->initial[i] = buffer;
        }
    }
    else {
        fixture->expected = NULL;
        fixture->initial = NULL;
    }

    return (void*) fixture;
}
static MunitResult test_clone_strings_array_0(const MunitParameter params[], void* fixture) {
    test_clone_strings_array_fixture* typed_fixture = (test_clone_strings_array_fixture*) fixture;
    char** expected = typed_fixture->expected;
    char **initial = typed_fixture->initial;
    int initial_n = typed_fixture->n;

    int n = initial_n;
    char **cloned = cloneStringsArray(initial, initial_n);
    freeStringsArray(initial, &n);
    
    if (initial_n > 0) {
      for (int i = 0; i < initial_n; i++) {
        munit_assert_string_equal(cloned[i], expected[i]);
      }
    }
    else {
      munit_assert_null(cloned);
    }

    return MUNIT_OK;
}
static void test_clone_strings_array_tear_down_0(void* fixture) {
    test_clone_strings_array_fixture* typed_fixture = (test_clone_strings_array_fixture*) fixture;
    freeStringsArray(typed_fixture->expected, &typed_fixture->n);
    free(typed_fixture);
}

// Test 0 <= n < size

static char* test_clone_strings_array_params_n_1[] = {
  "1", "2", "3", NULL
};
static MunitParameterEnum test_clone_strings_array_params_1[] = {
  { "n", test_clone_strings_array_params_n_1 },
  { NULL, NULL },
};
static void* test_clone_strings_array_setup_1(const MunitParameter params[], void* user_data) {
    test_clone_strings_array_fixture* fixture = (test_clone_strings_array_fixture*) malloc(sizeof(test_clone_strings_array_fixture));
    fixture->n = atoi(params[0].value);

    if (fixture->n > 1) {
        fixture->expected = (char**) malloc((fixture->n - 1) * sizeof(char*));
        fixture->initial = (char**) malloc(fixture->n * sizeof(char*));
        for (int i = 0; i < fixture->n; i++) {
            char *buffer = (char*) malloc(8 * sizeof(char));
            sprintf(buffer, "%d", i);

            if (i < fixture->n - 1) {
              fixture->expected[i] = strdup(buffer);
            }
            fixture->initial[i] = buffer;
        }
    }
    else if (fixture->n == 1) {
      fixture->expected = NULL;
      fixture->initial = (char**) malloc(fixture->n * sizeof(char*));
      fixture->initial[0] = strdup("0");
    }
    else {
        fixture->expected = NULL;
        fixture->initial = NULL;
    }

    return (void*) fixture;
}
static MunitResult test_clone_strings_array_1(const MunitParameter params[], void* fixture) {
    test_clone_strings_array_fixture* typed_fixture = (test_clone_strings_array_fixture*) fixture;
    char** expected = typed_fixture->expected;
    char **initial = typed_fixture->initial;
    int initial_n = typed_fixture->n;

    int n = initial_n;
    char **cloned = cloneStringsArray(initial, initial_n - 1);
    freeStringsArray(initial, &n);
    
    if (initial_n > 1) {
      for (int i = 0; i < initial_n - 1; i++) {
          munit_assert_string_equal(cloned[i], expected[i]);
      }
    }
    else {
      munit_assert_null(cloned);
    }

    return MUNIT_OK;
}
static void test_clone_strings_array_tear_down_1(void* fixture) {
    test_clone_strings_array_fixture* typed_fixture = (test_clone_strings_array_fixture*) fixture;
    int n = typed_fixture->n - 1 > 0 ? typed_fixture->n - 1 : 0;
    freeStringsArray(typed_fixture->expected, &n);
    free(typed_fixture);
}