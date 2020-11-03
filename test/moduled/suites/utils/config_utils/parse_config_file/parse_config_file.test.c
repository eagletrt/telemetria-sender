#include "parse_config_file.test.h"

/* HELPER FUNCTIONS SIGNATURES */

typedef struct {
    config_t *config;
    int index;
    char* path;
} test_parse_config_file_fixture;

static char *test_parse_config_file_params_file_path_index[];
static MunitParameterEnum test_parse_config_file_params[];


static void *test_parse_config_file_setup(const MunitParameter params[], void *user_data);
static MunitResult test_parse_config_file_0(const config_t *config);
static MunitResult test_parse_config_file_1(const config_t *config);
static MunitResult test_parse_config_file_2(const config_t *config);
static MunitResult test_parse_config_file(const MunitParameter params[], void *fixture);
static void test_parse_config_file_tear_down(void *fixture);

/* EXPORTED SUITE */

static MunitTest tests_parse_config_file[] = {
    {
        "",
        test_parse_config_file,
        test_parse_config_file_setup,
        test_parse_config_file_tear_down,
        MUNIT_TEST_OPTION_NONE,
        test_parse_config_file_params
    },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static MunitSuite config_utils_parse_config_file_suite = {
    "/parse_config_file",
    tests_parse_config_file,
    NULL,
    1,
    MUNIT_SUITE_OPTION_NONE
};

MunitSuite *get_config_utils_parse_config_file_suite() {
    return &config_utils_parse_config_file_suite;
}

/* HELPER FUNCTIONS DEFINITIONS */

static char *test_parse_config_file_params_file_path_index[] = {
    "0", 
    "1", 
    "2",
    NULL
};
static MunitParameterEnum test_parse_config_file_params[] = {
    { "file_path_index", test_parse_config_file_params_file_path_index },
    { NULL, NULL }
};

static void *test_parse_config_file_setup(const MunitParameter params[], void *user_data) {
    test_parse_config_file_fixture *fixture = (test_parse_config_file_fixture*) malloc(sizeof(test_parse_config_file_fixture));
    fixture->config = newConfig();
    fixture->index = atoi(params[0].value);
    asprintf(&fixture->path, "test/moduled/suites/utils/config_utils/parse_config_file/assets/%d.config.json", fixture->index);
    return fixture;
}

static MunitResult test_parse_config_file_0(const config_t *config) {
    munit_assert_string_equal(config->mqtt.host, "localhost");
    munit_assert_int(config->mqtt.port, ==, 9001);
    munit_assert_string_equal(config->mqtt.data_topic, "data_topic");
    munit_assert_string_equal(config->mqtt.log_topic, "log_topic");

    munit_assert_string_equal(config->mongodb.host, "localhost");
    munit_assert_int(config->mongodb.port, ==, 23023);
    munit_assert_string_equal(config->mongodb.db, "eagletrt");
    munit_assert_string_equal(config->mongodb.collection, "scimmera");

    munit_assert_false(config->gps.plugged);
    munit_assert_true(config->gps.simulated);
    munit_assert_string_equal(config->gps.interface, "/dev/pts/23");

    munit_assert_int(config->pilots_count, ==, 6);
    munit_assert_string_equal(config->pilots[0], "default");
    munit_assert_string_equal(config->pilots[1], "Schumacher");
    munit_assert_string_equal(config->pilots[2], "Senna");
    munit_assert_string_equal(config->pilots[3], "Leclerc");
    munit_assert_string_equal(config->pilots[4], "Hamilton");
    munit_assert_string_equal(config->pilots[5], "Euber");

    munit_assert_int(config->circuits_count, ==, 3);
    munit_assert_string_equal(config->circuits[0], "default");
    munit_assert_string_equal(config->circuits[1], "Imola");
    munit_assert_string_equal(config->circuits[2], "Povo");

    munit_assert_int(config->races_count, ==, 3);
    munit_assert_string_equal(config->races[0], "default");
    munit_assert_string_equal(config->races[1], "Hellpad");
    munit_assert_string_equal(config->races[2], "Skipass");

    munit_assert_string_equal(config->can_interface, "vcan0");
    munit_assert_int(config->sending_rate, ==, 250);
    munit_assert_true(config->verbose);
}
static MunitResult test_parse_config_file_1(const config_t *config) {
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
}
static MunitResult test_parse_config_file_2(const config_t *config) {
    munit_assert_string_equal(config->mqtt.host, "localhost");
    munit_assert_int(config->mqtt.port, ==, 2323);
    munit_assert_string_equal(config->mqtt.data_topic, "telemetria");
    munit_assert_string_equal(config->mqtt.log_topic, "log");

    munit_assert_string_equal(config->mongodb.host, "localhost");
    munit_assert_int(config->mongodb.port, ==, 27017);
    munit_assert_string_equal(config->mongodb.db, "eagle_test");
    munit_assert_string_equal(config->mongodb.collection, "scimmera");

    munit_assert_true(config->gps.plugged);
    munit_assert_true(config->gps.simulated);
    munit_assert_string_equal(config->gps.interface, "/dev/pts/2");

    munit_assert_int(config->pilots_count, ==, 1);
    munit_assert_string_equal(config->pilots[0], "default");

    munit_assert_int(config->circuits_count, >=, 1);
    munit_assert_string_equal(config->circuits[0], "default");

    munit_assert_int(config->races_count, ==, 2);
    munit_assert_string_equal(config->races[0], "default");
    munit_assert_string_equal(config->races[1], "Autocross");

    munit_assert_string_equal(config->can_interface, "can0");
    munit_assert_int(config->sending_rate, ==, 250);
    munit_assert_false(config->verbose);
}

static MunitResult test_parse_config_file(const MunitParameter params[], void *fixture) {
    test_parse_config_file_fixture *typed_fixture = (test_parse_config_file_fixture *)fixture;
    int index = typed_fixture->index;
    char* path = typed_fixture->path;
    config_t* config = typed_fixture->config;

    parseConfigFile(path, &config);

    switch (index)
    {
        case 0:
            test_parse_config_file_0(config);
            break;

        case 1:
            test_parse_config_file_1(config);
            break;

        case 2:
            test_parse_config_file_2(config);
            break;
        
        default:
            puts("INVALID ASSET INDEX");
            return MUNIT_FAIL;
    }
    munit_assert_not_null(typed_fixture->config);

    return MUNIT_OK;
}

static void test_parse_config_file_tear_down(void *fixture) {
    test_parse_config_file_fixture *typed_fixture = (test_parse_config_file_fixture*) fixture;
    deleteConfig(typed_fixture->config);
    free(typed_fixture->path);
    free(typed_fixture);
}
