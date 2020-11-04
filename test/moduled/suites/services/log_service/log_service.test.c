#include "log_service.test.h"

/* EXPORTED SUITE */

MunitSuite* get_log_service_suite() {
  MunitSuite* sub_suites = (MunitSuite*) malloc(13 * sizeof(MunitSuite));
  sub_suites[0] = *get_log_service_debug_config_path_suite();
  sub_suites[1] = *get_log_service_debug_gps_port_suite();
  sub_suites[2] = *get_log_service_debug_generic_suite();
  sub_suites[3] = *get_log_service_info_transition_suite();
  sub_suites[4] = *get_log_service_info_starting_up_suite();
  sub_suites[5] = *get_log_service_info_gps_plugged_suite();
  sub_suites[6] = *get_log_service_info_quitting_suite();
  sub_suites[7] = *get_log_service_info_new_session_suite();
  sub_suites[8] = *get_log_service_success_started_up_suite();
  sub_suites[9] = *get_log_service_success_insertion_suite();
  sub_suites[10] = *get_log_service_success_quitting_suite();
  sub_suites[11] = *get_log_service_error_parsing_config_suite();
  sub_suites[12] = (MunitSuite) { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL };

  MunitSuite* suite = (MunitSuite*) malloc(sizeof(MunitSuite));
  suite->prefix = "/log-service";
  suite->tests = NULL;
  suite->suites = sub_suites;
  suite->iterations = 1;
  suite->options = MUNIT_SUITE_OPTION_NONE;

  return suite;
}