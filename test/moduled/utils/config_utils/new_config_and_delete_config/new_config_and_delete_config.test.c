#include "new_config_and_delete_config.test.h"

/* HELPER FUNCTIONS SIGNATURES */

static char *test_new_config_and_delete_config_params_n[];
static MunitParameterEnum test_new_config_and_delete_config_params[];

static MunitResult test_new_config_and_delete_config(const MunitParameter params[], void *fixture);

/* EXPORTED SUITE */

static MunitTest tests_new_config_and_delete_config[] = {
    {"",
     test_new_config_and_delete_config,
     NULL,
     NULL,
     MUNIT_TEST_OPTION_NONE,
     test_new_config_and_delete_config_params},
    {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}};

static MunitSuite config_utils_new_config_and_delete_config_suite = {
    "/new_config_and_delete_config",
    tests_new_config_and_delete_config,
    NULL,
    1,
    MUNIT_SUITE_OPTION_NONE};

MunitSuite *get_config_utils_new_config_and_delete_config_suite()
{
    return &config_utils_new_config_and_delete_config_suite;
}

static char *test_new_config_and_delete_config_params_n[] = {
    "0", NULL};
static MunitParameterEnum test_new_config_and_delete_config_params[] = {
    {"n", test_new_config_and_delete_config_params_n},
    {NULL, NULL},
};

static MunitResult test_new_config_and_delete_config(const MunitParameter params[], void *fixture)
{
    config_t* config = newConfig();
    deleteConfig(config);
}