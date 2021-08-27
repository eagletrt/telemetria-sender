#ifndef TELEMETRY_DATA_STRUCTURE
#define TELEMETRY_DATA_STRUCTURE

typedef struct {
	double total;
	double max;
	double min;
} bms_hv_voltage_value_data;

typedef struct {
	long timestamp;
	bms_hv_voltage_value_data value;
} bms_hv_voltage_data;

typedef struct {
	double average;
	double max;
	double min;
} bms_hv_temperature_value_data;

typedef struct {
	long timestamp;
	bms_hv_temperature_value_data value;
} bms_hv_temperature_data;

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
	bms_hv_voltage_data *voltage;
	int voltage_count;
	int voltage_size;
	bms_hv_temperature_data *temperature;
	int temperature_count;
	int temperature_size;
	bms_hv_current_data *current;
	int current_count;
	int current_size;
	bms_hv_errors_data *errors;
	int errors_count;
	int errors_size;
	bms_hv_warnings_data *warnings;
	int warnings_count;
	int warnings_size;
} bms_hv_data;

typedef struct {
	double x;
	double y;
	double z;
} imu_gyro_value_data;

typedef struct {
	long timestamp;
	imu_gyro_value_data value;
} imu_gyro_data;

typedef struct {
	double x;
	double y;
	double z;
} imu_accel_value_data;

typedef struct {
	long timestamp;
	imu_accel_value_data value;
} imu_accel_data;

typedef struct {
	imu_gyro_data *gyro;
	int gyro_count;
	int gyro_size;
	imu_accel_data *accel;
	int accel_count;
	int accel_size;
} imu_data;

typedef struct {
	long timestamp;
	double value;
} inverters_right_speed_data;

typedef struct {
	long timestamp;
	double value;
} inverters_right_temperature_igbt_data;

typedef struct {
	long timestamp;
	double value;
} inverters_right_temperature_motors_data;

typedef struct {
	long timestamp;
	double value;
} inverters_right_torque_data;

typedef struct {
	inverters_right_speed_data *speed;
	int speed_count;
	int speed_size;
	inverters_right_temperature_igbt_data *temperature_igbt;
	int temperature_igbt_count;
	int temperature_igbt_size;
	inverters_right_temperature_motors_data *temperature_motors;
	int temperature_motors_count;
	int temperature_motors_size;
	inverters_right_torque_data *torque;
	int torque_count;
	int torque_size;
} inverters_right_data;

typedef struct {
	long timestamp;
	double value;
} inverters_left_speed_data;

typedef struct {
	long timestamp;
	double value;
} inverters_left_temperature_igbt_data;

typedef struct {
	long timestamp;
	double value;
} inverters_left_temperature_motors_data;

typedef struct {
	long timestamp;
	double value;
} inverters_left_torque_data;

typedef struct {
	inverters_left_speed_data *speed;
	int speed_count;
	int speed_size;
	inverters_left_temperature_igbt_data *temperature_igbt;
	int temperature_igbt_count;
	int temperature_igbt_size;
	inverters_left_temperature_motors_data *temperature_motors;
	int temperature_motors_count;
	int temperature_motors_size;
	inverters_left_torque_data *torque;
	int torque_count;
	int torque_size;
} inverters_left_data;

typedef struct {
	inverters_right_data right;
	inverters_left_data left;
} inverters_data;

typedef struct {
	int is_breaking;
	double pressure_front;
	double pressure_back;
} pedals_brake_value_data;

typedef struct {
	long timestamp;
	pedals_brake_value_data value;
} pedals_brake_data;

typedef struct {
	long timestamp;
	double value;
} pedals_throttle_data;

typedef struct {
	pedals_brake_data *brake;
	int brake_count;
	int brake_size;
	pedals_throttle_data *throttle;
	int throttle_count;
	int throttle_size;
} pedals_data;

typedef struct {
	long timestamp;
	double value;
} steering_wheel_encoder_data;

typedef struct {
	steering_wheel_encoder_data *encoder;
	int encoder_count;
	int encoder_size;
} steering_wheel_data;

typedef struct {
	double latitude_safe;
	double longitude_safe;
	double latitude;
	double longitude;
	double altitude;
	char* ns_indicator;
	char* ew_indicator;
	char* utc_time;
} gps_gga_value_data;

typedef struct {
	long timestamp;
	gps_gga_value_data value;
} gps_gga_data;

typedef struct {
	double latitude;
	double longitude;
	char* ns_indicator;
	char* ew_indicator;
	char* utc_time;
} gps_gll_value_data;

typedef struct {
	long timestamp;
	gps_gll_value_data value;
} gps_gll_data;

typedef struct {
	double ground_speed_knots;
	double ground_speed_human;
} gps_vtg_value_data;

typedef struct {
	long timestamp;
	gps_vtg_value_data value;
} gps_vtg_data;

typedef struct {
	double latitude;
	double longitude;
	char* ns_indicator;
	char* ew_indicator;
	char* utc_time;
	char* date;
	double ground_speed_knots;
} gps_rmc_value_data;

typedef struct {
	long timestamp;
	gps_rmc_value_data value;
} gps_rmc_data;

typedef struct {
	gps_gga_data *gga;
	int gga_count;
	int gga_size;
	gps_gll_data *gll;
	int gll_count;
	int gll_size;
	gps_vtg_data *vtg;
	int vtg_count;
	int vtg_size;
	gps_rmc_data *rmc;
	int rmc_count;
	int rmc_size;
} gps_data;

typedef struct {
	int id;
	long timestamp;
	char* sessionName;
	bms_hv_data bms_hv;
	imu_data imu;
	inverters_data inverters;
	pedals_data pedals;
	steering_wheel_data steering_wheel;
	gps_data gps;
} data_t;


#endif