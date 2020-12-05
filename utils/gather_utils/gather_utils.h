#ifndef TELEMETRY_GATHER_UTILS
#define TELEMETRY_GATHER_UTILS

/* IMPORTS */

#include <mongoc.h>
#include <time.h>
#include <sys/timeb.h>
#include <sys/sysinfo.h>
#include "data_structure.h"

/* FUNCTIONS */

/**
 * Creates and returns an empty data structure, that will contain all the messages gathered in a certain amount of 
 * time and translated to a bson document
 * @return The empty data structure
*/
data_t* gatherCreateData();

/**
 * Deallocates the passed data structure
 * @param document The data structure to deallocate 
*/
void gatherDeleteData(data_t *document);

/**
 * Converts the data structure to a bson document.
 * @param data The reference to the data structure to convert in bson
 * @param bson_document The reference to the bson document pointer that will contain the converted document.
*/
void gatherDataToBson(data_t *data, bson_t** bson_document);

#endif
