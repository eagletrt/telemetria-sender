#ifndef TELEMETRY_CONFIG_UTILS
#define TELEMETRY_CONFIG_UTILS

/* IMPORTS */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../misc_utils/misc_utils.h"
#include "./jsmn/jsmn.h"

/* STRUCTURES */

// {{GENERATE_CONFIG_TYPE}}

/* FUNCTIONS */

/**
 * Allocates an empty config instance
 * 
 * @return The config instance
*/
config_t* newConfig();

/**
 * Parses the config file
 * 
 * @param path The path of the config file
 * @param config The reference to the config instance
*/
void parseConfigFile(const char* path, config_t **config);

/**
 * Deallocates the given config instance
 * 
 * @param config The reference to the config instance
*/
void deleteConfig(config_t *config);

/**
 * Prints a config instances
 * 
 * @param config The reference to the config instance
*/
void printConfig(const config_t* config);

#endif