#ifndef TELEMETRY_TEST_CONFIG_UTILS_PARSE_CONFIG_FILE
#define TELEMETRY_TEST_CONFIG_UTILS_PARSE_CONFIG_FILE

/** IMPORTS **/

// Let this shit to be able to use asprintf
#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif

#include "../../../../../../utils/config_utils/config_utils.h"
#include "../../../../libs/munit/munit.h"


/* EXPORTED TESTS */

MunitSuite* get_config_utils_parse_config_file_suite();

#endif