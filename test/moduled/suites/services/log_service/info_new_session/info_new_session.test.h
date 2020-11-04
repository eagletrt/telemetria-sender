#ifndef TELEMETRY_TEST_LOG_SERVICE_INFO_NEW_SESSION
#define TELEMETRY_TEST_LOG_SERVICE_INFO_NEW_SESSION

/** IMPORTS **/

#include "../../../../../../services/log_service/log_service.h"
#include "../../../../../../state_machine/state_machine_condition.h"
#include "../../../../libs/munit/munit.h"
#include "../../../../libs/test_utils/test_utils.h"
#include "./assets/info_new_session.assets.h"

/* EXTERN */

extern condition_t condition;

/* EXPORTED TESTS */

MunitSuite* get_log_service_info_new_session_suite();

#endif