#ifndef TELEMETRY_LOG_UTILS
#define TELEMETRY_LOG_UTILS

/* IMPORTS */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "log_utils_colors.h"

/* FUNCTIONS */

void logInfo(const char* message);
void logSuccess(const char* message);
void logDebug(const char* message);
void logError(const char* message);
void logWarning(const char* message);

#endif
