#ifndef TELEMETRY_TEST_CONFIG_SERVICES_MONGO_SETUP
#define TELEMETRY_TEST_CONFIG_SERVICES_MONGO_SETUP

/** IMPORTS **/

#include "../../../../../../services/mongo_service/mongo_service.h"
#include "../../../../libs/munit/munit.h"

/* EXPORTED TESTS */
extern condition_t condition;
MunitSuite* get_mongo_services_mongo_setup_suite();

#endif