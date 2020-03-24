#ifndef TELEMETRY_MONGO_SERVICE
#define TELEMETRY_MONGO_SERVICE

/* IMPORTS */

// Let this shit to use asprintf
#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <bson.h>
#include <mongoc.h>
#include "../../utils/misc_utils/misc_utils.h"
#include "../../utils/log_utils/log_utils.h"
#include "../../state_machine/state_machine_condition.h"

/* EXTERN */

extern condition_t condition;

/* TYPES */

typedef enum {
    MONGO_OK,
    MONGO_SETUP_ERROR,
    MONGO_INSERT_ERROR
} mongo_code;

/* FUNCTIONS */

mongo_code mongoSetup();
mongo_code mongoStartSession();
mongo_code mongoInsert(bson_t* data);
void mongoQuit();
char* mongoErrorMessage(mongo_code code);

#endif