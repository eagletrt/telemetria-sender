#ifndef TELEMETRY_TEST_CONFIG_services_DELETE_CONFIG
#define TELEMETRY_TEST_CONFIG_services_DELETE_CONFIG

/** IMPORTS **/
#include "../../../../../../services/config_service/config_service.h"
#include "../../../../libs/munit/munit.h"

/* EXPORTED TESTS */
extern condition_t condition;
MunitSuite* get_config_services_handle_config_suite();

#endif