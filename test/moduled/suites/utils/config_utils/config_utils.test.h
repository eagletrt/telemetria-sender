#ifndef TELEMETRY_TEST_CONFIG_UTILS
#define TELEMETRY_TEST_CONFIG_UTILS

/* IMPORTS */

#include "../../../libs/munit/munit.h"
#include "./parse_config_file/parse_config_file.test.h"
#include "./delete_config/delete_config.test.h"
#include "./new_config/new_config.test.h"
#include "./print_config/print_config.test.h"

/* EXPORTED SUITE */

MunitSuite* get_config_utils_suite();

#endif
