#ifndef TELEMETRY_TEST_CONFIG_UTILS
#define TELEMETRY_TEST_CONFIG_UTILS

/* IMPORTS */

#include "../../../libs/munit/munit.h"
#include "./new_config_and_delete_config/new_config_and_delete_config.test.h"
#include "./parse_config_file/parse_config_file.test.h"

/* EXPORTED SUITE */

MunitSuite* get_config_utils_suite();

#endif
