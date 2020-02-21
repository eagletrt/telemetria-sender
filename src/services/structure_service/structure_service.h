#ifndef TELEMETRY_STRUCTURE_SERVICE
#define TELEMETRY_STRUCTURE_SERVICE

/* IMPORTS */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/timeb.h>
#include <sys/sysinfo.h>
#include <bson.h>
#include "../../state_machine/state_machine_condition.h"
#include "../can_service/can_service.h"
#include "../log_service/log_service.h"
#include "../gps_service/gps_service.h"
#include "structure_identifiers.h"

/* EXTERN */

extern condition_t condition;

/* TYPES */

typedef struct {
	int data_left;
	int data_right;
} inverterRight_value_data;

typedef struct {
	long timestamp;
	inverterRight_value_data value;
} inverterRight_data;

typedef struct {
	int data_left;
	int data_right;
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
	double latitude_GGA;
	double longitude_GGA;
	double altitude_GGA;
	char* ns_indicator_GGA;
	char* ew_indicator_GGA;
	char* utc_time_GGA;
	double latitude_GLL;
	double longitude_GLL;
	char* ns_indicator_GLL;
	char* ew_indicator_GLL;
	char* utc_time_GLL;
	double ground_speed_knots_VTG;
	double ground_speed_human_VTG;
	double latitude_RMC;
	double longitude_RMC;
	char* ns_indicator_RMC;
	char* ew_indicator_RMC;
	char* utc_time_RMC;
	char* date_RMC;
	double ground_speed_knots_RMC;
} gps_new_value_data;

typedef struct {
	long timestamp;
	gps_new_value_data value;
} gps_new_data;

typedef struct {
	double latitude_m;
	int latitude_o;
	double longitude_m;
	int longitude_o;
	double speed;
	double altitude;
} gps_old_location_value_data;

typedef struct {
	long timestamp;
	gps_old_location_value_data value;
} gps_old_location_data;

typedef struct {
	int hours;
	int minutes;
	int seconds;
} gps_old_time_value_data;

typedef struct {
	long timestamp;
	gps_old_time_value_data value;
} gps_old_time_data;

typedef struct {
	long timestamp;
	int value;
} gps_old_true_track_mode_data;

typedef struct {
	gps_old_location_data *location;
	int location_count;
	gps_old_time_data *time;
	int time_count;
	gps_old_true_track_mode_data *true_track_mode;
	int true_track_mode_count;
} gps_old_data;

typedef struct {
	gps_new_data *new;
	int new_count;
	gps_old_data old;
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



typedef enum { 
    GATHER_IDLE, 
    GATHER_ENABLE, 
    GATHER_KEEP, 
    GATHER_ERROR 
} gather_code;

/* FUNCTIONS */

data_t* structureCreate();
void structureToBson(data_t *document, bson_t **bson_document);
void structureDelete(data_t *document);
gather_code gatherStructure(data_t* document);
void resetStructureId();

#endif