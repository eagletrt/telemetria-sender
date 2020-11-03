#ifndef TELEMETRY_TEST_SERVICES.TEST
#define TELEMETRY_TEST_SERVICES.TEST

/* IMPORTS */

#include "../../libs/munit/munit.h"
#include "../../../../state_machine/state_machine_condition.h"
#include "./config_service/config_service.test.h"

/* EXPORTED SUITE */

MunitSuite* get_services_suite();

#endif
