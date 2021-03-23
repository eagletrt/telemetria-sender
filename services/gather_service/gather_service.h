#ifndef TELEMETRY_GATHER_SERVICE
#define TELEMETRY_GATHER_SERVICE

/* IMPORTS */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/timeb.h>
#include <sys/sysinfo.h>
#include <bson.h>
#include <pthread.h>
#include "../../utils/gather_utils/gather_utils.h"
#include "../../state_machine/state_machine_condition.h"

#include "./can/gather_can_primary_service.h"
#include "./can/gather_can_secondary_service.h"
#include "./gps/gather_gps_service.h"
#include "./master/gather_master_service.h"
#include "./sender/gather_sender_service.h"

/* EXTERN */

extern condition_t condition;

/* FUNCTIONS */

/**
 * Initializes the structure object of the condition
*/
void gatherSetup();

/**
 * When there is a change between idle and enabled, reinitializes the structure object of the condition
 */
void gatherSetupRestart();

/**
 * Resets the id of the data_head structure
*/
void gatherResetDataId();

#endif