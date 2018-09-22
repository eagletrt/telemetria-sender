
#ifndef UTILS_H
#define UTILS_H
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
} config_t;

void print_buffer(FILE * stream, const uint8_t *buf, size_t buflen);

config_t *new_config(char const *config_file);
int load_config(config_t *cfg, char const *config_file);

#endif