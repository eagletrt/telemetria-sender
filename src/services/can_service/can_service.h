#ifndef TELEMETRY_CAN_SERVICE
#define TELEMETRY_CAN_SERVICE

/* IMPORTS */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../utils/can_utils/can_utils.h"
#include "../../state_machine/state_machine_condition.h"

/* EXTERN */

extern condition_t condition;

/* TYPES */

typedef enum { 
    CAN_SERVICE_OK,
    CAN_SERVICE_CONNECTION_ERROR,
    CAN_SERVICE_BINDING_ERROR,
    CAN_SERVICE_SENDING_ERROR
} can_code;

/* FUNCTIONS */

can_code canSetup();
int canRead(int *id, int *data_left, int *data_right);
can_code canAnswerWheel(int enabled);
char* canErrorMessage(can_code code);

#endif