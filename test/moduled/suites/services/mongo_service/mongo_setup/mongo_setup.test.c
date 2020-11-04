#include "mongo_setup.test.h"

/* HELPER FUNCTIONS SIGNATURES */
static MunitParameterEnum test_mongo_setup_params_correct[];
static MunitParameterEnum test_mongo_setup_params_wrong[];

static void *test_mongo_setup(const MunitParameter params[], void *user_data);
static MunitResult test_mongo_setup_correct(const MunitParameter params[], void *fixture);
static MunitResult test_mongo_setup_wrong(const MunitParameter params[], void *fixture);
static void test_mongo_setup_wrong_tear_down(void* fixture);

/* EXPORTED SUITE */

static MunitTest tests_mongo_setup[] = {
    {
        "/correct-file",
        test_mongo_setup_correct,
        test_mongo_setup,
        test_mongo_setup_wrong_tear_down,
        MUNIT_TEST_OPTION_NONE,
        test_mongo_setup_params_correct
    },
    {
        "/wrong-file",
        test_mongo_setup_wrong,
        test_mongo_setup,
        test_mongo_setup_wrong_tear_down,
        MUNIT_TEST_OPTION_NONE,
        test_mongo_setup_params_wrong
    },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static MunitSuite mongo_services_mongo_setup_suite = {
    "/mongo_setup",
    tests_mongo_setup,
    NULL,
    1,
    MUNIT_SUITE_OPTION_NONE};

MunitSuite *get_mongo_services_mongo_setup_suite() {
    return &mongo_services_mongo_setup_suite;
}

/* HELPER FUNCTIONS DEFINITIONS */
static char *test_mongo_setup_params_correct_n[] = {
    "localhost|27017|eagle_test|test",
    NULL
};
static MunitParameterEnum test_mongo_setup_params_correct[] = {
    {"mongo", test_mongo_setup_params_correct_n},
    {NULL, NULL},
};

static char *test_mongo_setup_params_wrong_n[] = {
    "fakeurl|27017|eagle_test|test",
    "localhost|28123|eagle_test|test",
    "localhost|27017|wrong_db|test",
    "localhost|27017|eagle_test|test",
    "fakeurl|28123|wrong_db|test",
    NULL
};
static MunitParameterEnum test_mongo_setup_params_wrong[] = {
    {"mongo", test_mongo_setup_params_wrong_n},
    {NULL, NULL},
};

static void *test_mongo_setup(const MunitParameter params[], void *user_data) {
    char arg[100];
    strcpy(arg, params[0].value);
    char* host;
    char* port;
    char* db;
    char* collection;
    host = strtok(arg, "|");
    port = strtok(NULL, "|");
    db = strtok(NULL, "|");
    collection = strtok(NULL, "|");
    condition.mongodb.host = (char *)malloc((strlen(host) + 1) * sizeof(char));
    condition.mongodb.port = atoi(port);
    condition.mongodb.db = (char *)malloc((strlen(db) + 1) * sizeof(char));;
    condition.mongodb.collection = (char *)malloc((strlen(collection) + 1) * sizeof(char));;
    strcpy(condition.mongodb.host, host);
    strcpy(condition.mongodb.db, db);
    strcpy(condition.mongodb.collection, collection);
}

static MunitResult test_mongo_setup_correct(const MunitParameter params[], void *fixture) {
    int res = mongoSetup();
    munit_assert_true(res == MONGO_OK);
    return MUNIT_OK;
}

static MunitResult test_mongo_setup_wrong(const MunitParameter params[], void *fixture) {
    int res = mongoSetup();
    munit_assert_true(res == MONGO_OK);
    return MUNIT_OK;
}

static void test_mongo_setup_wrong_tear_down(void* fixture) {
    free(condition.mongodb.host);
    free(condition.mongodb.db);
    free(condition.mongodb.collection);
}

