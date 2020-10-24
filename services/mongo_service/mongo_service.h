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

/**
 * The enum type that describes a result for a mongo operation
*/
typedef enum {
    MONGO_OK,
    MONGO_SETUP_ERROR,
    MONGO_INSERT_ERROR
} mongo_code;

/* FUNCTIONS */

/**
 * Setups mongodb, creating a mongo instance with the connection options in the condition and adding them to the condition itself.
 * @return MONGO_SETUP_ERROR if there were an error, MONGO_OK otherwise
*/
mongo_code mongoSetup();
/**
 * Starts a session, by inserting a session document
 * @return MONGO_INSERT_ERROR if there were an error, MONGO_OK otherwise
*/
mongo_code mongoStartSession();
/**
 * Inserts a document to mongodb
 * @param data The bson document to insert
 * @return MONGO_INSERT_ERROR if there were an error, MONGO_OK otherwise
*/
mongo_code mongoInsert(bson_t* data);
/**
 * Quits mongodb, destroying the mongo instance
*/
void mongoQuit();
/**
 * Given a mongo code, returns a message
 * @param code The code that will define the returned message
 * @return The returned message
*/
char* mongoErrorMessage(mongo_code code);

#endif