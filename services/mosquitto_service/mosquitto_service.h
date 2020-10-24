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

/**
 * The enum type that describes a result for a mosquitto operation
*/
typedef enum {
    MOSQUITTO_OK,
    MOSQUITTO_CREATION_ERROR,
    MOSQUITTO_CONNECTION_ERROR,
    MOSQUITTO_PUBLISH_ERROR,
    MOSQUITTO_LOG_ERROR
} mosquitto_code;

/* FUNCTIONS */

/**
 * Setups mosquitto, creating a mosquitto instance with the connection options in the condition and adding them to the condition itself.
 * @* @return The result of the mosquitto operation.
*/
mosquitto_code mosquittoSetup();
/**
 * Sends a messago via mosquitto. The passed message must be a bson document and it will be sent to the topic in config->mqtt.data_topic
 * @param message The bson document that will be sent
 * @return The result of the mosquitto operation.
*/
mosquitto_code mosquittoSend(const bson_t* message);
/**
 * Sends a messago via mosquitto. The passed message must be a string and it will be sent to the topic in config->mqtt.log_topic
 * @param message The log message as a string
 * @return The result of the mosquitto operation.
*/
mosquitto_code mosquittoLog(const char* message);
/**
 * Quits mosquitto deallocating and destroying the instance
*/
void mosquittoQuit();

/**
 * It is the equivalent of "successStartedUp", but the message will be sent via mqtt on the log topic, using mosquittoLog internally
 * @return The result of the mosquitto operation.
*/
mosquitto_code mosquittoLogStartup();
/**
 * It is the equivalent of "infoTransition", but the message will be sent via mqtt on the log topic, using mosquittoLog internally
 * @param from The state before the transition
 * @param to The state after the transition
 * @param labels The array of strings containing the labels of the states
 * @return The result of the mosquitto operation.
*/
mosquitto_code mosquittoLogTransition(int from, int to, char** labels);
/**
 * It is the equivalent of "infoNewSession", but the message will be sent via mqtt on the log topic, using mosquittoLog internally
 * @return The result of the mosquitto operation.
*/
mosquitto_code mosquittoLogSession();
/**
 * It is the equivalent of "successInsertion", but the message will be sent via mqtt on the log topic, using mosquittoLog internally
 * @param size The size of the document inserted
 * @return The result of the mosquitto operation.
*/
mosquitto_code mosquittoLogInsertion(int length);
/**
 * It is the equivalent of "infoQuitting", but the message will be sent via mqtt on the log topic, using mosquittoLog internally
 * @return The result of the mosquitto operation.
*/
mosquitto_code mosquittoLogQuit();

/**
 * Given a mosquitto code, returns a message
 * @param code The code that will define the returned message
 * @return The returned message
*/
char* mosquittoErrorMessage(mosquitto_code code);

#endif