#ifndef TELEMETRY_TEST_LOG_SERVICE_ERROR_PARSING_CONFIG
#define TELEMETRY_TEST_LOG_SERVICE_ERROR_PARSING_CONFIG

/** IMPORTS **/

#include "../../../../../../services/log_service/log_service.h"
#include "../../../../../../state_machine/state_machine_condition.h"
#include "../../../../libs/munit/munit.h"
#include "../../../../libs/test_utils/test_utils.h"
#include "./assets/error_parsing_config.assets.h"

/* EXTERN */

extern condition_t condition;

/* EXPORTED TESTS */

MunitSuite* get_log_service_error_parsing_config_suite();

#endif