#ifndef TELEMETRY_GATHER_GPS_SERVICE
#define TELEMETRY_GATHER_GPS_SERVICE

/* IMPORTS */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include "../../../services/log_service/log_service.h"
#include "../../../utils/gather_utils/gather_utils.h"
#include "../../../utils/misc_utils/misc_utils.h"
#include "../../../state_machine/state_machine_condition.h"
#include "../../gps_service/gps_service.h"
#include "../../mongo_service/mongo_service.h"

/* EXTERN */

extern condition_t condition;

/* FUNCTIONS */

void gatherGpsStartThread();
void gatherGpsStopThread();

#endif