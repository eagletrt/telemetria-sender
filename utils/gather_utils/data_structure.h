#ifndef TELEMETRY_DATA_STRUCTURE
#define TELEMETRY_DATA_STRUCTURE

typedef struct {
	int pack;
	int bus;
	int max;
	int min;
} bms_hv_voltage_value_data;

typedef struct {
	long timestamp;
	bms_hv_voltage_value_data value;
} bms_hv_voltage_data;

typedef struct {
	int current;
	int power;
} bms_hv_current_value_data;

typedef struct {
	long timestamp;
	bms_hv_current_value_data value;
} bms_hv_current_data;

typedef struct {
	int avg;
	int max;
	int min;
} bms_hv_temperature_value_data;

typedef struct {
	long timestamp;
	bms_hv_temperature_value_data value;
} bms_hv_temperature_data;

typedef struct {
	bms_hv_voltage_data *voltage;
	int voltage_count;
	int voltage_size;
	bms_hv_current_data *current;
	int current_count;
	int current_size;
	bms_hv_temperature_data *temperature;
	int temperature_count;
	int temperature_size;
} bms_hv_data;

typedef struct {
	long timestamp;
	int value;
} bms_lv_current_data;

typedef struct {
	int total_voltage;
	int voltage_1;
	int voltage_2;
	int voltage_3;
	int voltage_4;
} bms_lv_voltage_value_data;

typedef struct {
	long timestamp;
	bms_lv_voltage_value_data value;
} bms_lv_voltage_data;

typedef struct {
	int battery;
	int dcdc;
} bms_lv_temperature_value_data;

typedef struct {
	long timestamp;
	bms_lv_temperature_value_data value;
} bms_lv_temperature_data;

typedef struct {
	bms_lv_current_data *current;
	int current_count;
	int current_size;
	bms_lv_voltage_data *voltage;
	int voltage_count;
	int voltage_size;
	bms_lv_temperature_data *temperature;
	int temperature_count;
	int temperature_size;
} bms_lv_data;

typedef struct {
	int x;
	int y;
	int z;
} imu_angle_value_data;

typedef struct {
	long timestamp;
	imu_angle_value_data value;
} imu_angle_data;

typedef struct {
	int x;
	int y;
	int z;
} imu_acceleration_value_data;

typedef struct {
	long timestamp;
	imu_acceleration_value_data value;
} imu_acceleration_data;

typedef struct {
	imu_angle_data *angle;
	int angle_count;
	int angle_size;
	imu_acceleration_data *acceleration;
	int acceleration_count;
	int acceleration_size;
} imu_data;

typedef struct {
	long timestamp;
	int value;
} pedals_brake_data;

typedef struct {
	long timestamp;
	int value;
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
	bms_lv_data bms_lv;
	imu_data imu;
	pedals_data pedals;
	gps_data gps;
} data_t;


#endif