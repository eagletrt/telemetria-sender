/*
 * mongo_custom_lib.h
 *
 * *          
 * This is a lib done by and for the FSAE Eagle Racing Team, from the University of Trento.
 * Everything is built on top of the libmongoc. 
 * 
 * It does contain a set of procedures useful for the Telemetry solution of the E-agle Racing Team.
 * 
 */

#ifndef MONGO_CUSTOM_LIB_H
#define MONGO_CUSTOM_LIB_H

//basic import
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <bson.h>
#include <mongoc.h>

extern int verbose;

typedef struct {
	mongoc_uri_t *uri;
  	mongoc_client_t *client;
  	mongoc_database_t *database;
  	mongoc_collection_t *collection;
} dbhandler_t;

dbhandler_t* mongo_setup(int port, char* host, char* database);
int mongo_insert(bson_t* insert, dbhandler_t* handler);
int mongo_set_collection(dbhandler_t* handler, char* driver, char* type, int timestamp);
int mongo_quit(dbhandler_t* handler);

#endif