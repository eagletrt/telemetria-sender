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

    munit_assert_string_equal(config->mongodb.host, "localhost");
    munit_assert_int(config->mongodb.port, ==, 27017);
    munit_assert_string_equal(config->mongodb.db, "eagle_test");
    munit_assert_string_equal(config->mongodb.collection, "chimera");

    munit_assert_true(config->gps.plugged);
    munit_assert_true(config->gps.simulated);
    munit_assert_string_equal(config->gps.interface, "/dev/pts/4");

    munit_assert_int(config->pilots_count, >=, 1);
    munit_assert_string_equal(config->pilots[0], "default");

    munit_assert_int(config->circuits_count, >=, 1);
    munit_assert_string_equal(config->circuits[0], "default");

    munit_assert_int(config->races_count, >=, 1);
    munit_assert_string_equal(config->races[0], "default");

    munit_assert_string_equal(config->can_interface, "can0");
    munit_assert_int(config->sending_rate, ==, 500);
    munit_assert_false(config->verbose);

    deleteConfig(config);

    return MUNIT_OK;
}
