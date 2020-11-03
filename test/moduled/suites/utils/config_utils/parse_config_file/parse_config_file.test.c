#include "parse_config_file.test.h"

/* HELPER FUNCTIONS SIGNATURES */

struct _test_parse_config_file_fixture
{
    config_t *config;
};
typedef struct _test_parse_config_file_fixture test_parse_config_file_fixture;

static char *test_parse_config_file_params_n[];
static MunitParameterEnum test_parse_config_file_params[];

static void *test_parse_config_file_setup(const MunitParameter params[], void *user_data);
static MunitResult test_parse_config_file(const MunitParameter params[], void *fixture);
static void test_parse_config_file_tear_down(void *fixture);

/* EXPORTED SUITE */

static MunitTest tests_parse_config_file[] = {
    {"",
     test_parse_config_file,
     test_parse_config_file_setup,
     test_parse_config_file_tear_down,
     MUNIT_TEST_OPTION_NONE,
     test_parse_config_file_params},
    {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}};

static MunitSuite config_utils_parse_config_file_suite = {
    "/parse_config_file",
    tests_parse_config_file,
    NULL,
    1,
    MUNIT_SUITE_OPTION_NONE};

MunitSuite *get_config_utils_parse_config_file_suite()
{
    return &config_utils_parse_config_file_suite;
}

static char *test_parse_config_file_params_n[] = {"test/assets/config_file/config.json", NULL};
static MunitParameterEnum test_parse_config_file_params[] = {
    {"filename", test_parse_config_file_params_n},
    {NULL, NULL},
};

static void *test_parse_config_file_setup(const MunitParameter params[], void *user_data)
{
    test_parse_config_file_fixture *fixture = (test_parse_config_file_fixture *)malloc(sizeof(test_parse_config_file_fixture));
    config_t *config = newConfig();
    fixture->config = config;
    return fixture;
}

static MunitResult test_parse_config_file(const MunitParameter params[], void *fixture)
{
    test_parse_config_file_fixture *typed_fixture = (test_parse_config_file_fixture *)fixture;

    parseConfigFile(params[0].value, &(typed_fixture->config));
    munit_assert_not_null(typed_fixture->config);

    return MUNIT_OK;
}

static void test_parse_config_file_tear_down(void *fixture)
{
    test_parse_config_file_fixture *typed_fixture = (test_parse_config_file_fixture *)fixture;
    printf(typed_fixture->config->mqtt.log_topic);
    deleteConfig(typed_fixture->config);
    free(typed_fixture);
}
