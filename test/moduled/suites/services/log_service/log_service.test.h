#ifndef TELEMETRY_TEST_LOG_SERVICE
#define TELEMETRY_TEST_LOG_SERVICE

/** IMPORTS **/

#include "../../../libs/munit/munit.h"
#include "./debug_config_path/debug_config_path.test.h"
#include "./debug_gps_port/debug_gps_port.test.h"

/* EXPORTED SUITE */

MunitSuite* get_log_service_suite();

#endif