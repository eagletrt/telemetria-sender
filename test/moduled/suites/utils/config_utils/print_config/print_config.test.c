#include "print_config.test.h"

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

/* HELPER FUNCTIONS SIGNATURES */

struct _test_print_config_fixture {
	config_t *config;
};
typedef struct _test_print_config_fixture test_print_config_fixture;

static char *test_print_config_params_n[];
static MunitParameterEnum test_print_config_params[];

static void *test_print_config_setup(const MunitParameter params[], void *user_data);
static MunitResult test_print_config(const MunitParameter params[], void *fixture);
static void test_print_config_tear_down(void *fixture);

/* EXPORTED SUITE */

static MunitTest tests_print_config[] = {
	{"",
	 test_print_config,
	 test_print_config_setup,
	 test_print_config_tear_down,
	 MUNIT_TEST_OPTION_NONE,
	 test_print_config_params},
	{NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}};

static MunitSuite config_utils_print_config_suite = {
	"/print_config",
	tests_print_config,
	NULL,
	1,
	MUNIT_SUITE_OPTION_NONE};

MunitSuite *get_config_utils_print_config_suite() {
	return &config_utils_print_config_suite;
}

/* HELPER FUNCTIONS DEFINITIONS */

static char *test_print_config_params_n[] = {"", "test/assets/config_file/config.json", NULL};
static MunitParameterEnum test_print_config_params[] = {
	{"filename", test_print_config_params_n},
	{NULL, NULL},
};

static void *test_print_config_setup(const MunitParameter params[], void *user_data) {
	test_print_config_fixture *fixture = (test_print_config_fixture *) malloc(sizeof(test_print_config_fixture));
	config_t *config = newConfig();
	if (strlen(params[0].value) > 0) {
		parseConfigFile(params[0].value, &config);
	}
	fixture->config = config;
	freopen("/dev/null", "w", stdout);
	return fixture;
}

static MunitResult test_print_config(const MunitParameter params[], void *fixture) {
	test_print_config_fixture *typed_fixture = (test_print_config_fixture *)fixture;
	printConfig(typed_fixture->config);
	return MUNIT_OK;
}

static void test_print_config_tear_down(void *fixture) {
	test_print_config_fixture *typed_fixture = (test_print_config_fixture *)fixture;
	deleteConfig(typed_fixture->config);
	free(typed_fixture);
}
