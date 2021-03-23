#ifndef TELEMETRY_GATHER_CAN_SECONDARY_SERVICE
#define TELEMETRY_GATHER_CAN_SECONDARY_SERVICE

/* IMPORTS */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include "../../../services/log_service/log_service.h"
#include "../../../utils/gather_utils/gather_utils.h"
#include "../../../utils/misc_utils/misc_utils.h"
#include "../../../utils/can-cicd/flatbuf-generator/Secondary/c/schema_reader.h"
#include "../../../utils/can-cicd/includes-generator/Secondary/ids.h"
#include "../../../state_machine/state_machine_condition.h"
#include "../../can_service/can_service.h"

/* EXTERN */

extern condition_t condition;

/* FUNCTIONS */

void gatherCanSecondaryStartThread(int enabled);
void gatherCanSecondaryStopThread();

#endif