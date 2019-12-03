#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "./jsmn/jsmn.h"

typedef struct {
 	char* broker_host;
  	int broker_port;
  	char* mqtt_topic;

  	char* mongo_host;
  	int mongo_port;
  	char* mongo_db;
  	char* mongo_collection;

  	int sending_time;

  	int sending_throttle;
  	int sending_brake;
  	int sending_steering_wheel_encoder;
  	int sending_front_wheels_encoder;
    int sending_imu_gyro;
    int sending_imu_axel;
    int sending_gps_data;
    int sending_bms_hv_data;
    int sending_bms_lv_data;
} config_t;

extern int verbose;

config_t* config_setup(const char* cfgpath);
int config_quit(config_t* cfg);

#endif