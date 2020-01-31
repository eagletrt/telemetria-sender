#include <stdio.h>
#include <string.h>
#include <time.h>
#include "mongo_custom.h"

char* itostr(int value);
bson_t* sessionDocumentBson(char* driver, char* race, int timestamp, char* sessionId);

dbhandler_t* mongo_setup(int port, char* host, char* database, char* collection) {
	dbhandler_t* toRtn;
	char* uri_string;

	if (verbose) printf("Mongo setup procedure started.\n");
	
	//lenght of the port
	char* port_string = (char*) malloc(sizeof(char)*6);
	for (int i = 4; i >= 0; --i) {
		port_string[i] = ((char)(port%10)) + '0';
		port /= 10;
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


	toRtn->database = mongoc_client_get_database(toRtn->client, database);
	toRtn->collection = NULL;
  	toRtn->collection = mongoc_client_get_collection(toRtn->client, mongoc_database_get_name(toRtn->database), collection);
	
	char pilot[] = "deathlok"; 
	char type[] = "MarioCircuit"; 
	mongo_set_collection(toRtn,pilot,type, time(0));

	if (verbose) printf("Mongo started up with success.\n\n");

 	return toRtn;
}

int mongo_set_collection(dbhandler_t* handler, char* driver, char* type, int timestamp) {

	char* collection_name;
	collection_name = (char*) malloc(sizeof(char)* (strlen(driver) + strlen(type) + 14));
	
	if (driver[0] > 90) driver[0] -= 'a'-'A';
	strcpy(collection_name, driver);
	
	if (type[0] > 90) type[0] -= 'a'-'A';
	strcat(collection_name, type);
	strcat(collection_name, "_");

	time_t t = timestamp;
	struct tm *formatted_time = localtime(&t);
	
	//DAY
	int day =  formatted_time->tm_mday + (formatted_time->tm_mon+1) * 100 + (formatted_time->tm_year+1900) * 10000 ;
	strcat(collection_name, itostr(day));
	strcat(collection_name, "_");

	//TIME
	int hour =  (formatted_time->tm_sec) + (formatted_time->tm_min) * 100 + (formatted_time->tm_hour) *10000; 
	strcat(collection_name, itostr(hour));

	
	handler->session_name = collection_name;
	mongo_insert(sessionDocumentBson(driver, type, timestamp, collection_name), handler);

  	if (verbose) printf("Mongo collection created with success. All the values will be stored in %s\\%s\n",mongoc_database_get_name(handler->database),mongoc_collection_get_name(handler->collection));
  	return 0;}

int mongo_insert(bson_t *insert, dbhandler_t* handler) {
	bson_error_t error;

	if (!mongoc_collection_insert_one (handler->collection, insert, NULL, NULL, &error)) {
		if (verbose) printf ( "%s\n", error.message);
    return 1;
	}
	if (verbose) printf("BSON successfully inserted inside the designed collection.\n");

  	return 0;
}

bson_t* sessionDocumentBson(char* driver, char* race, int timestamp, char* sessionId) {
	bson_t* result = bson_new();
	BSON_APPEND_UTF8 (result, "sessionId", sessionId);
	BSON_APPEND_INT32(result, "timestamp", timestamp);
	BSON_APPEND_UTF8 (result, "driver", driver);
	BSON_APPEND_UTF8 (result, "race", race);
	return result;
}

int mongo_quit(dbhandler_t* handler) {
	mongoc_collection_destroy(handler->collection);
   	mongoc_database_destroy(handler->database);
   	mongoc_client_destroy(handler->client);
   	mongoc_uri_destroy(handler->uri);
   	mongoc_cleanup();

   	free(handler);
   	return 0;
}



char* itostr(int value) {
	char* toRtn = (char*) malloc(sizeof(char) * 15);
	int reverted = 0;

	int times = 0;
	while (value != 0) {
		reverted = (reverted*10) + value%10;
		value /= 10;
		times++;
	}

	int pos = 0;
	while (times-- > 0) {
		toRtn[pos++] = (reverted % 10) + '0';
		reverted /= 10;
	}

	toRtn[pos] = 0;
	return toRtn;
}
