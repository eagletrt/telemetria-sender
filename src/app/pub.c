#include "version.h"
#include "utils.h"
#include <bson.h>
#include <mosquitto.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

int main(int argc, char const *argv[]) {
  const uint8_t *data;
  size_t jlen, blen;
  bson_t *docin, *doc;
  struct mosquitto *m;
  int status, i;
  struct timespec time;

  printf("%s Version %s\n", argv[0], GIT_COMMIT_HASH);
  printf("Testing BSON and Mosquitto pub\n\n");

  // Create an example document (input doc, will come from CAN)
  // Get current high resolution time
  // CLOCK_MONOTONIC ensures no fluctuations due to NTP
  if( clock_gettime(CLOCK_MONOTONIC, &time) == -1 ) {
    perror("clock gettime");
    exit(EXIT_FAILURE);
  }
  // Timestamp in BSON is expressen in milliseconds after epoch:
  // time.tv_sec * 1000 + time.tv_nsec / 1E6
  docin = BCON_NEW(
    "date", BCON_DATE_TIME(time.tv_sec * 1000 + time.tv_nsec / 1E6),
    "time", "[",
      BCON_INT64(time.tv_sec),
      BCON_INT32(time.tv_nsec),
    "]",
    "idx", BCON_INT32(1), 
    "name", BCON_UTF8("test"),
    "surname", BCON_UTF8("test2"),
    "ary", 
    "[",
      BCON_INT32(1),
      BCON_INT32(2),
      BCON_INT32(3),
      BCON_DOUBLE(3.14),
      BCON_DOUBLE(1.7E10),
    "]"
  );

  printf("> Original doc:\n%s\nlength: %d\n",
         bson_as_json(docin, &jlen), (int)jlen);
  // dump it to a data buffer
  data = bson_get_data(docin);
  printf("> Data:\n");
  print_buffer(stdout, data, docin->len);

  // check back conversion to JSON
  blen = docin->len;
  doc = bson_new_from_data(data, blen);
  printf("> Doc from BSON from raw data:\n%s\nJSON length: %zu, BSON data length: %u\n", bson_as_json(doc, &jlen), jlen, doc->len);

  printf("\nTesting Mosquitto pub\n");

  // Mosquitto initialize
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

  // Send BSON data as a buffer via MQTT
  printf("> Sending %zu bytes\n", blen);
  mosquitto_publish(m, NULL, "test/cross", blen, data, 0, false);

  // Show raw buffer
  printf("> Data sent:\n");
  print_buffer(stdout, data, blen);

  printf("> Clean exit\n");
  bson_destroy(docin);
  bson_destroy(doc);

  mosquitto_destroy(m);
  mosquitto_lib_cleanup();

  return 0;
}
