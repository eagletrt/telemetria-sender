#ifndef TELEMETRY_TEST_LOG_SERVICE_INFO_NEW_SESSION_ASSETS
#define TELEMETRY_TEST_LOG_SERVICE_INFO_NEW_SESSION_ASSETS

/** IMPORTS **/

#include <mongoc.h>
#include "../../../../../../../state_machine/state_machine_condition.h"

/* EXPORTED ASSETS */

extern mongodb_instance_t log_service_info_new_session_assets_mongodb_instances[3];
extern condition_t log_service_info_new_session_assets_conditions[3];
extern char* log_service_info_new_session_assets_results[3];

#endif