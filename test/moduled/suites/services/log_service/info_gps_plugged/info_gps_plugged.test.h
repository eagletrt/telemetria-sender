#ifndef TELEMETRY_TEST_LOG_SERVICE_INFO_GPS_PLUGGED
#define TELEMETRY_TEST_LOG_SERVICE_INFO_GPS_PLUGGED

/** IMPORTS **/

#include "../../../../../../services/log_service/log_service.h"
#include "../../../../../../state_machine/state_machine_condition.h"
#include "../../../../libs/munit/munit.h"
#include "../../../../libs/test_utils/test_utils.h"

/* EXPORTED TESTS */

MunitSuite* get_log_service_info_gps_plugged_suite();

#endif