#ifndef TELEMETRY_STATE_MACHINE_CONDITION_SERVICE
#define TELEMETRY_STATE_MACHINE_CONDITION_SERVICE

/* IMPORTS */

#include <pthread.h>
#include <mosquitto.h>
#include <mongoc.h>
#include "../utils/gather_utils/data_structure.h"

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
	char* session_name;
} mongodb_condition_t;

typedef struct {
    char* can_interface;
    int socket;
} can_condition_t;

typedef struct {
    char* interface;
    int plugged;
    int simulated;
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
    pthread_mutex_t data_head_mutex;
    pthread_mutex_t data_tail_mutex;
    pthread_mutex_t flush_toilet_mutex;
    pthread_mutex_t toilet_flushed_mutex;
    pthread_mutex_t toggle_state_mutex;
    pthread_mutex_t restarting_mutex;

    pthread_cond_t flush_toilet_cond;
    pthread_cond_t toilet_flushed_cond;
} structure_threads_condition_t;

typedef struct {
    char* model_version;
    int sending_rate;
    int id;
    data_t* data_head;
    data_t* data_tail;
    int flush_toilet;
    int toilet_flushed;
    int toggle_state;
    int enabled;
    int restarting;

    structure_threads_condition_t threads;
} structure_condition_t;

/**
 * The struct of the condition, the only global variable of the project and that defines the status of the finite-state-machine. 
*/
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