
#ifndef UTILS_H
#define UTILS_H
#include <bson.h>
#include <mosquitto.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  // MongoDB
  char *mongo_host;
  int mongo_port;
  char *mongo_collection;
  // MQTT
  char *broker_host;
  int broker_port;
  char *mqtt_topic;
} config_t;

void print_buffer(FILE * stream, const uint8_t *buf, size_t buflen);
int load_config(config_t *cfg, char const *config_file);


#endif