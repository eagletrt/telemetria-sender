#include "version.h"
#include <bson.h>
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
    uint8_t *buf = message->payload;
    bson_t *doc;
    size_t len, i;
    printf("> Payload len: %d\n", message->payloadlen);
    printf("> Data:\n");
    for (i = 0; i < message->payloadlen; i++) {
      printf("%02x", buf[i]);
      if ((i + 5) % 4 == 0)
        printf(" ");
    }
    printf("\n");
    doc = bson_new_from_data(buf, message->payloadlen);
    printf("> Document:\n%s\n> Document size: %d\n", bson_as_json(doc, &len), len);
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

  // Instal event handler
  signal(SIGINT, clean_exit);

  printf("%s Version %s\n", argv[0], GIT_COMMIT_HASH);
  printf("Testing Mosquitto sub\n\n");

  // Mosquitto setup
  mosquitto_lib_init();
  m = mosquitto_new(NULL, true, NULL);
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
  mosquitto_connect_callback_set(m, my_connect_callback);
  mosquitto_message_callback_set(m, my_message_callback);

  // Mosquitto event loop
  mosquitto_loop_start(m);
  while (_running) {
    sleep(1);
  }
  mosquitto_loop_stop(m, 1);

  // Finalize
  mosquitto_destroy(m);
  mosquitto_lib_cleanup();

  return 0;
}
