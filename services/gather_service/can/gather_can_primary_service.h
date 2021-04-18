#ifndef TELEMETRY_GATHER_CAN_PRIMARY_SERVICE
#define TELEMETRY_GATHER_CAN_PRIMARY_SERVICE

/* IMPORTS */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include "../../../services/log_service/log_service.h"
#include "../../../utils/gather_utils/gather_utils.h"
#include "../../../utils/misc_utils/misc_utils.h"
#include "../../../utils/can-cicd/includes-generator/Primary/ids.h"
#include "../../../utils/can-cicd/naked-generator/Primary/c/Primary.h"
#include "../../../state_machine/state_machine_condition.h"
#include "../../can_service/can_service.h"

/* EXTERN */

extern condition_t condition;

/* FUNCTIONS */

void gatherCanPrimaryStartThread(int enabled);
void gatherCanPrimaryStopThread();

#endif