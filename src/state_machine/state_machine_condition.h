#ifndef TELEMETRY_STATE_MACHINE_CONDITION_SERVICE
#define TELEMETRY_STATE_MACHINE_CONDITION_SERVICE

/* IMPORTS */

#include <mosquitto.h>
#include <mongoc.h>

/* STRUCTURES */

typedef struct mosquitto mqtt_instance_t;
typedef struct {
    char* host;
    int port;
    char* data_topic;
    char* log_topic;
    mqtt_instance_t *instance;
} mqtt_condition_t;

typedef struct {
	mongoc_uri_t *uri;
  	mongoc_client_t *client;
  	mongoc_database_t *database;
  	mongoc_collection_t *collection;
	char* session_name;
} mongodb_instance_t;

typedef struct {
    char* host;
    int port;
    char* db;
    char* collection;
    mongodb_instance_t *instance;
} mongodb_condition_t;

typedef struct {
    char* can_interface;
    int socket;
} can_condition_t;

typedef struct {
    char* interface;
    int plugged;
    int port;
} gps_condition_t;

typedef struct {
    char** pilots;
    int pilots_count;
    int selected_pilot;

    char** races;
    int races_count;
    int selected_race;
} session_condition_t;

typedef struct {
    int sending_rate;
    int id;
} structure_condition_t;

typedef struct {
    mqtt_condition_t mqtt;
    mongodb_condition_t mongodb;
    can_condition_t can;
    gps_condition_t gps;
    session_condition_t session;
    structure_condition_t structure;
    char* config_path;
    int verbose;
} condition_t;

#endif