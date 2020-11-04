#ifndef TELEMETRY_TEST_SERVICES_TEST
#define TELEMETRY_TEST_SERVICES_TEST

/* IMPORTS */

#include "../../libs/munit/munit.h"
#include "./log_service/log_service.test.h"
#include "./config_service/config_service.test.h"
#include "./mongo_service/mongo_service.test.h"

/* EXPORTED SUITE */

MunitSuite* get_services_suite();

#endif
