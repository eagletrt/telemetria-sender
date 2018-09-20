/**********************************************************************
 * sub.c - MQTT subsctriber and MongoDB client                        *
 **********************************************************************/
#include "utils.h"
#include "version.h"
#include <bson.h>
#include <mongoc.h>
#include <mosquitto.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>

int _running = 1;
static void clean_exit() {
  fprintf(stderr, "SIGINT\n");
  _running = 0;
}

void my_message_callback(struct mosquitto *mosq, void *userdata,
                         const struct mosquitto_message *message) {
  if (!message->payloadlen) {
    fprintf(stderr, "Skipping empty payload!\n");
  } else {
    mongoc_collection_t *coll = (mongoc_collection_t *)userdata;
    uint8_t *buf = message->payload;
    bson_t *doc;
    size_t len, i;
    bson_error_t error;
    printf("> Payload len: %d\n", message->payloadlen);
    printf("> Data:\n");
    print_buffer(stdout, buf, message->payloadlen);
    doc = bson_new_from_data(buf, message->payloadlen);
    printf("> Document:\n%s\n> Document size: %zu\n\n", bson_as_json(doc, &len),
           len);
    if (!mongoc_collection_insert_one(coll, doc, NULL, NULL, &error)) {
      fprintf(stderr, "%s\n", error.message);
    }
    bson_destroy(doc);
  }
}

void my_connect_callback(struct mosquitto *mosq, void *userdata, int result) {
  if (!result) {
    /* Subscribe to broker information topics on successful connect. */
    mosquitto_subscribe(mosq, NULL, "test/#", 2);
  } else {
    fprintf(stderr, "Connect failed\n");
  }
}

int main(int argc, char const *argv[]) {
  char *data;
  struct mosquitto *m;
  int status;
  const char *uri_string = "mongodb://localhost:27017";
  mongoc_uri_t *uri;
  mongoc_client_t *client;
  mongoc_database_t *database;
  mongoc_collection_t *collection;
  bson_error_t error;

  // Mongo setup
  mongoc_init();

  // Optionally get MongoDB URI from command line
  if (argc > 1) {
    uri_string = argv[1];
  }

  // Safely create a MongoDB URI object from the given string
  uri = mongoc_uri_new_with_error(uri_string, &error);
  if (!uri) {
    fprintf(stderr,
            "failed to parse URI: %s\n"
            "error message:       %s\n",
            uri_string, error.message);
    return EXIT_FAILURE;
  }

  // Create a new client instance
  client = mongoc_client_new_from_uri(uri);
  if (!client) {
    return EXIT_FAILURE;
  }

  // Register the application name so we can track it in the profile logs
  // on the server. This can also be done from the URI (see other examples).
  mongoc_client_set_appname(client, "ERPI-sub");

  // Get a handle on the database "test" and collection "logging"
  database = mongoc_client_get_database(client, "test");
  collection = mongoc_client_get_collection(client, "test", "logging");

  // Instal event handler
  signal(SIGINT, clean_exit);

  printf("%s Version %s\n", argv[0], GIT_COMMIT_HASH);
  printf("Testing Mosquitto sub\n\n");

  // Mosquitto setup
  mosquitto_lib_init();
  // create new instance and pass colloction as userdata, so that callbacks
  // are able to access mongo
  m = mosquitto_new(NULL, true, collection);
  status = mosquitto_connect(m, "localhost", 1883, 60);
  if (status == MOSQ_ERR_SUCCESS) {
    fprintf(stderr, "Connected to broker\n");
  } else if (status == MOSQ_ERR_INVAL) {
    fprintf(stderr, "Error connecting\n");
    exit(EXIT_FAILURE);
  } else if (status == MOSQ_ERR_ERRNO) {
    perror("MQTT");
    exit(EXIT_FAILURE);
  }
  // Set minimum callbacks
  mosquitto_connect_callback_set(m, my_connect_callback);
  mosquitto_message_callback_set(m, my_message_callback);

  // Mosquitto event loop
  mosquitto_loop_start(m);
  while (_running) {
    sleep(1);
  }
  mosquitto_loop_stop(m, 1);

  // Finalize
  mongoc_collection_destroy(collection);
  mongoc_database_destroy(database);
  mongoc_uri_destroy(uri);
  mongoc_client_destroy(client);
  mongoc_cleanup();

  mosquitto_destroy(m);
  mosquitto_lib_cleanup();

  return 0;
}
