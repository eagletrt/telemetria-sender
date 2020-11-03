#ifndef TELEMETRY_TEST_LOG_SERVICE_DEBUG_GENERIC
#define TELEMETRY_TEST_LOG_SERVICE_DEBUG_GENERIC

/** IMPORTS **/

#include "../../../../../../services/log_service/log_service.h"
#include "../../../../../../state_machine/state_machine_condition.h"
#include "../../../../libs/munit/munit.h"
#include "../../../../libs/test_utils/test_utils.h"

/* EXTERN */

extern condition_t condition;

/* EXPORTED TESTS */

MunitSuite* get_log_service_debug_generic_suite();

#endif