#include "handle_config.test.h"

/* HELPER FUNCTIONS SIGNATURES */

static MunitParameterEnum test_handle_config_params_correct[];
static MunitParameterEnum test_handle_config_params_wrong[];

static void *test_handle_config_setup(const MunitParameter params[], void *user_data);
static MunitResult test_handle_config_correct(const MunitParameter params[], void *fixture);
static MunitResult test_handle_config_wrong(const MunitParameter params[], void *fixture);

/* EXPORTED SUITE */

static MunitTest tests_handle_config[] = {
    {
        "/correct-file",
        test_handle_config_correct,
        test_handle_config_setup,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        test_handle_config_params_correct
    },
    {
        "/wrong-file",
        test_handle_config_wrong,
        test_handle_config_setup,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        test_handle_config_params_wrong
    },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static MunitSuite config_services_handle_config_suite = {
    "/handle_config",
    tests_handle_config,
    NULL,
    1,
    MUNIT_SUITE_OPTION_NONE};

MunitSuite *get_config_services_handle_config_suite() {
    return &config_services_handle_config_suite;
}

/* HELPER FUNCTIONS DEFINITIONS */
static char *test_handle_config_params_correct_n[] = {
    "test/moduled/suites/services/config_service/handle_config/assets/0.config.json",
    "test/moduled/suites/services/config_service/handle_config/assets/1.config.json",
    "test/moduled/suites/services/config_service/handle_config/assets/2.config.json",
    "test/moduled/suites/services/config_service/handle_config/assets/random-file.txt",
    "test/moduled/suites/services/config_service/handle_config/assets/wrong-config.json",
    NULL
};
static MunitParameterEnum test_handle_config_params_correct[] = {
    {"filename", test_handle_config_params_correct_n},
    {NULL, NULL},
};

static char *test_handle_config_params_wrong_n[] = {
    "",
    "fake/path/to/file.txt",
    NULL
};
static MunitParameterEnum test_handle_config_params_wrong[] = {
    {"filename", test_handle_config_params_wrong_n},
    {NULL, NULL},
};

static void *test_handle_config_setup(const MunitParameter params[], void *user_data) {
    condition.config_path = params[0].value;
	freopen("/dev/null", "w", stdout);
}

static MunitResult test_handle_config_correct(const MunitParameter params[], void *fixture) {
    munit_assert_true(handleConfig() == CONFIG_OK);
    return MUNIT_OK;
}

static MunitResult test_handle_config_wrong(const MunitParameter params[], void *fixture) {
    munit_assert_true(handleConfig() == CONFIG_ERROR);
    return MUNIT_OK;
}
