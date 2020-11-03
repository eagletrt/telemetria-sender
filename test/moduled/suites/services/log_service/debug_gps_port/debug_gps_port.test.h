#ifndef TELEMETRY_TEST_LOG_SERVICE_DEBUG_GPS_PORT
#define TELEMETRY_TEST_LOG_SERVICE_DEBUG_GPS_PORT

/** IMPORTS **/

#include "../../../../../../services/log_service/log_service.h"
#include "../../../../../../state_machine/state_machine_condition.h"
#include "../../../../libs/munit/munit.h"
#include "../../../../libs/test_utils/test_utils.h"
#include "./assets/debug_gps_port.assets.h"

/* EXTERN */

extern condition_t condition;

/* EXPORTED TESTS */

MunitSuite* get_log_service_debug_gps_port_suite();

#endif