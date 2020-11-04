#include "log_service.test.h"

/* EXPORTED SUITE */

MunitSuite* get_log_service_suite() {
  MunitSuite* sub_suites = (MunitSuite*) malloc(7 * sizeof(MunitSuite));
  sub_suites[0] = *get_log_service_debug_config_path_suite();
  sub_suites[1] = *get_log_service_debug_gps_port_suite();
  sub_suites[2] = *get_log_service_debug_generic_suite();
  sub_suites[3] = *get_log_service_info_transition_suite();
  sub_suites[4] = *get_log_service_info_starting_up_suite();
  sub_suites[5] = *get_log_service_info_gps_plugged_suite();
  sub_suites[6] = (MunitSuite) { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL };

  MunitSuite* suite = (MunitSuite*) malloc(sizeof(MunitSuite));
  suite->prefix = "/log-service";
  suite->tests = NULL;
  suite->suites = sub_suites;
  suite->iterations = 1;
  suite->options = MUNIT_SUITE_OPTION_NONE;

  return suite;
}