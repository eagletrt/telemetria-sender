#ifndef TELEMETRY_DATA_STRUCTURE
#define TELEMETRY_DATA_STRUCTURE

typedef struct {
	int average_temp;
	int max_temp;
	int min_temp;
} bms_hv_temperature_value_data;

typedef struct {
	long timestamp;
	bms_hv_temperature_value_data value;
} bms_hv_temperature_data;

typedef struct {
	int pack_voltage;
	int bus_voltage;
	int min_cell_voltage;
	int max_cell_voltage;
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
	int error_code;
	int error_index;
	int active;
} bms_hv_errors_value_data;

typedef struct {
	long timestamp;
	bms_hv_errors_value_data value;
} bms_hv_errors_data;

typedef struct {
	bms_hv_temperature_data *temperature;
	int temperature_count;
	int temperature_size;
	bms_hv_voltage_data *voltage;
	int voltage_count;
	int voltage_size;
	bms_hv_current_data *current;
	int current_count;
	int current_size;
	bms_hv_errors_data *errors;
	int errors_count;
	int errors_size;
} bms_hv_data;

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
	gps_data gps;
} data_t;


#endif