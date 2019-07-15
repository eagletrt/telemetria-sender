/**********************************************************************
 * sub.c - MQTT subsctriber and MongoDB client                        *
 **********************************************************************/
#include "../utils.h"
#include "../version.h"
#include <bson.h>
#include <mongoc.h>
#include <mosquitto.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <string.h>
#include <assert.h>

int _running = 1;
static void clean_exit() {
  fprintf(stderr, "SIGINT\n");
  _running = 0;
}

typedef struct {
  mongoc_collection_t *coll;
  config_t *cfg;
} userdata_t;

void my_message_callback(struct mosquitto *mosq, void *userdata,
                         const struct mosquitto_message *message) {
  if (!message->payloadlen) {
    fprintf(stderr, "Skipping empty payload!\n");
  } else {
    userdata_t *ud = (userdata_t *)userdata;
    uint8_t *buf = (uint8_t *)message->payload;
    bson_t *doc;
    size_t len, i;
    bson_error_t error;
    printf("> Payload len: %d\n", message->payloadlen);
    printf("> Data:\n");
    //print_buffer(stdout, buf, message->payloadlen);
    if (strcmp(message->topic, "test/log") == 0) {
      char *json;
      doc = bson_new_from_data(buf, message->payloadlen);
      json = bson_as_json(doc, &len);

      //printf("> Document:\n%s\n> Document size: %zu\n\n", json, len);
      free(json);
      if (!mongoc_collection_insert_one(ud->coll, doc, NULL, NULL, &error)) {
        fprintf(stderr, "%s\n", error.message);
      }
      bson_destroy(doc);
    }
    else {
      //fprintf(stderr, "Message on %s: %s\n", message->topic, (char *)message->payload);
    }
  }
}

void my_connect_callback(struct mosquitto *mosq, void *userdata, int result) {
  if (!result) {
    userdata_t *ud = (userdata_t *)userdata;
    /* Subscribe to broker information topics on successful connect. */
    printf("Connected to %s.\n", ud->cfg->broker_host);
    mosquitto_subscribe(mosq, NULL, ud->cfg->mqtt_topic, 0);
  } else {
    fprintf(stderr, "Connect failed\n");
  }
}

void my_subscribe_callback(struct mosquitto *mosq, void *userdata, int mid, int qos_count, const int *granted_qos) {
  userdata_t *ud = (userdata_t *)userdata;
  /* Subscribe to broker information topics on successful connect. */
  printf("Subscribed to %s.\n", ud->cfg->mqtt_topic);
}

int main(int argc, char const *argv[]) {
  char *data;
  struct mosquitto *m;
  int status;
  mongoc_uri_t *uri;
  mongoc_client_t *client;
  mongoc_database_t *database;
  mongoc_collection_t *collection;
  bson_error_t error;
  userdata_t ud = {NULL, NULL};

  if (argc != 2) {
    fprintf(stderr, "Need exactly one argument: the path to the configuration file\n");
    return EXIT_FAILURE;
  }
  if ((ud.cfg = new_config(argv[1], CFG_SUB)) == NULL) {
    fprintf(stderr, "Could not load config file\n");
    return EXIT_FAILURE;
  }
  
  // Instal event handler
  signal(SIGINT, clean_exit);

  printf("%s Version %s\n", argv[0], GIT_COMMIT_HASH);

  //  _____                 ____  _____ 
  // |     |___ ___ ___ ___|    \| __  |
  // | | | | . |   | . | . |  |  | __ -|
  // |_|_|_|___|_|_|_  |___|____/|_____|
  //               |___|                
  mongoc_init();
  printf("MongoDB: %s, %s.%s... ", ud.cfg->mongo_uri, ud.cfg->mongo_db, ud.cfg->mongo_collection);
  fflush(stdout);
  // Safely create a MongoDB URI object from the given string
  uri = mongoc_uri_new_with_error(ud.cfg->mongo_uri, &error);
  if (!uri) {
    fprintf(stderr,
            "failed to parse URI: %s\n"
            "error message:       %s\n",
            ud.cfg->mongo_uri, error.message);
    return EXIT_FAILURE;
  }

  // Create a new client instance
  client = mongoc_client_new_from_uri(uri);
  if (!client) {
    return EXIT_FAILURE;
  }
  printf("done\n");

  // Register the application name so we can track it in the profile logs
  // on the server. This can also be done from the URI (see other examples).
  mongoc_client_set_appname(client, "ERPI-sub");

  // Get a handle on the database "test" and collection "logging"
  database = mongoc_client_get_database(client, ud.cfg->mongo_db);
  collection = mongoc_client_get_collection(client, ud.cfg->mongo_db, ud.cfg->mongo_collection);
  ud.coll = collection;

  
  //  _____                 _ _   _       
  // |     |___ ___ ___ _ _|_| |_| |_ ___ 
  // | | | | . |_ -| . | | | |  _|  _| . |
  // |_|_|_|___|___|_  |___|_|_| |_| |___|
  //                 |_|                  
  printf("MQTT: %s:%d/%s... ", ud.cfg->broker_host, ud.cfg->broker_port, ud.cfg->mqtt_topic);
  fflush(stdout);
  mosquitto_lib_init();
  // create new instance and pass userdata, so that callbacks
  // are able to access mongo collection and config struct
  m = mosquitto_new(argv[0], false, &ud);
  if (!m) {
    perror("mosquitto_new()");
    exit(EXIT_FAILURE);
  }
  assert(m != NULL);
  // Set minimum callbacks
  mosquitto_connect_callback_set(m, my_connect_callback);
  mosquitto_message_callback_set(m, my_message_callback);
  mosquitto_subscribe_callback_set(m, my_subscribe_callback);
  // connect async, since we are using the threaded API (mosquitto_loop_start())
  status = mosquitto_connect_async(m, ud.cfg->broker_host, ud.cfg->broker_port, 5);

                   
  //  __                
  // |  |   ___ ___ ___ 
  // |  |__| . | . | . |
  // |_____|___|___|  _|
  //               |_|  
  mosquitto_loop_start(m);
  fprintf(stderr, "Entering loop.\n");
  while (_running) {
    sleep(1);
  }
  fprintf(stderr, "Clean exit\n");
  mosquitto_loop_stop(m, 1);

  // Finalize
  mongoc_collection_destroy(collection);
  mongoc_database_destroy(database);
  mongoc_uri_destroy(uri);
  mongoc_client_destroy(client);
  mongoc_cleanup();

  mosquitto_destroy(m);
  mosquitto_lib_cleanup();

  // free_config(ud.cfg);

  return 0;
}
