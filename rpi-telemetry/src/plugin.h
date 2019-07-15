#ifndef PLUGIN_H
#define PLUGIN_H
#include <stdlib.h>
#include <stdint.h>

#define RESOLVER 0x01
#define FRONT_WHEELS_ENCODER 0x02
#define IMU 0x03
#define THROTTLE 0x04
#define BRAKE 0x05
#define STEERING_WHEEL_ENCODER 0x06
#define GPS 0x07
#define BMS_HV_TEMP 0x08
#define BMS_HV_VOLT 0x09
#define INV_TEMP 0x0A
#define INV_VOLT 0x0B
#define INV_CURR 0x0C
#define BMS_LV_TEMP 0x0D

typedef struct {
    double x, y, z;
} imu_tensor;

typedef struct {
    double latitude, longitude, speed, altitude;
    char lat_o, lon_o;
} gps_tensor;

typedef struct {
  double temp;
  double volt;
  double kwh;
} bms_hv;

typedef struct {
  double temp;
  double volt;
  double curr;
} inverter;

typedef struct {
  double temp;
  double volt;
} bms_lv;

//CAN DATA
typedef struct {
  uint32_t id;
  uint32_t timestamp;

  int data_lenght;
  int* data_raw;

  double resolver[20];
  double front_wheels_encoder[20];
  imu_tensor imu_gyro[20];
  imu_tensor imu_axel[20];
  int throttle[20];
  int brake[10];
  double steering_wheel_encoder[20];
  gps_tensor gps[20];
  int marker;

  bms_hv bms_hv[1];
  inverter inv[1];
  bms_lv bms_lv[1];

  //errori
} can_data_t;


/*
OLD STRUCT

typedef struct {
  uint32_t id;
  uint32_t timestamp;
  struct {
    uint32_t latitude, longitude;
    double elevation;
  } location;
  double speed;
  double odometry;
  double steering_angle;
  double brake;
  double throttle;
  struct {
    double x, y, z;
  } acceleration[N_IMUS];
  struct {
    double x, y, z;
  } gyro[N_IMUS];
  struct {
    double x, y, z;
  } magneto[N_IMUS];
  struct {
    double voltage;
    double current[3];
    double temperature[ACCUMULATOR_MODULES];
  } accumulator;
} can_data_t;*/

#endif