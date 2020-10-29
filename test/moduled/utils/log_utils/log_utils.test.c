#include "log_utils.test.h" 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_LEN 100

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

// UTILS SIGNATURES

struct _redirect_config {
    char buffer[5*BUFFER_LEN+1];
    int out_pipe[2];
    int saved_stdout;
};
typedef struct _redirect_config redirect_config;
static redirect_config init_redirect_config();
static void redirect_stdout(redirect_config* config);
static void recover_stdout(redirect_config* config);

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
    redirect_config config = init_redirect_config();

    char* expected_info = (char*) malloc(BUFFER_LEN * sizeof(char));
    char* expected_success = (char*) malloc(BUFFER_LEN * sizeof(char));
    char* expected_debug = (char*) malloc(BUFFER_LEN * sizeof(char));
    char* expected_error = (char*) malloc(BUFFER_LEN * sizeof(char));
    char* expected_warning = (char*) malloc(BUFFER_LEN * sizeof(char));
    sprintf(expected_info, "%s[INFO]%s\t\t%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET, typed_fixture->str);
    sprintf(expected_success, "%s[SUCCESS]%s\t%s\n", ANSI_COLOR_GREEN, ANSI_COLOR_RESET, typed_fixture->str);
    sprintf(expected_debug, "%s[DEBUG]%s\t\t%s\n", ANSI_COLOR_DARK_GRAY, ANSI_COLOR_RESET, typed_fixture->str);
    sprintf(expected_error, "%s[ERROR]%s\t\t%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET, typed_fixture->str);
    sprintf(expected_warning, "%s[WARNING]%s\t%s\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET, typed_fixture->str);

    char* expected = (char*) malloc(5 * BUFFER_LEN * sizeof(char));
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

// UTILS DEFINITIONS

static redirect_config init_redirect_config() {
    redirect_config temp = {.buffer = {0}};
    return temp;
}

static void redirect_stdout(redirect_config* config) {
    config->saved_stdout = dup(STDOUT_FILENO); //salvataggio addr di stdout
    if( pipe(config->out_pipe) != 0 ) exit(1); //apertura pipe
    dup2(config->out_pipe[1], STDOUT_FILENO); //ridirezione stdout nel pipe
    close(config->out_pipe[1]); //chiusura pipe
}

static void recover_stdout(redirect_config* config) {
    fflush(stdout);
    read(config->out_pipe[0], config->buffer, 5*BUFFER_LEN); //lettura pipe nel buffer
    dup2(config->saved_stdout, STDOUT_FILENO); //ripristino stdout
}

