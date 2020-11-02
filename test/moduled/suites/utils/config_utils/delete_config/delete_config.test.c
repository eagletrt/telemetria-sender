#include "delete_config.test.h"

/* HELPER FUNCTIONS SIGNATURES */

typedef struct {
	config_t *config;
} test_delete_config_fixture;

static char *test_delete_config_params_n[];
static MunitParameterEnum test_delete_config_params[];

static void *test_delete_config_setup(const MunitParameter params[], void *user_data);
static MunitResult test_delete_config(const MunitParameter params[], void *fixture);
static void test_delete_config_tear_down(void *fixture);

/* EXPORTED SUITE */

static MunitTest tests_delete_config[] = {
	{
		"",
		test_delete_config,
		test_delete_config_setup,
		test_delete_config_tear_down,
		MUNIT_TEST_OPTION_NONE,
		test_delete_config_params
	},
	{ NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static MunitSuite config_utils_delete_config_suite = {
	"/delete_config",
	tests_delete_config,
	NULL,
	1,
	MUNIT_SUITE_OPTION_NONE};

MunitSuite *get_config_utils_delete_config_suite() {
	return &config_utils_delete_config_suite;
}

/* HELPER FUNCTIONS DEFINITIONS */

static char *test_delete_config_params_n[] = {
	"", 
	"test/moduled/suites/utils/config_utils/delete_config/assets/0.config.json", 
	"test/moduled/suites/utils/config_utils/delete_config/assets/1.config.json", 
	"test/moduled/suites/utils/config_utils/delete_config/assets/2.config.json", 
	NULL
};

static MunitParameterEnum test_delete_config_params[] = {
	{ "filename", test_delete_config_params_n },
	{ NULL, NULL },
};

static void *test_delete_config_setup(const MunitParameter params[], void *user_data) {
	test_delete_config_fixture *fixture = (test_delete_config_fixture *) malloc(sizeof(test_delete_config_fixture));
	config_t *config = newConfig();
	if (strlen(params[0].value) > 0) {
		parseConfigFile(params[0].value, &config);
	}
	fixture->config = config;
}

static MunitResult test_delete_config(const MunitParameter params[], void *fixture) {
	test_delete_config_fixture *typed_fixture = (test_delete_config_fixture *)fixture;
	deleteConfig(typed_fixture->config);
	return MUNIT_OK;
}

static void test_delete_config_tear_down(void *fixture) {
	test_delete_config_fixture *typed_fixture = (test_delete_config_fixture *) malloc(sizeof(test_delete_config_fixture));
}
