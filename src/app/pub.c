#include "version.h"
#include <bson.h>
#include <mosquitto.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[]) {
  const uint8_t *data;
  size_t jlen, blen;
  bson_t *docin, *doc;
  struct mosquitto *m;
  int status, i;

  printf("%s Version %s\n", argv[0], GIT_COMMIT_HASH);
  printf("Testing BSON and Mosquitto pub\n\n");

  // Create an example document (input doc, will come from CAN)
  docin = BCON_NEW("idx", BCON_INT32(1), "name", BCON_UTF8("test"));
  printf("> Original doc:\n%s\nlength: %d\n",
         bson_as_relaxed_extended_json(docin, &jlen), jlen);
  // dump it to a data buffer
  data = bson_get_data(docin);
  printf("> Data: ");
  for (i = 0; i < docin->len; i++) {
    printf("%02x", data[i]);
    if ((i + 5) % 4 == 0) printf(" ");
  }
  printf("\n");
  // check back conversion to JSON
  blen = docin->len;
  doc = bson_new_from_data(data, blen);
  printf("> Doc from BSON from raw data:\n%s\nJSON length: %d, BSON data length: %d\n", bson_as_json(doc, &jlen), jlen, doc->len);

  printf("\nTesting Mosquitto pub\n");

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
  printf("> Sending %d bytes\n", blen);
  mosquitto_publish(m, NULL, "test/cross", blen, data, 0, false);
  printf("> Data sent:\n");
  for (i = 0; i < blen; i++) {
    printf("%02x", data[i]);
    if ((i + 5) % 4 == 0) printf(" ");
  }
  printf("\n");

  printf("> Clean exit\n");
  bson_destroy(docin);
  bson_destroy(doc);

  mosquitto_destroy(m);
  mosquitto_lib_cleanup();

  return 0;
}
