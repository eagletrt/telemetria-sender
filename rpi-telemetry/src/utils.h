
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
  // Cache
  const char *cache_path;
} config_t;

typedef struct {
  lua_State *lua;
  // MongoDB
  int timing;
  int heavy_package_turnover;

  int resolver_data;
  int front_wheels_encoder_data;
  int imu_data;
  int throttle_data;  
  int brake_data;
  int steering_wheel_encoder_data;
  int gps_data;

  int bms_hv_temp_data;
  int bms_hv_volt_data;
  int inv_temp_data;  
  int inv_volt_data;
  int inv_curr_data;
  int bms_lv_temp_data;
} config_send;

typedef enum {
  CFG_PUB, CFG_SUB, CFG_SEND
} pubsub_t;

void print_buffer(FILE * stream, const uint8_t *buf, size_t buflen);

config_t *new_config(char const *config_file, pubsub_t type);
config_send *new_config_send(char const *config_file) ;

int can_data_to_bson(can_data_t *can_data, bson_t **bson, char const *plugin_path);

uint64_t now_ns();
uint64_t wait_next(uint64_t interval);

#endif