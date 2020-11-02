#include "new_config.test.h"

/* HELPER FUNCTIONS SIGNATURES */

struct _test_new_config_fixture
{
    config_t *config;
};
typedef struct _test_new_config_fixture test_new_config_fixture;

static char *test_new_config_params_n[];
static MunitParameterEnum test_new_config_params[];

static void *test_new_config_setup(const MunitParameter params[], void *user_data);
static MunitResult test_new_config(const MunitParameter params[], void *fixture);
static void test_new_config_tear_down(void *fixture);

/* EXPORTED SUITE */

static MunitTest tests_new_config[] = {
    {"",
     test_new_config,
     test_new_config_setup,
     test_new_config_tear_down,
     MUNIT_TEST_OPTION_NONE,
     test_new_config_params},
    {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}};

static MunitSuite config_utils_new_config_suite = {
    "/new_config",
    tests_new_config,
    NULL,
    1,
    MUNIT_SUITE_OPTION_NONE};

MunitSuite *get_config_utils_new_config_suite()
{
    return &config_utils_new_config_suite;
}

static char *test_new_config_params_n[] = {"", NULL};
static MunitParameterEnum test_new_config_params[] = {
    {"filename", test_new_config_params_n},
    {NULL, NULL},
};

static void *test_new_config_setup(const MunitParameter params[], void *user_data)
{
    test_new_config_fixture *fixture = (test_new_config_fixture *)malloc(sizeof(test_new_config_fixture));
}

static MunitResult test_new_config(const MunitParameter params[], void *fixture)
{
    test_new_config_fixture *typed_fixture = (test_new_config_fixture *)fixture;
    typed_fixture->config = newConfig();
    return MUNIT_OK;
}

static void test_new_config_tear_down(void *fixture)
{
    test_new_config_fixture *typed_fixture = (test_new_config_fixture *)malloc(sizeof(test_new_config_fixture));
    //deleteConfig(&(typed_fixture->config)); //TODO: fix
}
