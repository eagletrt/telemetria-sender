#ifndef TELEMETRY_TEST_LOG_SERVICE_ERROR_GENERIC
#define TELEMETRY_TEST_LOG_SERVICE_ERROR_GENERIC

/** IMPORTS **/

#include "../../../../../../services/log_service/log_service.h"
#include "../../../../../../state_machine/state_machine_condition.h"
#include "../../../../libs/munit/munit.h"
#include "../../../../libs/test_utils/test_utils.h"

/* EXTERN */

extern condition_t condition;

/* EXPORTED TESTS */

MunitSuite* get_log_service_error_generic_suite();

#endif