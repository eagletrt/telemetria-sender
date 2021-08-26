#ifndef TELEMETRY_CAN_SERVICE
#define TELEMETRY_CAN_SERVICE

/* IMPORTS */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../utils/can_utils/can_utils.h"
#include "../../state_machine/state_machine_condition.h"
#include "../../utils/can-cicd/includes_generator/Primary/ids.h"
#include "../../utils/can-cicd/naked_generator/Primary/c/Primary.h"

/* EXTERN */

extern condition_t condition;

/* TYPES */

/**
 * The enum containing all the can service error codes 
*/ 
typedef enum { 
    CAN_SERVICE_OK,
    CAN_SERVICE_CONNECTION_ERROR,
    CAN_SERVICE_BINDING_ERROR,
    CAN_SERVICE_SENDING_ERROR
} can_code;

/* FUNCTIONS */

/**
 *  Opens the primary can socket, by looking and updating the condition
 *  @return The outcome of the function as a can_code
*/ 
can_code canSetupPrimary();

/**
 *  Opens the secondary can socket, by looking and updating the condition
 *  @return The outcome of the function as a can_code
*/ 
can_code canSetupSecondary();

/**
 * Reads data from the primary can
 * @param id The id of the read message will be saved here
 * @param data The array of eight bytes that will be filled
 * @return The result of the operation
*/
int canReadPrimary(int *id, char** data);

/**
 * Reads data from the secondary can
 * @param id The id of the read message will be saved here
 * @param data The array of eight bytes that will be filled
 * @return The result of the operation
*/
int canReadSecondary(int *id, char** data);

/**
 * Sends a can message to the volante to answer when it receives a enable or idle message
 * @param enabled 0 if the telemetry is not enabled, 1 if it is enabled
 * @return The outcome of the function as a can_code
*/
can_code canAnswerWheel(int enabled);

/**
 * Sends a can message to broadcast the current timestamp
 * @param timestamp The current timestamp
 * @return The outcome of the function as a can_code
*/
can_code canSendTimestamp(long timestamp);

/**
 * Given a can_code, it returns the message corrisponding that error code
 * @param code The error code as a can_code
 * @return The message corrisponding the given code 
*/
char* canErrorMessage(can_code code);

#endif