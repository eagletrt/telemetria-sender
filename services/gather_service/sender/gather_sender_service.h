#ifndef TELEMETRY_GATHER_SENDER_SERVICE
#define TELEMETRY_GATHER_SENDER_SERVICE

/* IMPORTS */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "../../../utils/gather_utils/gather_utils.h"
#include "../../../services/log_service/log_service.h"
#include "../../../services/mosquitto_service/mosquitto_service.h"
#include "../../../services/mongo_service/mongo_service.h"
#include "../../../state_machine/state_machine_condition.h"

/* EXTERN */

extern condition_t condition;

/* FUNCTIONS */

void gatherSenderStartThread();
void gatherSenderStopThread();

#endif