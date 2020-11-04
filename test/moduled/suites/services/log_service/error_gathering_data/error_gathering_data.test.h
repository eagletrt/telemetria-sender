#ifndef TELEMETRY_TEST_LOG_SERVICE_ERROR_GATHERING_DATA
#define TELEMETRY_TEST_LOG_SERVICE_ERROR_GATHERING_DATA

/** IMPORTS **/

#include "../../../../../../services/log_service/log_service.h"
#include "../../../../../../state_machine/state_machine_condition.h"
#include "../../../../libs/munit/munit.h"
#include "../../../../libs/test_utils/test_utils.h"

/* EXPORTED TESTS */

MunitSuite* get_log_service_error_gathering_data_suite();

#endif