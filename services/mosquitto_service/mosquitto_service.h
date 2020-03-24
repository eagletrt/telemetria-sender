#ifndef TELEMETRY_MOSQUITTO_SERVICE
#define TELEMETRY_MOSQUITTO_SERVICE

/* IMPORTS */

// Let this shit to use asprintf
#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <bson.h>
#include <mosquitto.h>
#include "../../utils/log_utils/log_utils.h"
#include "../../state_machine/state_machine_condition.h"

/* EXTERN */

extern condition_t condition;

/* TYPES */

typedef enum {
    MOSQUITTO_OK,
    MOSQUITTO_CREATION_ERROR,
    MOSQUITTO_CONNECTION_ERROR,
    MOSQUITTO_PUBLISH_ERROR,
    MOSQUITTO_LOG_ERROR
} mosquitto_code;

/* FUNCTIONS */

mosquitto_code mosquittoSetup();
mosquitto_code mosquittoSend(const bson_t* message);
mosquitto_code mosquittoLog(const char* message);
void mosquittoQuit();

mosquitto_code mosquittoLogStartup();
mosquitto_code mosquittoLogTransition(int from, int to, char** labels);
mosquitto_code mosquittoLogSession();
mosquitto_code mosquittoLogInsertion(int length);
mosquitto_code mosquittoLogQuit();
char* mosquittoErrorMessage(mosquitto_code code);

#endif