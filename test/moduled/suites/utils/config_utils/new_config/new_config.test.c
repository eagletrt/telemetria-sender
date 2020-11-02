#include "new_config.test.h"

/* HELPER FUNCTIONS SIGNATURES */

static MunitResult test_new_config(const MunitParameter params[], void *fixture);

/* EXPORTED SUITE */

static MunitTest tests_new_config[] = {
    {
        "",
        test_new_config,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static MunitSuite config_utils_new_config_suite = {
    "/new_config",
    tests_new_config,
    NULL,
    1,
    MUNIT_SUITE_OPTION_NONE};

MunitSuite *get_config_utils_new_config_suite() {
    return &config_utils_new_config_suite;
}

/* HELPER FUNCTIONS DEFINITIONS */

static MunitResult test_new_config(const MunitParameter params[], void *fixture)
{
    config_t *config = newConfig();

    munit_assert_string_equal(config->mqtt.host, "localhost");
    munit_assert_int(config->mqtt.port, ==, 1883);
    munit_assert_string_equal(config->mqtt.data_topic, "telemetria");
    munit_assert_string_equal(config->mqtt.log_topic, "telemetria_log");

    deleteConfig(config);

    return MUNIT_OK;
}
