
#ifndef UTILS_H
#define UTILS_H
#include "plugin.h"
#include <bson.h>
#include <mosquitto.h>
#include <stdio.h>
#include <string.h>
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>

typedef struct {
  lua_State *lua;
  // MongoDB
  const char *mongo_uri;
  const char *mongo_db;
  const char *mongo_collection;
  // MQTT
  const char *broker_host;
  int broker_port;
  const char *mqtt_topic;
  // CAN
  const char *plugin_path;
} config_t;

typedef enum {
  CFG_PUB, CFG_SUB
} pubsub_t;

void print_buffer(FILE * stream, const uint8_t *buf, size_t buflen);

config_t *new_config(char const *config_file, pubsub_t type);

int can_data_to_bson(can_data_t *can_data, bson_t **bson, char const *plugin_path);

#endif