#include "print_config.test.h"

/* HELPER FUNCTIONS SIGNATURES */

static void *test_print_config_setup(const MunitParameter params[], void *user_data);
static MunitResult test_print_config(const MunitParameter params[], void *fixture);
static void test_print_config_tear_down(void *fixture);

/* EXPORTED SUITE */

static MunitTest tests_print_config[] = {
	{
		"",
		test_print_config,
		NULL,
		NULL,
		MUNIT_TEST_OPTION_NONE,
		NULL
	},
	{ NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static MunitSuite config_utils_print_config_suite = {
	"/print_config",
	tests_print_config,
	NULL,
	1,
	MUNIT_SUITE_OPTION_NONE
};

MunitSuite *get_config_utils_print_config_suite() {
	return &config_utils_print_config_suite;
}

/* HELPER FUNCTIONS DEFINITIONS */

static MunitResult test_print_config(const MunitParameter params[], void *fixture) {
	config_t *config = newConfig();
	redirect_config_t redirect_config = init_redirect_config();
	redirect_stdout(&redirect_config);
	printConfig(config);
	recover_stdout(&redirect_config);
	munit_assert_not_null(config);
	deleteConfig(config);
	return MUNIT_OK;
}
