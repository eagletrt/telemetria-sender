#ifndef TELEMETRY_LOG_UTILS
#define TELEMETRY_LOG_UTILS

/* IMPORTS */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "log_utils_colors.h"

/* FUNCTIONS */

/**
 * Prints an info log, preceded by a blue [INFO] tag.
 * @param message The message that will be printed.
*/
void logInfo(const char* message);
/**
 * Prints a success log, preceded by a green [SUCCESS] tag.
 * @param message The message that will be printed.
*/
void logSuccess(const char* message);
/**
 * Prints a debug log, preceded by a gray [DEBUG] tag.
 * @param message The message that will be printed.
*/
void logDebug(const char* message);
/**
 * Prints an error log, preceded by a red [ERROR] tag.
 * @param message The message that will be printed.
*/
void logError(const char* message);
/**
 * Prints a warning log, preceded by a yellow [WARNING] tag.
 * @param message The message that will be printed.
*/
void logWarning(const char* message);

#endif
