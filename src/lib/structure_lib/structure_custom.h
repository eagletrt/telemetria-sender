#ifndef STRUCT_H
#define STRUCT_H

#include <bson.h>

extern int verbose;
extern int receive_can_compact(int socket, int* id, int* data1, int*data2);
extern int telemetry_handler(int id, int data1, int data2);

typedef struct {
	int data1;
	int data2;
} inverterRight_value_data;

typedef struct {
	long timestamp;
	inverterRight_value_data value;
} inverterRight_data;

typedef struct {
	int data1;
	int data2;
} inverterLeft_value_data;

typedef struct {
	long timestamp;
	inverterLeft_value_data value;
} inverterLeft_data;

typedef struct {
	double max;
	double min;
	double average;
} bms_hv_temperature_value_data;

typedef struct {
	long timestamp;
	bms_hv_temperature_value_data value;
} bms_hv_temperature_data;

typedef struct {
	double max;
	double min;
	double total;
} bms_hv_voltage_value_data;

typedef struct {
	long timestamp;
	bms_hv_voltage_value_data value;
} bms_hv_voltage_data;

typedef struct {
	double current;
	double pow;
} bms_hv_current_value_data;

typedef struct {
	long timestamp;
	bms_hv_current_value_data value;
} bms_hv_current_data;

typedef struct {
	int fault_id;
	int fault_index;
} bms_hv_errors_value_data;

typedef struct {
	long timestamp;
	bms_hv_errors_value_data value;
} bms_hv_errors_data;

typedef struct {
	int fault_id;
	int fault_index;
} bms_hv_warnings_value_data;

typedef struct {
	long timestamp;
	bms_hv_warnings_value_data value;
} bms_hv_warnings_data;

typedef struct {
	bms_hv_temperature_data *temperature;
	int temperature_count;
	bms_hv_voltage_data *voltage;
	int voltage_count;
	bms_hv_current_data *current;
	int current_count;
	bms_hv_errors_data *errors;
	int errors_count;
	bms_hv_warnings_data *warnings;
	int warnings_count;
} bms_hv_data;

typedef struct {
	double voltage;
	double temperature;
} bms_lv_values_value_data;

typedef struct {
	long timestamp;
	bms_lv_values_value_data value;
} bms_lv_values_data;

typedef struct {
	long timestamp;
	int value;
} bms_lv_errors_data;

typedef struct {
	bms_lv_values_data *values;
	int values_count;
	bms_lv_errors_data *errors;
	int errors_count;
} bms_lv_data;

typedef struct {
	long timestamp;
	double latitude;
	double longitude;
	double altitude;
	int ns_indicator;
	int ew_indicator;
} gps_data;

typedef struct {
	double x;
	double y;
	double z;
	double scale;
} imu_gyro_value_data;

typedef struct {
	long timestamp;
	imu_gyro_value_data value;
} imu_gyro_data;

typedef struct {
	double x;
	double y;
	double z;
	double scale;
} imu_accel_value_data;

typedef struct {
	long timestamp;
	imu_accel_value_data value;
} imu_accel_data;

typedef struct {
	double speed;
	double speedms;
} front_wheels_encoder_value_data;

typedef struct {
	long timestamp;
	front_wheels_encoder_value_data value;
} front_wheels_encoder_data;

typedef struct {
	double meters;
	double rotations;
	double angle;
	double clock_period;
} distance_value_data;

typedef struct {
	long timestamp;
	distance_value_data value;
} distance_data;

typedef struct {
	long timestamp;
	double value;
} throttle_data;

typedef struct {
	long timestamp;
	double value;
} brake_data;

typedef struct {
	long timestamp;
	double value;
} steering_wheel_encoder_data;

typedef struct {
	int control;
	int cooling;
	int map;
} steering_wheel_gears_value_data;

typedef struct {
	long timestamp;
	steering_wheel_gears_value_data value;
} steering_wheel_gears_data;

typedef struct {
	steering_wheel_encoder_data *encoder;
	int encoder_count;
	steering_wheel_gears_data *gears;
	int gears_count;
	int marker;
} steering_wheel_data;

typedef struct {
	int id;
	long timestamp;
	inverterRight_data *inverterRight;
	int inverterRight_count;
	inverterLeft_data *inverterLeft;
	int inverterLeft_count;
	bms_hv_data bms_hv;
	bms_lv_data bms_lv;
	gps_data gps;
	imu_gyro_data *imu_gyro;
	int imu_gyro_count;
	imu_accel_data *imu_accel;
	int imu_accel_count;
	front_wheels_encoder_data *front_wheels_encoder;
	int front_wheels_encoder_count;
	distance_data *distance;
	int distance_count;
	throttle_data *throttle;
	int throttle_count;
	brake_data *brake;
	int brake_count;
	steering_wheel_data steering_wheel;
} data_t;


data_t* data_setup();
int data_elaborate(data_t* data, bson_t** sending);
int data_quit(data_t* data);
int data_gather(data_t* data, int timing, int socket);

#endif