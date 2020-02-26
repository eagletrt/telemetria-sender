#ifndef TELEMETRY_STRUCTURE_SERVICE
#define TELEMETRY_STRUCTURE_SERVICE

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
#include "../../state_machine/state_machine_condition.h"
#include "../can_service/can_service.h"
#include "../log_service/log_service.h"
#include "../gps_service/gps_service.h"
#include "structure_identifiers.h"

/* EXTERN */

extern condition_t condition;

/* TYPES */

// {{GENERATE_STRUCTURE_CODE}}

typedef enum { 
    GATHER_IDLE, 
    GATHER_ENABLE, 
    GATHER_KEEP, 
    GATHER_ERROR 
} gather_code;

/* FUNCTIONS */

data_t* structureCreate();
void structureToBson(data_t *document, bson_t **bson_document);
void structureDelete(data_t *document);
gather_code gatherStructure(data_t* document);
void resetStructureId();

#endif