#ifndef TELEMETRY_TEST_LOG_UTILS
#define TELEMETRY_TEST_LOG_UTILS

/** IMPORTS **/

#include "../../../libs/munit/munit.h"
#include "./log_info/log_info.test.h"
#include "./log_success/log_success.test.h"
#include "./log_debug/log_debug.test.h"
#include "./log_error/log_error.test.h"
#include "./log_warning/log_warning.test.h"

/* EXPORTED SUITE */

MunitSuite* get_log_utils_suite();

#endif