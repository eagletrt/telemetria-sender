#ifndef TELEMETRY_STATE_MACHINE
#define TELEMETRY_STATE_MACHINE

/* IMPORTS */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../services/log_service/log_service.h"
#include "../services/can_service/can_service.h"
#include "../services/config_service/config_service.h"
#include "../services/gps_service/gps_service.h"
#include "../services/mongo_service/mongo_service.h"
#include "../services/mosquitto_service/mosquitto_service.h"
#include "../services/gather_service/gather_service.h"
#include "state_machine_condition.h"

/* EXTERN */

extern condition_t condition;

/* MACHINE */
typedef enum { INIT, RESTART, IDLE, ENABLED, EXIT } state_codes;
typedef enum { ERROR, INITIALIZED, REPEAT, TOGGLE, DISABLE, ENABLE } result_codes;

result_codes init_state();
result_codes restart_state();
result_codes idle_state();
result_codes enabled_state();
result_codes exit_state();

extern result_codes (*state[])();

typedef struct {
    state_codes from_state;
    result_codes result_code;
    state_codes to_state;
} transition_t;

extern transition_t state_transitions[];

state_codes lookup_transitions(state_codes current_state, result_codes result_code);

#endif