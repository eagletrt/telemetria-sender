#ifndef TELEMETRY_TEST_LOG_SERVICE_SUCCESS_INSERTION
#define TELEMETRY_TEST_LOG_SERVICE_SUCCESS_INSERTION

/** IMPORTS **/

#include "../../../../../../services/log_service/log_service.h"
#include "../../../../../../state_machine/state_machine_condition.h"
#include "../../../../libs/munit/munit.h"
#include "../../../../libs/test_utils/test_utils.h"

/* EXPORTED TESTS */

MunitSuite* get_log_service_success_insertion_suite();

#endif