#ifndef TELEMETRY_LOG_SERVICE
#define TELEMETRY_LOG_SERVICE

/* IMPORTS */

// Let this shit to use asprintf
#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../utils/log_utils/log_utils.h"
#include "../../state_machine/state_machine_condition.h"

/* EXTERN */

extern condition_t condition;

/* FUNCTIONS */

/**
 * Prints the debug log showing the config path
*/
void debugConfigPath();
/**
 * Prints the debug log showing the gps port
*/
void debugGpsPort();
/**
 * Prints the debug log for a generic message
 * @param message The string to log
*/
void debugGeneric(char* message);

/**
 * Prints the info log showing a state transition
 * @param from The state before the transition
 * @param to The state after the transition
 * @param labels The array of strings containing the labels of the states
*/
void infoTransition(int from, int to, char** labels);
/**
 * Prints the info log for the starting up telemetry
*/
void infoStartingUp();
/**
 * Prints the info log for the gps plugged
*/
void infoGpsPlugged();
/**
 * Prints the info log for the new session created
*/
void infoNewSession();
/**
 * Prints the info log for the quitting telemetry 
*/
void infoQuitting();

/**
 * Prints the success log for the started up telemetry
*/
void successStartedUp();
/**
 * Prints the success log for the insertion to mongodb
 * @param size The size of the document inserted
*/
void successInsertion(int size);
/**
 * Prints the success log for the quitting telemetry
*/
void successQuitting();

/**
 * Prints the error log for the parsing telemetry
*/
void errorParsingConfig();
/**
 * Prints the error log for the gathering data
*/
void errorGatheringData();
/**
 * Prints the error log for the opening GPS
*/
void errorOpeningGPS();
/**
 * Prints the error log for a generic error message
 * @param message The string to log (it will be also deallocated)
*/
void errorGeneric(char* message);

#endif