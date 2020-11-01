#include "log_utils.test.h" 

// TEST SIGNATURES

// Test equality between result and expected

struct _test_content_equality_fixture {
    char* str;
};
typedef struct _test_content_equality_fixture test_content_equality_fixture;

static char* test_content_equality_params_str[];
static MunitParameterEnum test_content_equality_params[];

static MunitResult test_content_equality (const MunitParameter params[], void* fixture); //test
static void* test_content_equality_setup (const MunitParameter params[], void* user_data); //setup
static void test_content_equality_tear_down (void* fixture); //destroy

// EXPORTED SUITE

static MunitTest tests_log_utils[] = {
    {
        "/content-equality",
        test_content_equality,
        test_content_equality_setup,
        test_content_equality_tear_down,
        MUNIT_TEST_OPTION_NONE,
        test_content_equality_params
    }
};

static MunitSuite log_utils_suite = {
  "/log-utils",
  tests_log_utils,
  NULL,
  1,
  MUNIT_SUITE_OPTION_NONE
};

MunitSuite* get_log_utils_suite() {
  return &log_utils_suite;
}

// TEST DEFINITIONS

static char* test_content_equality_params_str[] = {
  "generic_string", "__#&aaaa", "", NULL
};

static MunitParameterEnum test_content_equality_params[] = {
  { "str", test_content_equality_params_str },
  { NULL, NULL },
};

// TO REMOVE
/*static void logger(char* s) {
    FILE* fp = fopen("debug_log_utils.txt", "a");
    fprintf(fp, "> %s\n", s);
    fclose(fp);
}*/

static MunitResult test_content_equality (const MunitParameter params[], void* fixture) {
    
    test_content_equality_fixture* typed_fixture = (test_content_equality_fixture*) fixture;
    redirect_config_t config = init_redirect_config();

    char* expected_info = (char*) malloc(TEST_UTILS_BUFFER_SIZE * sizeof(char));
    char* expected_success = (char*) malloc(TEST_UTILS_BUFFER_SIZE * sizeof(char));
    char* expected_debug = (char*) malloc(TEST_UTILS_BUFFER_SIZE * sizeof(char));
    char* expected_error = (char*) malloc(TEST_UTILS_BUFFER_SIZE * sizeof(char));
    char* expected_warning = (char*) malloc(TEST_UTILS_BUFFER_SIZE * sizeof(char));
    sprintf(expected_info, "%s[INFO]%s\t\t%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET, typed_fixture->str);
    sprintf(expected_success, "%s[SUCCESS]%s\t%s\n", ANSI_COLOR_GREEN, ANSI_COLOR_RESET, typed_fixture->str);
    sprintf(expected_debug, "%s[DEBUG]%s\t\t%s\n", ANSI_COLOR_DARK_GRAY, ANSI_COLOR_RESET, typed_fixture->str);
    sprintf(expected_error, "%s[ERROR]%s\t\t%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET, typed_fixture->str);
    sprintf(expected_warning, "%s[WARNING]%s\t%s\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET, typed_fixture->str);

    char* expected = (char*) malloc(5 * TEST_UTILS_BUFFER_SIZE * sizeof(char));
    sprintf(expected, "%s%s%s%s%s", expected_info, expected_success, expected_debug, expected_error, expected_warning);
    
    redirect_stdout(&config);

    logInfo(typed_fixture->str);
    logSuccess(typed_fixture->str);
    logDebug(typed_fixture->str);
    logError(typed_fixture->str);
    logWarning(typed_fixture->str);

    recover_stdout(&config);

    munit_assert_string_equal(config.buffer, expected);

    free(expected_info);
    free(expected_success);
    free(expected_debug);
    free(expected_error);
    free(expected_warning);
    free(expected);

    return MUNIT_OK;
}

static void* test_content_equality_setup (const MunitParameter params[], void* user_data) {
    test_content_equality_fixture* typed_fixture = (test_content_equality_fixture*) malloc(sizeof(test_content_equality_fixture));
    typed_fixture->str = params[0].value;
    return (void*) typed_fixture;
}

static void test_content_equality_tear_down (void* fixture) {
    test_content_equality_fixture* typed_fixture = (test_content_equality_fixture*) fixture;
    free(typed_fixture);
}