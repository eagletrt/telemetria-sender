#ifndef TELEMETRY_TEST_LOG_SERVICE_DEBUG_CONFIG_PATH
#define TELEMETRY_TEST_LOG_SERVICE_DEBUG_CONFIG_PATH

/** IMPORTS **/

#include "../../../../../../services/log_service/log_service.h"
#include "../../../../../../state_machine/state_machine_condition.h"
#include "../../../../libs/munit/munit.h"
#include "../../../../libs/test_utils/test_utils.h"
#include "./assets/debug_config_path.assets.h"

/* EXTERN */

extern condition_t condition;

/* EXPORTED TESTS */

MunitSuite* get_log_service_debug_config_path_suite();

#endif