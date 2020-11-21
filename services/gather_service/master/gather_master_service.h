#ifndef TELEMETRY_GATHER_MASTER_SERVICE
#define TELEMETRY_GATHER_MASTER_SERVICE

/* IMPORTS */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <sys/timeb.h>
#include <sys/sysinfo.h>
#include "../../../services/log_service/log_service.h"
#include "../../../services/mongo_service/mongo_service.h"
#include "../../../services/mosquitto_service/mosquitto_service.h"
#include "../../../services/can_service/can_service.h"
#include "../../../utils/misc_utils/misc_utils.h"
#include "../../../utils/gather_utils/gather_utils.h"
#include "../../../state_machine/state_machine_condition.h"

/* EXTERN */

extern condition_t condition;

/* FUNCTIONS */

void gatherMasterWait();
void gatherMasterSwap();
void gatherMasterResetToiletFlushed();
void gatherMasterEnableFlushToilet();
void gatherMasterWaitToiletFlushed();
void gatherStartNewSession();
void gatherAnswerWheel();

#endif