#include <stdio.h>
#include <string.h>
#include <time.h>
#include "mongo_custom.h"

dbhandler_t* mongo_setup(int port, char* host, char* database, char* collection) {
	dbhandler_t* toRtn;
	char* uri_string;

	if (verbose) printf("Mongo setup procedure started.\n");
	
	//lenght of the port
	char* port_string = (char*) malloc(sizeof(char)*6);
	for (int i = 4; i >= 0; --i) {
		port_string[i] = ((char)(port%10)) + '0';
		port /=10;
	}
	port_string[5] = '\0';
	while (port_string[0] == '0') {
		for (int i = 0; i <= 4; ++i)
			port_string[i] = port_string[i+1];
		port_string[5] = '\0';
	}

	//lenght of the hostname, lenght of the port + 10 for "mongodb://" + 1 for ":" + 1 for "\0"
	int lenght = strlen(host) + strlen(port_string) + 12;
	uri_string = (char *) malloc(sizeof(char) * lenght);

	strcpy(uri_string, "mongodb://");
	strcat(uri_string, host);
	strcat(uri_string, ":");
	strcat(uri_string, port_string);
	
	if (verbose) printf("Mongo uri designed: %s\n", uri_string);
   	
	toRtn = (dbhandler_t*) malloc(sizeof(dbhandler_t));

   	bson_error_t error;
	toRtn->uri = mongoc_uri_new_with_error(uri_string, &error);
   	if (!toRtn->uri) {
		if (verbose) printf("Establishing a connection to \"%s\" failed.\nError message: %s.\n", uri_string, error.message);
		return NULL;
	}

	toRtn->client = mongoc_client_new_from_uri(toRtn->uri);
	if (!toRtn->client) {
	    if (verbose) printf("Client process startup failed.\n");
		return NULL;
	}

	mongoc_client_set_appname (toRtn->client, "E-agle racing team - Telemetria");

	char date[100];
	time_t now = time(NULL);
	struct tm *t = localtime(&now);
	strftime(date,99,"%y%m_%H%M",t);
	//printf("%s\n", date);

	int size = strlen(collection) + strlen(date);
	char* collection_name = (char*) malloc(sizeof(char)*size);
	strcpy(collection_name, collection);
	strcat(collection_name, date);

	toRtn->database = mongoc_client_get_database(toRtn->client, database);
  	toRtn->collection = mongoc_client_get_collection(toRtn->client, database, collection_name);
	
	if (verbose) printf("Mongo started up with success.\n\n");

 	return toRtn;}

int mongo_insert(bson_t *insert, dbhandler_t* handler) {
	bson_error_t error;

	if (!mongoc_collection_insert_one (handler->collection, insert, NULL, NULL, &error)) {
		if (verbose) printf ( "%s\n", error.message);
    return 1;
	}
	if (verbose) printf("The doc was successfully inserted inside the designed collection.\n");

  	return 0;}

int mongo_quit(dbhandler_t* handler) {
	mongoc_collection_destroy(handler->collection);
   	mongoc_database_destroy(handler->database);
   	mongoc_client_destroy(handler->client);
   	mongoc_uri_destroy(handler->uri);
   	mongoc_cleanup ();

   	free(handler);
   	return 0;}