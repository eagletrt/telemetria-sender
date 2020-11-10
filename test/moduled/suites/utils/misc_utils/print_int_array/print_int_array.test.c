#include "print_int_array.test.h"

/* HELPER FUNCTIONS SIGNATURES */

typedef struct {
    int n;
    int* array;
    char* expected;
} test_print_int_array_fixture;

// Test n = size

static char* test_print_int_array_params_n_0[];
static MunitParameterEnum test_print_int_array_params_0[];

static void* test_print_int_array_setup_0(const MunitParameter params[], void* user_data);
static MunitResult test_print_int_array_0(const MunitParameter params[], void* fixture);
static void test_print_int_array_tear_down_0(void* fixture);

// Test 0 <= n < size

static char* test_print_int_array_params_n_1[];
static MunitParameterEnum test_print_int_array_params_1[];

static void* test_print_int_array_setup_1(const MunitParameter params[], void* user_data);
static MunitResult test_print_int_array_1(const MunitParameter params[], void* fixture);
static void test_print_int_array_tear_down_1(void* fixture);

/* EXPORTED SUITE */

static MunitTest tests_print_int_array[] = {
  {
    "/n-equal-to-size",
    test_print_int_array_0,
    test_print_int_array_setup_0,
    test_print_int_array_tear_down_0,
    MUNIT_TEST_OPTION_NONE,
    test_print_int_array_params_0
  },
  {
    "/n-lower-than-size",
    test_print_int_array_1,
    test_print_int_array_setup_1,
    test_print_int_array_tear_down_1,
    MUNIT_TEST_OPTION_NONE,
    test_print_int_array_params_1
  },
  { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static MunitSuite misc_utils_print_int_array_suite = {
  "/print-int-array",
  tests_print_int_array,
  NULL,
  1,
  MUNIT_SUITE_OPTION_NONE
};

MunitSuite* get_misc_utils_print_int_array_suite() {
  return &misc_utils_print_int_array_suite;
}

/* HELPER FUNCTIONS DEFINITIONS */

// Test n = size

static char* test_print_int_array_params_n_0[] = {
  "0", "1", "2", "3", NULL
};
static MunitParameterEnum test_print_int_array_params_0[] = {
  { "n", test_print_int_array_params_n_0 },
  { NULL, NULL },
};
static void* test_print_int_array_setup_0(const MunitParameter params[], void* user_data) {
    test_print_int_array_fixture* fixture = (test_print_int_array_fixture*) malloc(sizeof(test_print_int_array_fixture));
    fixture->n = atoi(params[0].value);

    if (fixture->n > 0) {
        const int MAX_SIZE = 1000;
        fixture->array = (int*) malloc(fixture->n * sizeof(int));
        fixture->expected = (char*) malloc(MAX_SIZE * sizeof(char));
        fixture->expected[0] = '\0';
        strcat(fixture->expected, "["); 
        for (int i = 0; i < fixture->n; i++) {
            char *buffer = (char*) malloc(8 * sizeof(char));
            sprintf(buffer, "%d", i);
            strcat(fixture->expected, "\n\t");
            strcat(fixture->expected, buffer);
            free(buffer);

            fixture->array[i] = i;
        }
        strcat(fixture->expected, "\n]\n");
    }
    else {
        fixture->array = NULL;
        fixture->expected = strdup("[]\n");
    }

    return (void*) fixture;
}
static MunitResult test_print_int_array_0(const MunitParameter params[], void* fixture) {
    test_print_int_array_fixture* typed_fixture = (test_print_int_array_fixture*) fixture;
    char* expected = typed_fixture->expected;
    int* array = typed_fixture->array;
    int n = typed_fixture->n;

    redirect_config_t* redirect_config = init_redirect_config();
    redirect_stdout(redirect_config);
    printIntArray(array, n);
    recover_stdout(redirect_config);

    munit_assert_string_equal(redirect_config->buffer, expected);

    return MUNIT_OK;
}
static void test_print_int_array_tear_down_0(void* fixture) {
    test_print_int_array_fixture* typed_fixture = (test_print_int_array_fixture*) fixture;
    free(typed_fixture->array);
    free(typed_fixture->expected);
    free(typed_fixture);
}

// Test 0 <= n < size 

static char* test_print_int_array_params_n_1[] = {
  "0", "1", "2", "3", NULL
};
static MunitParameterEnum test_print_int_array_params_1[] = {
  { "n", test_print_int_array_params_n_1 },
  { NULL, NULL },
};
static void* test_print_int_array_setup_1(const MunitParameter params[], void* user_data) {
    test_print_int_array_fixture* fixture = (test_print_int_array_fixture*) malloc(sizeof(test_print_int_array_fixture));
    fixture->n = atoi(params[0].value);

    if (fixture->n > 1) {
        const int MAX_SIZE = 1000;
        fixture->array = (int*) malloc(fixture->n * sizeof(int));
        fixture->expected = (char*) malloc(MAX_SIZE * sizeof(char));
        fixture->expected[0] = '\0';
        strcat(fixture->expected, "["); 
        for (int i = 0; i < fixture->n; i++) {
            fixture->array[i] = i;

            if (i != fixture->n - 1) {
              char *buffer = (char*) malloc(8 * sizeof(char));
              sprintf(buffer, "%d", i);
              strcat(fixture->expected, "\n\t");
              strcat(fixture->expected, buffer);
              free(buffer);
            }
        }
        strcat(fixture->expected, "\n]\n");
    }
    else if (fixture->n == 1) {
      fixture->array = (int*) malloc(fixture->n * sizeof(int));
      fixture->array[0] = 0;
      fixture->expected = strdup("[]\n");
    }
    else {
        fixture->array = NULL;
        fixture->expected = strdup("[]\n");
    }

    return (void*) fixture;
}
static MunitResult test_print_int_array_1(const MunitParameter params[], void* fixture) {
    test_print_int_array_fixture* typed_fixture = (test_print_int_array_fixture*) fixture;
    char* expected = typed_fixture->expected;
    int* array = typed_fixture->array;
    int n = typed_fixture->n;

    redirect_config_t* redirect_config = init_redirect_config();
    redirect_stdout(redirect_config);
    printIntArray(array, n - 1);
    recover_stdout(redirect_config);

    munit_assert_string_equal(redirect_config->buffer, expected);

    return MUNIT_OK;
}
static void test_print_int_array_tear_down_1(void* fixture) {
    test_print_int_array_fixture* typed_fixture = (test_print_int_array_fixture*) fixture;
    free(typed_fixture->array);
    free(typed_fixture->expected);
    free(typed_fixture);
}