#include "log_utils.test.h" 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_LEN 50

// TEST SIGNATURES

// Test equality between result and expected

struct _test_content_equality_fixture {
    int n;
    char* pbuffer;
    int stdout_save;
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
  "stringa1", "stringa2", "__#&aaaa", " ", NULL
};

static MunitParameterEnum test_content_equality_params[] = {
  { "n", test_content_equality_params_str },
  { NULL, NULL },
};

static MunitResult test_content_equality (const MunitParameter params[], void* fixture) {
    
    //TO DO
    //ASSERTIONS

    return (MunitResult) NULL;
}

static void* test_content_equality_setup (const MunitParameter params[], void* user_data) {
    
    // TO DO
    // STDOUT REDIRECT

    test_content_equality_fixture* fixture = (test_content_equality_fixture*) malloc(sizeof(test_content_equality_fixture));
    fixture->n = atoi(params[0].value);
    fixture->stdout_save = 0;
    return  NULL;
}

static void test_content_equality_tear_down (void* fixture) {

    // TO DO
    // STDOUT REDIRECT (REVERSE)


    test_content_equality_fixture* this_fixture = (test_content_equality_fixture* )fixture;
    
    free(this_fixture);
    return;
}