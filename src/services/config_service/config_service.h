#ifndef TELEMETRY_CONFIG_SERVICE
#define TELEMETRY_CONFIG_SERVICE

/* IMPORTS */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../../utils/config_utils/config_utils.h"
#include "../../state_machine/state_machine_condition.h"

/* EXTERN */

extern condition_t condition;

/* TYPES */

typedef enum {
    CONFIG_OK,
    CONFIG_ERROR
} config_code;

/* FUNCTIONS */

config_code handleConfig();

#endif