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

void debugConfigPath();
void debugGpsPort();

void infoTransition(int from, int to, char** labels);
void infoStartingUp();
void infoGpsPlugged();
void infoNewSession();
void infoQuitting();

void successStartedUp();
void successInsertion(int length);
void successQuitting();

void errorParsingConfig();
void errorGatheringData();
void errorOpeningGPS();
void errorGeneric(char* message);

void debugGeneric(char* message);

#endif