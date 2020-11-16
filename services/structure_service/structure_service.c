#include "structure_service.h" 

/* INTERNAL TYPES */

typedef struct {
    data_t* document;
    gather_code* outcome;
} gather_can_args_t;

typedef struct {
    data_t* document;
} gather_gps_args_t;

/* INTERNAL FUNCTIONS SIGNATURES */

static void resetGpsElement(data_t *document, int index);
static double parseNmeaCoord(double coord);
static double getMillisecondsFromTimespec(struct timespec time);
static long long int getCurrentTimestamp();
static void* gatherCan(void *args);
static void* gatherGps(void *args);

/* EXPORTED FUNCTIONS */

data_t* structureCreate() {
	data_t* data = (data_t*) malloc(sizeof(data_t));
	data->steering_wheel.marker = 0;
	data->inverters.right.speed_size = 500;
	data->inverters.right.speed = (inverters_right_speed_data*) malloc(sizeof(inverters_right_speed_data) * data->inverters.right.speed_size);
	data->inverters.right.speed_count = 0;
	data->inverters.right.temperature_igbt_size = 500;
	data->inverters.right.temperature_igbt = (inverters_right_temperature_igbt_data*) malloc(sizeof(inverters_right_temperature_igbt_data) * data->inverters.right.temperature_igbt_size);
	data->inverters.right.temperature_igbt_count = 0;
	data->inverters.right.temperature_motors_size = 500;
	data->inverters.right.temperature_motors = (inverters_right_temperature_motors_data*) malloc(sizeof(inverters_right_temperature_motors_data) * data->inverters.right.temperature_motors_size);
	data->inverters.right.temperature_motors_count = 0;
	data->inverters.right.torque_size = 500;
	data->inverters.right.torque = (inverters_right_torque_data*) malloc(sizeof(inverters_right_torque_data) * data->inverters.right.torque_size);
	data->inverters.right.torque_count = 0;
	data->inverters.left.speed_size = 500;
	data->inverters.left.speed = (inverters_left_speed_data*) malloc(sizeof(inverters_left_speed_data) * data->inverters.left.speed_size);
	data->inverters.left.speed_count = 0;
	data->inverters.left.temperature_igbt_size = 500;
	data->inverters.left.temperature_igbt = (inverters_left_temperature_igbt_data*) malloc(sizeof(inverters_left_temperature_igbt_data) * data->inverters.left.temperature_igbt_size);
	data->inverters.left.temperature_igbt_count = 0;
	data->inverters.left.temperature_motors_size = 500;
	data->inverters.left.temperature_motors = (inverters_left_temperature_motors_data*) malloc(sizeof(inverters_left_temperature_motors_data) * data->inverters.left.temperature_motors_size);
	data->inverters.left.temperature_motors_count = 0;
	data->inverters.left.torque_size = 500;
	data->inverters.left.torque = (inverters_left_torque_data*) malloc(sizeof(inverters_left_torque_data) * data->inverters.left.torque_size);
	data->inverters.left.torque_count = 0;
	data->bms_hv.temperature_size = 500;
	data->bms_hv.temperature = (bms_hv_temperature_data*) malloc(sizeof(bms_hv_temperature_data) * data->bms_hv.temperature_size);
	data->bms_hv.temperature_count = 0;
	data->bms_hv.voltage_size = 500;
	data->bms_hv.voltage = (bms_hv_voltage_data*) malloc(sizeof(bms_hv_voltage_data) * data->bms_hv.voltage_size);
	data->bms_hv.voltage_count = 0;
	data->bms_hv.current_size = 500;
	data->bms_hv.current = (bms_hv_current_data*) malloc(sizeof(bms_hv_current_data) * data->bms_hv.current_size);
	data->bms_hv.current_count = 0;
	data->bms_hv.errors_size = 500;
	data->bms_hv.errors = (bms_hv_errors_data*) malloc(sizeof(bms_hv_errors_data) * data->bms_hv.errors_size);
	data->bms_hv.errors_count = 0;
	data->bms_hv.warnings_size = 500;
	data->bms_hv.warnings = (bms_hv_warnings_data*) malloc(sizeof(bms_hv_warnings_data) * data->bms_hv.warnings_size);
	data->bms_hv.warnings_count = 0;
	data->bms_lv.values_size = 500;
	data->bms_lv.values = (bms_lv_values_data*) malloc(sizeof(bms_lv_values_data) * data->bms_lv.values_size);
	data->bms_lv.values_count = 0;
	data->bms_lv.errors_size = 500;
	data->bms_lv.errors = (bms_lv_errors_data*) malloc(sizeof(bms_lv_errors_data) * data->bms_lv.errors_size);
	data->bms_lv.errors_count = 0;
	data->gps.new.gga_size = 500;
	data->gps.new.gga = (gps_new_gga_data*) malloc(sizeof(gps_new_gga_data) * data->gps.new.gga_size);
	data->gps.new.gga_count = 0;
	data->gps.new.gll_size = 500;
	data->gps.new.gll = (gps_new_gll_data*) malloc(sizeof(gps_new_gll_data) * data->gps.new.gll_size);
	data->gps.new.gll_count = 0;
	data->gps.new.vtg_size = 500;
	data->gps.new.vtg = (gps_new_vtg_data*) malloc(sizeof(gps_new_vtg_data) * data->gps.new.vtg_size);
	data->gps.new.vtg_count = 0;
	data->gps.new.rmc_size = 500;
	data->gps.new.rmc = (gps_new_rmc_data*) malloc(sizeof(gps_new_rmc_data) * data->gps.new.rmc_size);
	data->gps.new.rmc_count = 0;
	data->gps.old.location_size = 500;
	data->gps.old.location = (gps_old_location_data*) malloc(sizeof(gps_old_location_data) * data->gps.old.location_size);
	data->gps.old.location_count = 0;
	data->gps.old.time_size = 500;
	data->gps.old.time = (gps_old_time_data*) malloc(sizeof(gps_old_time_data) * data->gps.old.time_size);
	data->gps.old.time_count = 0;
	data->gps.old.true_track_mode_size = 500;
	data->gps.old.true_track_mode = (gps_old_true_track_mode_data*) malloc(sizeof(gps_old_true_track_mode_data) * data->gps.old.true_track_mode_size);
	data->gps.old.true_track_mode_count = 0;
	data->imu_old.gyro_size = 500;
	data->imu_old.gyro = (imu_old_gyro_data*) malloc(sizeof(imu_old_gyro_data) * data->imu_old.gyro_size);
	data->imu_old.gyro_count = 0;
	data->imu_old.accel_size = 500;
	data->imu_old.accel = (imu_old_accel_data*) malloc(sizeof(imu_old_accel_data) * data->imu_old.accel_size);
	data->imu_old.accel_count = 0;
	data->imu.gyro_size = 500;
	data->imu.gyro = (imu_gyro_data*) malloc(sizeof(imu_gyro_data) * data->imu.gyro_size);
	data->imu.gyro_count = 0;
	data->imu.accel_size = 500;
	data->imu.accel = (imu_accel_data*) malloc(sizeof(imu_accel_data) * data->imu.accel_size);
	data->imu.accel_count = 0;
	data->front_wheels_encoders.right.speed_size = 500;
	data->front_wheels_encoders.right.speed = (front_wheels_encoders_right_speed_data*) malloc(sizeof(front_wheels_encoders_right_speed_data) * data->front_wheels_encoders.right.speed_size);
	data->front_wheels_encoders.right.speed_count = 0;
	data->front_wheels_encoders.right.speed_rads_size = 500;
	data->front_wheels_encoders.right.speed_rads = (front_wheels_encoders_right_speed_rads_data*) malloc(sizeof(front_wheels_encoders_right_speed_rads_data) * data->front_wheels_encoders.right.speed_rads_size);
	data->front_wheels_encoders.right.speed_rads_count = 0;
	data->front_wheels_encoders.right.angle_size = 500;
	data->front_wheels_encoders.right.angle = (front_wheels_encoders_right_angle_data*) malloc(sizeof(front_wheels_encoders_right_angle_data) * data->front_wheels_encoders.right.angle_size);
	data->front_wheels_encoders.right.angle_count = 0;
	data->front_wheels_encoders.left.speed_size = 500;
	data->front_wheels_encoders.left.speed = (front_wheels_encoders_left_speed_data*) malloc(sizeof(front_wheels_encoders_left_speed_data) * data->front_wheels_encoders.left.speed_size);
	data->front_wheels_encoders.left.speed_count = 0;
	data->front_wheels_encoders.left.speed_rads_size = 500;
	data->front_wheels_encoders.left.speed_rads = (front_wheels_encoders_left_speed_rads_data*) malloc(sizeof(front_wheels_encoders_left_speed_rads_data) * data->front_wheels_encoders.left.speed_rads_size);
	data->front_wheels_encoders.left.speed_rads_count = 0;
	data->front_wheels_encoders.left.angle_size = 500;
	data->front_wheels_encoders.left.angle = (front_wheels_encoders_left_angle_data*) malloc(sizeof(front_wheels_encoders_left_angle_data) * data->front_wheels_encoders.left.angle_size);
	data->front_wheels_encoders.left.angle_count = 0;
	data->distance_size = 500;
	data->distance = (distance_data*) malloc(sizeof(distance_data) * data->distance_size);
	data->distance_count = 0;
	data->pedals.throttle_size = 500;
	data->pedals.throttle = (pedals_throttle_data*) malloc(sizeof(pedals_throttle_data) * data->pedals.throttle_size);
	data->pedals.throttle_count = 0;
	data->pedals.brake_size = 500;
	data->pedals.brake = (pedals_brake_data*) malloc(sizeof(pedals_brake_data) * data->pedals.brake_size);
	data->pedals.brake_count = 0;
	data->steering_wheel.encoder_size = 500;
	data->steering_wheel.encoder = (steering_wheel_encoder_data*) malloc(sizeof(steering_wheel_encoder_data) * data->steering_wheel.encoder_size);
	data->steering_wheel.encoder_count = 0;
	data->steering_wheel.gears_size = 500;
	data->steering_wheel.gears = (steering_wheel_gears_data*) malloc(sizeof(steering_wheel_gears_data) * data->steering_wheel.gears_size);
	data->steering_wheel.gears_count = 0;
	
	return data;
}

void structureToBson(data_t *data, bson_t** bson_document) {
	*bson_document = bson_new();
	bson_t *children = (bson_t*)malloc(sizeof(bson_t) * 5);
	BSON_APPEND_INT32(*bson_document, "id", data->id);
	BSON_APPEND_INT64(*bson_document, "timestamp", data->timestamp);
	BSON_APPEND_UTF8(*bson_document, "sessionName", data->sessionName);
	BSON_APPEND_DOCUMENT_BEGIN(*bson_document, "inverters", &children[0]);
	BSON_APPEND_DOCUMENT_BEGIN(&children[0], "right", &children[1]);
	BSON_APPEND_ARRAY_BEGIN(&children[1], "speed", &children[2]);
	for (int i = 0; i < (data->inverters.right.speed_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "0", &children[3]);
		BSON_APPEND_INT64(&children[3], "timestamp", data->inverters.right.speed[i].timestamp);
		BSON_APPEND_DOUBLE(&children[3], "value", data->inverters.right.speed[i].value);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
	}
	bson_append_array_end(&children[1], &children[2]);
	bson_destroy(&children[2]);
	BSON_APPEND_ARRAY_BEGIN(&children[1], "temperature_igbt", &children[2]);
	for (int i = 0; i < (data->inverters.right.temperature_igbt_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "0", &children[3]);
		BSON_APPEND_INT64(&children[3], "timestamp", data->inverters.right.temperature_igbt[i].timestamp);
		BSON_APPEND_DOUBLE(&children[3], "value", data->inverters.right.temperature_igbt[i].value);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
	}
	bson_append_array_end(&children[1], &children[2]);
	bson_destroy(&children[2]);
	BSON_APPEND_ARRAY_BEGIN(&children[1], "temperature_motors", &children[2]);
	for (int i = 0; i < (data->inverters.right.temperature_motors_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "0", &children[3]);
		BSON_APPEND_INT64(&children[3], "timestamp", data->inverters.right.temperature_motors[i].timestamp);
		BSON_APPEND_DOUBLE(&children[3], "value", data->inverters.right.temperature_motors[i].value);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
	}
	bson_append_array_end(&children[1], &children[2]);
	bson_destroy(&children[2]);
	BSON_APPEND_ARRAY_BEGIN(&children[1], "torque", &children[2]);
	for (int i = 0; i < (data->inverters.right.torque_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "0", &children[3]);
		BSON_APPEND_INT64(&children[3], "timestamp", data->inverters.right.torque[i].timestamp);
		BSON_APPEND_DOUBLE(&children[3], "value", data->inverters.right.torque[i].value);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
	}
	bson_append_array_end(&children[1], &children[2]);
	bson_destroy(&children[2]);
	bson_append_document_end(&children[0], &children[1]);
	bson_destroy(&children[1]);
	BSON_APPEND_DOCUMENT_BEGIN(&children[0], "left", &children[1]);
	BSON_APPEND_ARRAY_BEGIN(&children[1], "speed", &children[2]);
	for (int i = 0; i < (data->inverters.left.speed_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "0", &children[3]);
		BSON_APPEND_INT64(&children[3], "timestamp", data->inverters.left.speed[i].timestamp);
		BSON_APPEND_DOUBLE(&children[3], "value", data->inverters.left.speed[i].value);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
	}
	bson_append_array_end(&children[1], &children[2]);
	bson_destroy(&children[2]);
	BSON_APPEND_ARRAY_BEGIN(&children[1], "temperature_igbt", &children[2]);
	for (int i = 0; i < (data->inverters.left.temperature_igbt_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "0", &children[3]);
		BSON_APPEND_INT64(&children[3], "timestamp", data->inverters.left.temperature_igbt[i].timestamp);
		BSON_APPEND_DOUBLE(&children[3], "value", data->inverters.left.temperature_igbt[i].value);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
	}
	bson_append_array_end(&children[1], &children[2]);
	bson_destroy(&children[2]);
	BSON_APPEND_ARRAY_BEGIN(&children[1], "temperature_motors", &children[2]);
	for (int i = 0; i < (data->inverters.left.temperature_motors_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "0", &children[3]);
		BSON_APPEND_INT64(&children[3], "timestamp", data->inverters.left.temperature_motors[i].timestamp);
		BSON_APPEND_DOUBLE(&children[3], "value", data->inverters.left.temperature_motors[i].value);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
	}
	bson_append_array_end(&children[1], &children[2]);
	bson_destroy(&children[2]);
	BSON_APPEND_ARRAY_BEGIN(&children[1], "torque", &children[2]);
	for (int i = 0; i < (data->inverters.left.torque_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "0", &children[3]);
		BSON_APPEND_INT64(&children[3], "timestamp", data->inverters.left.torque[i].timestamp);
		BSON_APPEND_DOUBLE(&children[3], "value", data->inverters.left.torque[i].value);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
	}
	bson_append_array_end(&children[1], &children[2]);
	bson_destroy(&children[2]);
	bson_append_document_end(&children[0], &children[1]);
	bson_destroy(&children[1]);
	bson_append_document_end(*bson_document, &children[0]);
	bson_destroy(&children[0]);
	BSON_APPEND_DOCUMENT_BEGIN(*bson_document, "bms_hv", &children[0]);
	BSON_APPEND_ARRAY_BEGIN(&children[0], "temperature", &children[1]);
	for (int i = 0; i < (data->bms_hv.temperature_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[1], "0", &children[2]);
		BSON_APPEND_INT64(&children[2], "timestamp", data->bms_hv.temperature[i].timestamp);
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "value", &children[3]);
		BSON_APPEND_DOUBLE(&children[3], "max", data->bms_hv.temperature[i].value.max);
		BSON_APPEND_DOUBLE(&children[3], "min", data->bms_hv.temperature[i].value.min);
		BSON_APPEND_DOUBLE(&children[3], "average", data->bms_hv.temperature[i].value.average);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
		bson_append_document_end(&children[1], &children[2]);
		bson_destroy(&children[2]);
	}
	bson_append_array_end(&children[0], &children[1]);
	bson_destroy(&children[1]);
	BSON_APPEND_ARRAY_BEGIN(&children[0], "voltage", &children[1]);
	for (int i = 0; i < (data->bms_hv.voltage_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[1], "0", &children[2]);
		BSON_APPEND_INT64(&children[2], "timestamp", data->bms_hv.voltage[i].timestamp);
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "value", &children[3]);
		BSON_APPEND_DOUBLE(&children[3], "max", data->bms_hv.voltage[i].value.max);
		BSON_APPEND_DOUBLE(&children[3], "min", data->bms_hv.voltage[i].value.min);
		BSON_APPEND_DOUBLE(&children[3], "total", data->bms_hv.voltage[i].value.total);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
		bson_append_document_end(&children[1], &children[2]);
		bson_destroy(&children[2]);
	}
	bson_append_array_end(&children[0], &children[1]);
	bson_destroy(&children[1]);
	BSON_APPEND_ARRAY_BEGIN(&children[0], "current", &children[1]);
	for (int i = 0; i < (data->bms_hv.current_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[1], "0", &children[2]);
		BSON_APPEND_INT64(&children[2], "timestamp", data->bms_hv.current[i].timestamp);
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "value", &children[3]);
		BSON_APPEND_DOUBLE(&children[3], "current", data->bms_hv.current[i].value.current);
		BSON_APPEND_DOUBLE(&children[3], "pow", data->bms_hv.current[i].value.pow);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
		bson_append_document_end(&children[1], &children[2]);
		bson_destroy(&children[2]);
	}
	bson_append_array_end(&children[0], &children[1]);
	bson_destroy(&children[1]);
	BSON_APPEND_ARRAY_BEGIN(&children[0], "errors", &children[1]);
	for (int i = 0; i < (data->bms_hv.errors_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[1], "0", &children[2]);
		BSON_APPEND_INT64(&children[2], "timestamp", data->bms_hv.errors[i].timestamp);
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "value", &children[3]);
		BSON_APPEND_INT32(&children[3], "fault_id", data->bms_hv.errors[i].value.fault_id);
		BSON_APPEND_INT32(&children[3], "fault_index", data->bms_hv.errors[i].value.fault_index);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
		bson_append_document_end(&children[1], &children[2]);
		bson_destroy(&children[2]);
	}
	bson_append_array_end(&children[0], &children[1]);
	bson_destroy(&children[1]);
	BSON_APPEND_ARRAY_BEGIN(&children[0], "warnings", &children[1]);
	for (int i = 0; i < (data->bms_hv.warnings_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[1], "0", &children[2]);
		BSON_APPEND_INT64(&children[2], "timestamp", data->bms_hv.warnings[i].timestamp);
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "value", &children[3]);
		BSON_APPEND_INT32(&children[3], "fault_id", data->bms_hv.warnings[i].value.fault_id);
		BSON_APPEND_INT32(&children[3], "fault_index", data->bms_hv.warnings[i].value.fault_index);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
		bson_append_document_end(&children[1], &children[2]);
		bson_destroy(&children[2]);
	}
	bson_append_array_end(&children[0], &children[1]);
	bson_destroy(&children[1]);
	bson_append_document_end(*bson_document, &children[0]);
	bson_destroy(&children[0]);
	BSON_APPEND_DOCUMENT_BEGIN(*bson_document, "bms_lv", &children[0]);
	BSON_APPEND_ARRAY_BEGIN(&children[0], "values", &children[1]);
	for (int i = 0; i < (data->bms_lv.values_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[1], "0", &children[2]);
		BSON_APPEND_INT64(&children[2], "timestamp", data->bms_lv.values[i].timestamp);
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "value", &children[3]);
		BSON_APPEND_DOUBLE(&children[3], "voltage", data->bms_lv.values[i].value.voltage);
		BSON_APPEND_DOUBLE(&children[3], "temperature", data->bms_lv.values[i].value.temperature);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
		bson_append_document_end(&children[1], &children[2]);
		bson_destroy(&children[2]);
	}
	bson_append_array_end(&children[0], &children[1]);
	bson_destroy(&children[1]);
	BSON_APPEND_ARRAY_BEGIN(&children[0], "errors", &children[1]);
	for (int i = 0; i < (data->bms_lv.errors_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[1], "0", &children[2]);
		BSON_APPEND_INT64(&children[2], "timestamp", data->bms_lv.errors[i].timestamp);
		BSON_APPEND_INT32(&children[2], "value", data->bms_lv.errors[i].value);
		bson_append_document_end(&children[1], &children[2]);
		bson_destroy(&children[2]);
	}
	bson_append_array_end(&children[0], &children[1]);
	bson_destroy(&children[1]);
	bson_append_document_end(*bson_document, &children[0]);
	bson_destroy(&children[0]);
	BSON_APPEND_DOCUMENT_BEGIN(*bson_document, "gps", &children[0]);
	BSON_APPEND_DOCUMENT_BEGIN(&children[0], "new", &children[1]);
	BSON_APPEND_ARRAY_BEGIN(&children[1], "gga", &children[2]);
	for (int i = 0; i < (data->gps.new.gga_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "0", &children[3]);
		BSON_APPEND_INT64(&children[3], "timestamp", data->gps.new.gga[i].timestamp);
		BSON_APPEND_DOCUMENT_BEGIN(&children[3], "value", &children[4]);
		BSON_APPEND_DOUBLE(&children[4], "latitude_safe", data->gps.new.gga[i].value.latitude_safe);
		BSON_APPEND_DOUBLE(&children[4], "longitude_safe", data->gps.new.gga[i].value.longitude_safe);
		BSON_APPEND_DOUBLE(&children[4], "latitude", data->gps.new.gga[i].value.latitude);
		BSON_APPEND_DOUBLE(&children[4], "longitude", data->gps.new.gga[i].value.longitude);
		BSON_APPEND_DOUBLE(&children[4], "altitude", data->gps.new.gga[i].value.altitude);
		BSON_APPEND_UTF8(&children[4], "ns_indicator", data->gps.new.gga[i].value.ns_indicator);
		BSON_APPEND_UTF8(&children[4], "ew_indicator", data->gps.new.gga[i].value.ew_indicator);
		BSON_APPEND_UTF8(&children[4], "utc_time", data->gps.new.gga[i].value.utc_time);
		bson_append_document_end(&children[3], &children[4]);
		bson_destroy(&children[4]);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
	}
	bson_append_array_end(&children[1], &children[2]);
	bson_destroy(&children[2]);
	BSON_APPEND_ARRAY_BEGIN(&children[1], "gll", &children[2]);
	for (int i = 0; i < (data->gps.new.gll_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "0", &children[3]);
		BSON_APPEND_INT64(&children[3], "timestamp", data->gps.new.gll[i].timestamp);
		BSON_APPEND_DOCUMENT_BEGIN(&children[3], "value", &children[4]);
		BSON_APPEND_DOUBLE(&children[4], "latitude", data->gps.new.gll[i].value.latitude);
		BSON_APPEND_DOUBLE(&children[4], "longitude", data->gps.new.gll[i].value.longitude);
		BSON_APPEND_UTF8(&children[4], "ns_indicator", data->gps.new.gll[i].value.ns_indicator);
		BSON_APPEND_UTF8(&children[4], "ew_indicator", data->gps.new.gll[i].value.ew_indicator);
		BSON_APPEND_UTF8(&children[4], "utc_time", data->gps.new.gll[i].value.utc_time);
		bson_append_document_end(&children[3], &children[4]);
		bson_destroy(&children[4]);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
	}
	bson_append_array_end(&children[1], &children[2]);
	bson_destroy(&children[2]);
	BSON_APPEND_ARRAY_BEGIN(&children[1], "vtg", &children[2]);
	for (int i = 0; i < (data->gps.new.vtg_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "0", &children[3]);
		BSON_APPEND_INT64(&children[3], "timestamp", data->gps.new.vtg[i].timestamp);
		BSON_APPEND_DOCUMENT_BEGIN(&children[3], "value", &children[4]);
		BSON_APPEND_DOUBLE(&children[4], "ground_speed_knots", data->gps.new.vtg[i].value.ground_speed_knots);
		BSON_APPEND_DOUBLE(&children[4], "ground_speed_human", data->gps.new.vtg[i].value.ground_speed_human);
		bson_append_document_end(&children[3], &children[4]);
		bson_destroy(&children[4]);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
	}
	bson_append_array_end(&children[1], &children[2]);
	bson_destroy(&children[2]);
	BSON_APPEND_ARRAY_BEGIN(&children[1], "rmc", &children[2]);
	for (int i = 0; i < (data->gps.new.rmc_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "0", &children[3]);
		BSON_APPEND_INT64(&children[3], "timestamp", data->gps.new.rmc[i].timestamp);
		BSON_APPEND_DOCUMENT_BEGIN(&children[3], "value", &children[4]);
		BSON_APPEND_DOUBLE(&children[4], "latitude", data->gps.new.rmc[i].value.latitude);
		BSON_APPEND_DOUBLE(&children[4], "longitude", data->gps.new.rmc[i].value.longitude);
		BSON_APPEND_UTF8(&children[4], "ns_indicator", data->gps.new.rmc[i].value.ns_indicator);
		BSON_APPEND_UTF8(&children[4], "ew_indicator", data->gps.new.rmc[i].value.ew_indicator);
		BSON_APPEND_UTF8(&children[4], "utc_time", data->gps.new.rmc[i].value.utc_time);
		BSON_APPEND_UTF8(&children[4], "date", data->gps.new.rmc[i].value.date);
		BSON_APPEND_DOUBLE(&children[4], "ground_speed_knots", data->gps.new.rmc[i].value.ground_speed_knots);
		bson_append_document_end(&children[3], &children[4]);
		bson_destroy(&children[4]);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
	}
	bson_append_array_end(&children[1], &children[2]);
	bson_destroy(&children[2]);
	bson_append_document_end(&children[0], &children[1]);
	bson_destroy(&children[1]);
	BSON_APPEND_DOCUMENT_BEGIN(&children[0], "old", &children[1]);
	BSON_APPEND_ARRAY_BEGIN(&children[1], "location", &children[2]);
	for (int i = 0; i < (data->gps.old.location_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "0", &children[3]);
		BSON_APPEND_INT64(&children[3], "timestamp", data->gps.old.location[i].timestamp);
		BSON_APPEND_DOCUMENT_BEGIN(&children[3], "value", &children[4]);
		BSON_APPEND_DOUBLE(&children[4], "latitude_m", data->gps.old.location[i].value.latitude_m);
		BSON_APPEND_INT32(&children[4], "latitude_o", data->gps.old.location[i].value.latitude_o);
		BSON_APPEND_DOUBLE(&children[4], "longitude_m", data->gps.old.location[i].value.longitude_m);
		BSON_APPEND_INT32(&children[4], "longitude_o", data->gps.old.location[i].value.longitude_o);
		BSON_APPEND_DOUBLE(&children[4], "speed", data->gps.old.location[i].value.speed);
		BSON_APPEND_DOUBLE(&children[4], "altitude", data->gps.old.location[i].value.altitude);
		bson_append_document_end(&children[3], &children[4]);
		bson_destroy(&children[4]);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
	}
	bson_append_array_end(&children[1], &children[2]);
	bson_destroy(&children[2]);
	BSON_APPEND_ARRAY_BEGIN(&children[1], "time", &children[2]);
	for (int i = 0; i < (data->gps.old.time_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "0", &children[3]);
		BSON_APPEND_INT64(&children[3], "timestamp", data->gps.old.time[i].timestamp);
		BSON_APPEND_DOCUMENT_BEGIN(&children[3], "value", &children[4]);
		BSON_APPEND_INT32(&children[4], "hours", data->gps.old.time[i].value.hours);
		BSON_APPEND_INT32(&children[4], "minutes", data->gps.old.time[i].value.minutes);
		BSON_APPEND_INT32(&children[4], "seconds", data->gps.old.time[i].value.seconds);
		bson_append_document_end(&children[3], &children[4]);
		bson_destroy(&children[4]);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
	}
	bson_append_array_end(&children[1], &children[2]);
	bson_destroy(&children[2]);
	BSON_APPEND_ARRAY_BEGIN(&children[1], "true_track_mode", &children[2]);
	for (int i = 0; i < (data->gps.old.true_track_mode_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "0", &children[3]);
		BSON_APPEND_INT64(&children[3], "timestamp", data->gps.old.true_track_mode[i].timestamp);
		BSON_APPEND_INT32(&children[3], "value", data->gps.old.true_track_mode[i].value);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
	}
	bson_append_array_end(&children[1], &children[2]);
	bson_destroy(&children[2]);
	bson_append_document_end(&children[0], &children[1]);
	bson_destroy(&children[1]);
	bson_append_document_end(*bson_document, &children[0]);
	bson_destroy(&children[0]);
	BSON_APPEND_DOCUMENT_BEGIN(*bson_document, "imu_old", &children[0]);
	BSON_APPEND_ARRAY_BEGIN(&children[0], "gyro", &children[1]);
	for (int i = 0; i < (data->imu_old.gyro_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[1], "0", &children[2]);
		BSON_APPEND_INT64(&children[2], "timestamp", data->imu_old.gyro[i].timestamp);
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "value", &children[3]);
		BSON_APPEND_DOUBLE(&children[3], "x", data->imu_old.gyro[i].value.x);
		BSON_APPEND_DOUBLE(&children[3], "y", data->imu_old.gyro[i].value.y);
		BSON_APPEND_DOUBLE(&children[3], "z", data->imu_old.gyro[i].value.z);
		BSON_APPEND_DOUBLE(&children[3], "scale", data->imu_old.gyro[i].value.scale);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
		bson_append_document_end(&children[1], &children[2]);
		bson_destroy(&children[2]);
	}
	bson_append_array_end(&children[0], &children[1]);
	bson_destroy(&children[1]);
	BSON_APPEND_ARRAY_BEGIN(&children[0], "accel", &children[1]);
	for (int i = 0; i < (data->imu_old.accel_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[1], "0", &children[2]);
		BSON_APPEND_INT64(&children[2], "timestamp", data->imu_old.accel[i].timestamp);
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "value", &children[3]);
		BSON_APPEND_DOUBLE(&children[3], "x", data->imu_old.accel[i].value.x);
		BSON_APPEND_DOUBLE(&children[3], "y", data->imu_old.accel[i].value.y);
		BSON_APPEND_DOUBLE(&children[3], "z", data->imu_old.accel[i].value.z);
		BSON_APPEND_DOUBLE(&children[3], "scale", data->imu_old.accel[i].value.scale);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
		bson_append_document_end(&children[1], &children[2]);
		bson_destroy(&children[2]);
	}
	bson_append_array_end(&children[0], &children[1]);
	bson_destroy(&children[1]);
	bson_append_document_end(*bson_document, &children[0]);
	bson_destroy(&children[0]);
	BSON_APPEND_DOCUMENT_BEGIN(*bson_document, "imu", &children[0]);
	BSON_APPEND_ARRAY_BEGIN(&children[0], "gyro", &children[1]);
	for (int i = 0; i < (data->imu.gyro_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[1], "0", &children[2]);
		BSON_APPEND_INT64(&children[2], "timestamp", data->imu.gyro[i].timestamp);
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "value", &children[3]);
		BSON_APPEND_DOUBLE(&children[3], "x", data->imu.gyro[i].value.x);
		BSON_APPEND_DOUBLE(&children[3], "y", data->imu.gyro[i].value.y);
		BSON_APPEND_DOUBLE(&children[3], "z", data->imu.gyro[i].value.z);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
		bson_append_document_end(&children[1], &children[2]);
		bson_destroy(&children[2]);
	}
	bson_append_array_end(&children[0], &children[1]);
	bson_destroy(&children[1]);
	BSON_APPEND_ARRAY_BEGIN(&children[0], "accel", &children[1]);
	for (int i = 0; i < (data->imu.accel_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[1], "0", &children[2]);
		BSON_APPEND_INT64(&children[2], "timestamp", data->imu.accel[i].timestamp);
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "value", &children[3]);
		BSON_APPEND_DOUBLE(&children[3], "x", data->imu.accel[i].value.x);
		BSON_APPEND_DOUBLE(&children[3], "y", data->imu.accel[i].value.y);
		BSON_APPEND_DOUBLE(&children[3], "z", data->imu.accel[i].value.z);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
		bson_append_document_end(&children[1], &children[2]);
		bson_destroy(&children[2]);
	}
	bson_append_array_end(&children[0], &children[1]);
	bson_destroy(&children[1]);
	bson_append_document_end(*bson_document, &children[0]);
	bson_destroy(&children[0]);
	BSON_APPEND_DOCUMENT_BEGIN(*bson_document, "front_wheels_encoders", &children[0]);
	BSON_APPEND_DOCUMENT_BEGIN(&children[0], "right", &children[1]);
	BSON_APPEND_ARRAY_BEGIN(&children[1], "speed", &children[2]);
	for (int i = 0; i < (data->front_wheels_encoders.right.speed_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "0", &children[3]);
		BSON_APPEND_INT64(&children[3], "timestamp", data->front_wheels_encoders.right.speed[i].timestamp);
		BSON_APPEND_DOCUMENT_BEGIN(&children[3], "value", &children[4]);
		BSON_APPEND_DOUBLE(&children[4], "speed", data->front_wheels_encoders.right.speed[i].value.speed);
		BSON_APPEND_INT32(&children[4], "error_flag", data->front_wheels_encoders.right.speed[i].value.error_flag);
		bson_append_document_end(&children[3], &children[4]);
		bson_destroy(&children[4]);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
	}
	bson_append_array_end(&children[1], &children[2]);
	bson_destroy(&children[2]);
	BSON_APPEND_ARRAY_BEGIN(&children[1], "speed_rads", &children[2]);
	for (int i = 0; i < (data->front_wheels_encoders.right.speed_rads_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "0", &children[3]);
		BSON_APPEND_INT64(&children[3], "timestamp", data->front_wheels_encoders.right.speed_rads[i].timestamp);
		BSON_APPEND_DOUBLE(&children[3], "value", data->front_wheels_encoders.right.speed_rads[i].value);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
	}
	bson_append_array_end(&children[1], &children[2]);
	bson_destroy(&children[2]);
	BSON_APPEND_ARRAY_BEGIN(&children[1], "angle", &children[2]);
	for (int i = 0; i < (data->front_wheels_encoders.right.angle_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "0", &children[3]);
		BSON_APPEND_INT64(&children[3], "timestamp", data->front_wheels_encoders.right.angle[i].timestamp);
		BSON_APPEND_DOCUMENT_BEGIN(&children[3], "value", &children[4]);
		BSON_APPEND_DOUBLE(&children[4], "angle_0", data->front_wheels_encoders.right.angle[i].value.angle_0);
		BSON_APPEND_DOUBLE(&children[4], "angle_1", data->front_wheels_encoders.right.angle[i].value.angle_1);
		BSON_APPEND_DOUBLE(&children[4], "angle_delta", data->front_wheels_encoders.right.angle[i].value.angle_delta);
		bson_append_document_end(&children[3], &children[4]);
		bson_destroy(&children[4]);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
	}
	bson_append_array_end(&children[1], &children[2]);
	bson_destroy(&children[2]);
	bson_append_document_end(&children[0], &children[1]);
	bson_destroy(&children[1]);
	BSON_APPEND_DOCUMENT_BEGIN(&children[0], "left", &children[1]);
	BSON_APPEND_ARRAY_BEGIN(&children[1], "speed", &children[2]);
	for (int i = 0; i < (data->front_wheels_encoders.left.speed_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "0", &children[3]);
		BSON_APPEND_INT64(&children[3], "timestamp", data->front_wheels_encoders.left.speed[i].timestamp);
		BSON_APPEND_DOCUMENT_BEGIN(&children[3], "value", &children[4]);
		BSON_APPEND_DOUBLE(&children[4], "speed", data->front_wheels_encoders.left.speed[i].value.speed);
		BSON_APPEND_INT32(&children[4], "error_flag", data->front_wheels_encoders.left.speed[i].value.error_flag);
		bson_append_document_end(&children[3], &children[4]);
		bson_destroy(&children[4]);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
	}
	bson_append_array_end(&children[1], &children[2]);
	bson_destroy(&children[2]);
	BSON_APPEND_ARRAY_BEGIN(&children[1], "speed_rads", &children[2]);
	for (int i = 0; i < (data->front_wheels_encoders.left.speed_rads_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "0", &children[3]);
		BSON_APPEND_INT64(&children[3], "timestamp", data->front_wheels_encoders.left.speed_rads[i].timestamp);
		BSON_APPEND_DOUBLE(&children[3], "value", data->front_wheels_encoders.left.speed_rads[i].value);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
	}
	bson_append_array_end(&children[1], &children[2]);
	bson_destroy(&children[2]);
	BSON_APPEND_ARRAY_BEGIN(&children[1], "angle", &children[2]);
	for (int i = 0; i < (data->front_wheels_encoders.left.angle_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "0", &children[3]);
		BSON_APPEND_INT64(&children[3], "timestamp", data->front_wheels_encoders.left.angle[i].timestamp);
		BSON_APPEND_DOCUMENT_BEGIN(&children[3], "value", &children[4]);
		BSON_APPEND_DOUBLE(&children[4], "angle_0", data->front_wheels_encoders.left.angle[i].value.angle_0);
		BSON_APPEND_DOUBLE(&children[4], "angle_1", data->front_wheels_encoders.left.angle[i].value.angle_1);
		BSON_APPEND_DOUBLE(&children[4], "angle_delta", data->front_wheels_encoders.left.angle[i].value.angle_delta);
		bson_append_document_end(&children[3], &children[4]);
		bson_destroy(&children[4]);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
	}
	bson_append_array_end(&children[1], &children[2]);
	bson_destroy(&children[2]);
	bson_append_document_end(&children[0], &children[1]);
	bson_destroy(&children[1]);
	bson_append_document_end(*bson_document, &children[0]);
	bson_destroy(&children[0]);
	BSON_APPEND_ARRAY_BEGIN(*bson_document, "distance", &children[0]);
	for (int i = 0; i < (data->distance_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[0], "0", &children[1]);
		BSON_APPEND_INT64(&children[1], "timestamp", data->distance[i].timestamp);
		BSON_APPEND_DOCUMENT_BEGIN(&children[1], "value", &children[2]);
		BSON_APPEND_DOUBLE(&children[2], "meters", data->distance[i].value.meters);
		BSON_APPEND_DOUBLE(&children[2], "rotations", data->distance[i].value.rotations);
		BSON_APPEND_DOUBLE(&children[2], "angle", data->distance[i].value.angle);
		BSON_APPEND_DOUBLE(&children[2], "clock_period", data->distance[i].value.clock_period);
		bson_append_document_end(&children[1], &children[2]);
		bson_destroy(&children[2]);
		bson_append_document_end(&children[0], &children[1]);
		bson_destroy(&children[1]);
	}
	bson_append_array_end(*bson_document, &children[0]);
	bson_destroy(&children[0]);
	BSON_APPEND_DOCUMENT_BEGIN(*bson_document, "pedals", &children[0]);
	BSON_APPEND_ARRAY_BEGIN(&children[0], "throttle", &children[1]);
	for (int i = 0; i < (data->pedals.throttle_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[1], "0", &children[2]);
		BSON_APPEND_INT64(&children[2], "timestamp", data->pedals.throttle[i].timestamp);
		BSON_APPEND_INT32(&children[2], "value", data->pedals.throttle[i].value);
		bson_append_document_end(&children[1], &children[2]);
		bson_destroy(&children[2]);
	}
	bson_append_array_end(&children[0], &children[1]);
	bson_destroy(&children[1]);
	BSON_APPEND_ARRAY_BEGIN(&children[0], "brake", &children[1]);
	for (int i = 0; i < (data->pedals.brake_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[1], "0", &children[2]);
		BSON_APPEND_INT64(&children[2], "timestamp", data->pedals.brake[i].timestamp);
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "value", &children[3]);
		BSON_APPEND_INT32(&children[3], "is_breaking", data->pedals.brake[i].value.is_breaking);
		BSON_APPEND_DOUBLE(&children[3], "pressure_front", data->pedals.brake[i].value.pressure_front);
		BSON_APPEND_DOUBLE(&children[3], "pressure_back", data->pedals.brake[i].value.pressure_back);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
		bson_append_document_end(&children[1], &children[2]);
		bson_destroy(&children[2]);
	}
	bson_append_array_end(&children[0], &children[1]);
	bson_destroy(&children[1]);
	bson_append_document_end(*bson_document, &children[0]);
	bson_destroy(&children[0]);
	BSON_APPEND_DOCUMENT_BEGIN(*bson_document, "steering_wheel", &children[0]);
	BSON_APPEND_ARRAY_BEGIN(&children[0], "encoder", &children[1]);
	for (int i = 0; i < (data->steering_wheel.encoder_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[1], "0", &children[2]);
		BSON_APPEND_INT64(&children[2], "timestamp", data->steering_wheel.encoder[i].timestamp);
		BSON_APPEND_DOUBLE(&children[2], "value", data->steering_wheel.encoder[i].value);
		bson_append_document_end(&children[1], &children[2]);
		bson_destroy(&children[2]);
	}
	bson_append_array_end(&children[0], &children[1]);
	bson_destroy(&children[1]);
	BSON_APPEND_ARRAY_BEGIN(&children[0], "gears", &children[1]);
	for (int i = 0; i < (data->steering_wheel.gears_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[1], "0", &children[2]);
		BSON_APPEND_INT64(&children[2], "timestamp", data->steering_wheel.gears[i].timestamp);
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "value", &children[3]);
		BSON_APPEND_INT32(&children[3], "control", data->steering_wheel.gears[i].value.control);
		BSON_APPEND_INT32(&children[3], "cooling", data->steering_wheel.gears[i].value.cooling);
		BSON_APPEND_INT32(&children[3], "map", data->steering_wheel.gears[i].value.map);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
		bson_append_document_end(&children[1], &children[2]);
		bson_destroy(&children[2]);
	}
	bson_append_array_end(&children[0], &children[1]);
	bson_destroy(&children[1]);
	BSON_APPEND_INT32(&children[0], "marker", data->steering_wheel.marker);
	bson_append_document_end(*bson_document, &children[0]);
	bson_destroy(&children[0]);
	
}

void structureDelete(data_t *data) {
	free(data->inverters.right.speed);
	free(data->inverters.right.temperature_igbt);
	free(data->inverters.right.temperature_motors);
	free(data->inverters.right.torque);
	free(data->inverters.left.speed);
	free(data->inverters.left.temperature_igbt);
	free(data->inverters.left.temperature_motors);
	free(data->inverters.left.torque);
	free(data->bms_hv.temperature);
	free(data->bms_hv.voltage);
	free(data->bms_hv.current);
	free(data->bms_hv.errors);
	free(data->bms_hv.warnings);
	free(data->bms_lv.values);
	free(data->bms_lv.errors);
	free(data->gps.new.gga);
	free(data->gps.new.gll);
	free(data->gps.new.vtg);
	free(data->gps.new.rmc);
	free(data->gps.old.location);
	free(data->gps.old.time);
	free(data->gps.old.true_track_mode);
	free(data->imu_old.gyro);
	free(data->imu_old.accel);
	free(data->imu.gyro);
	free(data->imu.accel);
	free(data->front_wheels_encoders.right.speed);
	free(data->front_wheels_encoders.right.speed_rads);
	free(data->front_wheels_encoders.right.angle);
	free(data->front_wheels_encoders.left.speed);
	free(data->front_wheels_encoders.left.speed_rads);
	free(data->front_wheels_encoders.left.angle);
	free(data->distance);
	free(data->pedals.throttle);
	free(data->pedals.brake);
	free(data->steering_wheel.encoder);
	free(data->steering_wheel.gears);
	free(data);
	
}

void resetStructureId() {
    condition.structure.id = 0;
}

gather_code gatherStructure(data_t *document) {
    gather_code outcome = GATHER_KEEP;

    pthread_t can_thread, gps_thread;
    gather_can_args_t gather_can_args;
    gather_can_args.document = document;
    gather_can_args.outcome = &outcome;
    gather_gps_args_t gather_gps_args;
    gather_gps_args.document = document;

	pthread_create(&can_thread, NULL, gatherCan, &gather_can_args); 
    pthread_create(&gps_thread, NULL, gatherGps, &gather_gps_args);    
    pthread_join(can_thread, NULL);
    pthread_join(gps_thread, NULL);
    
    document->timestamp = getCurrentTimestamp();
    document->sessionName = condition.mongodb.instance->session_name;
    document->id = condition.structure.id++;

	return outcome;
}

/* INTERNAL FUNCTIONS DEFINITIONS */

static double parseNmeaCoord(double coord) {
    double temp = coord / 100;
    double left = floor(temp);
    double right = (temp - left) * (5 / 3);
    return (left + right);
}

static double getMillisecondsFromTimespec(struct timespec time) {
    return (double) (time.tv_sec * 1000 + 1.0E-6 * time.tv_nsec);
}

static long long int getCurrentTimestamp() {
	struct timeb timer_msec;
	if(!ftime(&timer_msec)) {
		return ((long long int) timer_msec.time) * 1000ll + ((long long int) timer_msec.millitm);
	}
	else {
		return -1;
	}
}

static void* gatherCan(void *args) {
    // Get arguments from void pointer
    gather_can_args_t* arguments = (gather_can_args_t*) args;
    data_t* document = arguments->document;
    gather_code* outcome = arguments->outcome;

    // Init timer variables
    struct timespec t_start, t_end;
    double start, end;
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    start = getMillisecondsFromTimespec(t_start);

    // Declare other used variables
    int id = 0, data_left, data_right, first_byte;
    int lon_done = 0, lat_done = 0;
    int byte_left, byte_right, temp;

    // Parse messages for x milliseconds
    do {
        // Read can
        canRead(&id, &data_left, &data_right);
		first_byte = ((data_left >> 24) & 255);

        // Parse message in base of the id
        switch(id) {

            case (INVERTER_RIGHT_ID):
                switch (first_byte)
                {
                    case INVERTER_SPEED_FB:
                        if (document->inverters.right.speed_count < document->inverters.right.speed_size) {
                            byte_left = (data_left >> 8) & 0x000000FF;
                            byte_right = (data_left >> 16) & 0x000000FF;
                            temp = byte_left * 256 + byte_right;
                            document->inverters.right.speed[document->inverters.right.speed_count].timestamp = getCurrentTimestamp();
                            document->inverters.right.speed[document->inverters.right.speed_count].value = (temp >= 32768 ? temp - 65536 : temp);
                            ++(document->inverters.right.speed_count);
                        }
                        break;

                    case INVERTER_TEMPERATURE_IGBT_FB:
                        if (document->inverters.right.temperature_igbt_count < document->inverters.right.temperature_igbt_size) {
                            byte_left = (data_left >> 8) & 0x000000FF;
                            byte_right = (data_left >> 16) & 0x000000FF;
                            temp = byte_left * 256 + byte_right;
                            document->inverters.right.temperature_igbt[document->inverters.right.temperature_igbt_count].timestamp = getCurrentTimestamp();
                            document->inverters.right.temperature_igbt[document->inverters.right.temperature_igbt_count].value = (temp >= 32768 ? temp - 65536 : temp);
                            ++(document->inverters.right.temperature_igbt_count);
                        }
                        break;

                    case INVERTER_TEMPERATURE_MOTORS_FB:
                        if (document->inverters.right.temperature_motors_count < document->inverters.right.temperature_motors_size) {
                            byte_left = (data_left >> 8) & 0x000000FF;
                            byte_right = (data_left >> 16) & 0x000000FF;
                            temp = byte_left * 256 + byte_right;
                            document->inverters.right.temperature_motors[document->inverters.right.temperature_motors_count].timestamp = getCurrentTimestamp();
                            document->inverters.right.temperature_motors[document->inverters.right.temperature_motors_count].value = (temp >= 32768 ? temp - 65536 : temp);
                            ++(document->inverters.right.temperature_motors_count);
                        }
                        break;
                    case INVERTER_TORQUE_FB:
                        if (document->inverters.right.torque_count < document->inverters.right.torque_size) {
                            byte_left = (data_left >> 8) & 0x000000FF;
                            byte_right = (data_left >> 16) & 0x000000FF;
                            temp = byte_left * 256 + byte_right;
                            document->inverters.right.torque[document->inverters.right.torque_count].timestamp = getCurrentTimestamp();
                            document->inverters.right.torque[document->inverters.right.torque_count].value = (temp >= 32768 ? temp - 65536 : temp);
                            ++(document->inverters.right.torque_count);
                        }
                        break;
                }
                break;

            case (INVERTER_LEFT_ID):
                switch (first_byte)
                {
                    case INVERTER_SPEED_FB: 
                        if (document->inverters.left.speed_count < document->inverters.left.speed_size) {
                            byte_left = (data_left >> 8) & 0x000000FF;
                            byte_right = (data_left >> 16) & 0x000000FF;
                            temp = byte_left * 256 + byte_right;
                            document->inverters.left.speed[document->inverters.left.speed_count].timestamp = getCurrentTimestamp();
                            document->inverters.left.speed[document->inverters.left.speed_count].value = (temp >= 32768 ? temp - 65536 : temp);
                            ++(document->inverters.left.speed_count);
                        }
                        break;

                    case INVERTER_TEMPERATURE_IGBT_FB:
                        if (document->inverters.left.temperature_igbt_count < document->inverters.left.temperature_igbt_size) {
                            byte_left = (data_left >> 8) & 0x000000FF;
                            byte_right = (data_left >> 16) & 0x000000FF;
                            temp = byte_left * 256 + byte_right;
                            document->inverters.left.temperature_igbt[document->inverters.left.temperature_igbt_count].timestamp = getCurrentTimestamp();
                            document->inverters.left.temperature_igbt[document->inverters.left.temperature_igbt_count].value = (temp >= 32768 ? temp - 65536 : temp);
                            ++(document->inverters.left.temperature_igbt_count);
                        }
                        break;

                    case INVERTER_TEMPERATURE_MOTORS_FB:
                        if (document->inverters.left.temperature_motors_count < document->inverters.left.temperature_motors_size) {
                            byte_left = (data_left >> 8) & 0x000000FF;
                            byte_right = (data_left >> 16) & 0x000000FF;
                            temp = byte_left * 256 + byte_right;
                            document->inverters.left.temperature_motors[document->inverters.left.temperature_motors_count].timestamp = getCurrentTimestamp();
                            document->inverters.left.temperature_motors[document->inverters.left.temperature_motors_count].value = (temp >= 32768 ? temp - 65536 : temp);
                            ++(document->inverters.left.temperature_motors_count);
                        }
                        break;
                    case INVERTER_TORQUE_FB:
                        if (document->inverters.left.torque_count < document->inverters.left.torque_size) {
                            byte_left = (data_left >> 8) & 0x000000FF;
                            byte_right = (data_left >> 16) & 0x000000FF;
                            temp = byte_left * 256 + byte_right;
                            document->inverters.left.torque[document->inverters.left.torque_count].timestamp = getCurrentTimestamp();
                            document->inverters.left.torque[document->inverters.left.torque_count].value = (temp >= 32768 ? temp - 65536 : temp);
                            ++(document->inverters.left.torque_count);
                        }
                        break;
                }
                break;

            case (BMS_HV_ID):
                switch (first_byte)
                {
                    case VOLTAGE_FB:
                        if (document->bms_hv.voltage_count < document->bms_hv.voltage_size) {
                            document->bms_hv.voltage[document->bms_hv.voltage_count].timestamp = getCurrentTimestamp();
                            document->bms_hv.voltage[document->bms_hv.voltage_count].value.total = (double)(data_left & 0x00FFFFFF) / 10000;
                            document->bms_hv.voltage[document->bms_hv.voltage_count].value.max = (double)((data_right >> 16) & 0x0000FFFF) / 10000;
                            document->bms_hv.voltage[document->bms_hv.voltage_count].value.min = (double)(data_right & 0x0000FFFF) / 10000;
                            ++(document->bms_hv.voltage_count);
                        }
                        break;

                    case TEMPERATURE_FB:
                        if (document->bms_hv.temperature_count < document->bms_hv.temperature_size) {
                            document->bms_hv.temperature[document->bms_hv.temperature_count].timestamp = getCurrentTimestamp();
                            document->bms_hv.temperature[document->bms_hv.temperature_count].value.average = ((data_left >> 8) & 0x0000FFFF) / 100;
                            document->bms_hv.temperature[document->bms_hv.temperature_count].value.max = (((data_left & 0x000000FF) * 256 + ((data_right >> 24) & 0x000000FF))) / 100;
                            document->bms_hv.temperature[document->bms_hv.temperature_count].value.min = ((data_right >> 8) & 0x0000FFFF) / 100;
                            ++(document->bms_hv.temperature_count);
                        }
                        break;

                    case CURRENT_FB:
                        if (document->bms_hv.current_count < document->bms_hv.current_size) {
                            document->bms_hv.current[document->bms_hv.current_count].timestamp = getCurrentTimestamp();
                            document->bms_hv.current[document->bms_hv.current_count].value.current = (double)((data_left >> 8) & 0x0000FFFF) / 10;
                            document->bms_hv.current[document->bms_hv.current_count].value.pow = (double)((data_left & 0x000000FF) * 256 + ((data_right >> 24) & 0x000000FF));
                            ++(document->bms_hv.current_count);
                        }
                        break;

                    case ERRORS_FB:
                        if (document->bms_hv.errors_count < document->bms_hv.errors_size) {
                            document->bms_hv.errors[document->bms_hv.errors_count].timestamp = getCurrentTimestamp();
                            document->bms_hv.errors[document->bms_hv.errors_count].value.fault_id = ((data_left >> 16) & 0x000000FF);
                            document->bms_hv.errors[document->bms_hv.errors_count].value.fault_index = ((data_left >> 8) & 0x000000FF) / 10;
                            ++(document->bms_hv.errors_count);
                        }
                        break;

                    case WARNINGS_FB:
                        if (document->bms_hv.warnings_count < document->bms_hv.warnings_size) {
                            document->bms_hv.warnings[document->bms_hv.warnings_count].timestamp = getCurrentTimestamp();
                            document->bms_hv.warnings[document->bms_hv.warnings_count].value.fault_id = ((data_left >> 16) & 0x000000FF);
                            document->bms_hv.warnings[document->bms_hv.warnings_count].value.fault_index = ((data_left >> 8) & 0x000000FF) / 10;
                            ++(document->bms_hv.warnings_count);
                        }
                        break;
                }
                break;

            case (PEDALS_ID):
                if (first_byte == THROTTLE_FB && document->pedals.throttle_count < document->pedals.throttle_size) {
                    document->pedals.throttle[document->pedals.throttle_count].timestamp = getCurrentTimestamp();
                    document->pedals.throttle[document->pedals.throttle_count].value = ((data_left >> 16) & 0x000000FF);

                    ++(document->pedals.throttle_count);
                }
                else if (first_byte == BRAKE_FB && document->pedals.brake_count < document->pedals.brake_size) {
                    document->pedals.brake[document->pedals.brake_count].timestamp = getCurrentTimestamp();
                    document->pedals.brake[document->pedals.brake_count].value.is_breaking = ((data_left >> 16) & 0x000000FF);
                    document->pedals.brake[document->pedals.brake_count].value.pressure_front = ((data_left & 0x0000FF00) + ((data_right >> 24) & 0x000000FF)) / 500;
                    document->pedals.brake[document->pedals.brake_count].value.pressure_back = (((data_right >> 8) & 0x0000FF00) + (data_right & 0x000000FF)) / 500;

                    ++(document->pedals.brake_count);
                }
                break;

            case (IMU_SWE_OLD_ID):
                switch (first_byte)
                {
                    case IMU_GYRO_FB:
                        if (document->imu_old.gyro_count < document->imu_old.gyro_size) {
                            document->imu_old.gyro[document->imu_old.gyro_count].timestamp = getCurrentTimestamp();
                            
                            document->imu_old.gyro[document->imu_old.gyro_count].value.x = (double)((data_left >> 8) & 0x0000FFFF);
                            document->imu_old.gyro[document->imu_old.gyro_count].value.y = (double)(((data_left & 0x000000FF) << 8) + ((data_right >> 24) & 0x000000FF));
                            document->imu_old.gyro[document->imu_old.gyro_count].value.z = (double)((data_right >> 8) & 0x0000FFFF);
                            document->imu_old.gyro[document->imu_old.gyro_count].value.scale = ((data_right)&0x000000FF) * 10;

                            document->imu_old.gyro[document->imu_old.gyro_count].value.x /= 10.0;
                            document->imu_old.gyro[document->imu_old.gyro_count].value.y /= 10.0;
                            document->imu_old.gyro[document->imu_old.gyro_count].value.z /= 10.0;

                            document->imu_old.gyro[document->imu_old.gyro_count].value.x -= document->imu_old.gyro[document->imu_old.gyro_count].value.scale;
                            document->imu_old.gyro[document->imu_old.gyro_count].value.y -= document->imu_old.gyro[document->imu_old.gyro_count].value.scale;
                            document->imu_old.gyro[document->imu_old.gyro_count].value.z -= document->imu_old.gyro[document->imu_old.gyro_count].value.scale;

                            ++(document->imu_old.gyro_count);
                        }
                        break;

                    case IMU_ACCEL_FB:
                        if (document->imu_old.accel_count < document->imu_old.accel_size) {
                            document->imu_old.accel[document->imu_old.accel_count].timestamp = getCurrentTimestamp();
                            document->imu_old.accel[document->imu_old.accel_count].value.x = (double)((data_left >> 8) & 0x0000FFFF);
                            document->imu_old.accel[document->imu_old.accel_count].value.y = (double)(((data_left & 0x000000FF) << 8) + ((data_right >> 24) & 0x000000FF));
                            document->imu_old.accel[document->imu_old.accel_count].value.z = (double)((data_right >> 8) & 0x0000FFFF);
                            document->imu_old.accel[document->imu_old.accel_count].value.scale = (data_right)&0x000000FF;

                            document->imu_old.accel[document->imu_old.accel_count].value.x /= 100.0;
                            document->imu_old.accel[document->imu_old.accel_count].value.y /= 100.0;
                            document->imu_old.accel[document->imu_old.accel_count].value.z /= 100.0;

                            document->imu_old.accel[document->imu_old.accel_count].value.x -= document->imu_old.accel[document->imu_old.accel_count].value.scale;
                            document->imu_old.accel[document->imu_old.accel_count].value.y -= document->imu_old.accel[document->imu_old.accel_count].value.scale;
                            document->imu_old.accel[document->imu_old.accel_count].value.z -= document->imu_old.accel[document->imu_old.accel_count].value.scale;

                            ++(document->imu_old.accel_count);
                        }
                        break;

                    case SWE_FB:
                        if (document->steering_wheel.encoder_count < document->steering_wheel.encoder_size) {
                            document->steering_wheel.encoder[document->steering_wheel.encoder_count].timestamp = getCurrentTimestamp();
                            document->steering_wheel.encoder[document->steering_wheel.encoder_count].value = ((data_left >> 16) & 255);
                            ++(document->steering_wheel.encoder_count);
                        }
                        break;
                }
                break;
                
            case (IMU_GYRO_ID):
                if (document->imu.gyro_count < document->imu.gyro_size) {
                    document->imu.gyro[document->imu.gyro_count].timestamp = getCurrentTimestamp();
                    temp = ((data_left >> 16) & 0x0000FFFF); 
                    document->imu.gyro[document->imu.gyro_count].value.x = (temp >= 32768 ? temp - 65536 : temp);

                    temp = (data_left & 0x0000FFFF);
                    document->imu.gyro[document->imu.gyro_count].value.y = (temp >= 32768 ? temp - 65536 : temp);
                    
                    temp = ((data_right >> 16) & 0x0000FFFF);
                    document->imu.gyro[document->imu.gyro_count].value.z = (temp >= 32768 ? temp - 65536 : temp);

                    document->imu.gyro[document->imu.gyro_count].value.x *= (245.0 / 65536.0);
                    document->imu.gyro[document->imu.gyro_count].value.y *= (245.0 / 65536.0);
                    document->imu.gyro[document->imu.gyro_count].value.z *= (245.0 / 65536.0);

                    ++(document->imu.gyro_count);
                }
                break;

            case (IMU_ACCEL_ID):
                if (document->imu.accel_count < document->imu.accel_size) {
                    document->imu.accel[document->imu.accel_count].timestamp = getCurrentTimestamp();
                    temp = ((data_left >> 16) & 0x0000FFFF);
                    document->imu.accel[document->imu.accel_count].value.x = (temp >= 32768 ? temp - 65536 : temp);

                    temp = (data_left & 0x0000FFFF);
                    document->imu.accel[document->imu.accel_count].value.y = (temp >= 32768 ? temp - 65536 : temp);
                    
                    temp = ((data_right >> 16) & 0x0000FFFF);
                    document->imu.accel[document->imu.accel_count].value.z = (temp >= 32768 ? temp - 65536 : temp);

                    document->imu.accel[document->imu.accel_count].value.x *= (8.0 / 65536.0) * 100;
                    document->imu.accel[document->imu.accel_count].value.y *= (8.0 / 65536.0) * 100;
                    document->imu.accel[document->imu.accel_count].value.z *= (8.0 / 65536.0) * 100;

                    ++(document->imu.accel_count);
                }
                break;

            case (FRONT_WHEELS_ENCODER_RIGHT_ID):
                switch (first_byte)
                {
                    case LATSPD_FB:
                        if (lat_done)
                        {
                            ++(document->gps.old.location_count);

                            if (document->gps.old.location_count < document->gps.old.location_size) {
                                document->gps.old.location[document->gps.old.location_count].timestamp = getCurrentTimestamp();
                                document->gps.old.location[document->gps.old.location_count].value.latitude_m = (double)(((((data_left >> 8) & 0x0000FFFF) << 8) * 10000) + (((data_left & 0x000000FF) * 0xFF) << 8) + ((data_right >> 24) & 0x000000FF)) / 10000.0;
                                document->gps.old.location[document->gps.old.location_count].value.latitude_o = (data_right >> 16) & 0x000000FF;
                                document->gps.old.location[document->gps.old.location_count].value.speed = data_right & 0x0000FFFF;

                                document->gps.old.location[document->gps.old.location_count].value.longitude_m = 0;
                                document->gps.old.location[document->gps.old.location_count].value.longitude_o = 0;
                                document->gps.old.location[document->gps.old.location_count].value.altitude = 0;
                                lat_done = 1;
                                lon_done = 0;
                            }
                        }
                        else if (lon_done)
                        {
                            if (abs(document->gps.old.location[document->gps.old.location_count].timestamp - getCurrentTimestamp()) < 25)
                            {
                                document->gps.old.location[document->gps.old.location_count].value.latitude_m = (double)(((((data_left >> 8) & 0x0000FFFF) << 8) * 10000) + (((data_left & 0x000000FF) * 0xFF) << 8) + ((data_right >> 24) & 0x000000FF)) / 10000.0;
                                document->gps.old.location[document->gps.old.location_count].value.latitude_o = (data_right >> 16) & 0x000000FF;
                                document->gps.old.location[document->gps.old.location_count].value.speed = data_right & 0x0000FFFF;

                                ++(document->gps.old.location_count);

                                lat_done = 0;
                                lon_done = 0;
                            }
                            else
                            {
                                ++(document->gps.old.location_count);

                                if (document->gps.old.location_count < document->gps.old.location_size) {
                                    document->gps.old.location[document->gps.old.location_count].timestamp = getCurrentTimestamp();
                                    document->gps.old.location[document->gps.old.location_count].value.latitude_m = (double)(((((data_left >> 8) & 0x0000FFFF) << 8) * 10000) + (((data_left & 0x000000FF) * 0xFF) << 8) + ((data_right >> 24) & 0x000000FF)) / 10000.0;
                                    document->gps.old.location[document->gps.old.location_count].value.latitude_o = (data_right >> 16) & 0x000000FF;
                                    document->gps.old.location[document->gps.old.location_count].value.speed = data_right & 0x0000FFFF;

                                    document->gps.old.location[document->gps.old.location_count].value.longitude_m = 0;
                                    document->gps.old.location[document->gps.old.location_count].value.longitude_o = 0;
                                    document->gps.old.location[document->gps.old.location_count].value.altitude = 0;

                                    lat_done = 1;
                                    lon_done = 0;
                                }
                            }
                        }
                        else {
                            if (document->gps.old.location_count < document->gps.old.location_size) {
                                document->gps.old.location[document->gps.old.location_count].timestamp = getCurrentTimestamp();
                                document->gps.old.location[document->gps.old.location_count].value.latitude_m = (double)(((((data_left >> 8) & 0x0000FFFF) << 8) * 10000) + (((data_left & 0x000000FF) * 0xFF) << 8) + ((data_right >> 24) & 0x000000FF)) / 10000.0;
                                document->gps.old.location[document->gps.old.location_count].value.latitude_o = (data_right >> 16) & 0x000000FF;
                                document->gps.old.location[document->gps.old.location_count].value.speed = data_right & 0x0000FFFF;

                                document->gps.old.location[document->gps.old.location_count].value.longitude_m = 0;
                                document->gps.old.location[document->gps.old.location_count].value.longitude_o = 0;
                                document->gps.old.location[document->gps.old.location_count].value.altitude = 0;

                                lat_done = 1;
                                lon_done = 0;
                            }
                        }
                        break;

                    case LONALT_FB:
                        if (lon_done)
                        {
                            ++(document->gps.old.location_count);

                            if (document->gps.old.location_count < document->gps.old.location_size) {
                                document->gps.old.location[document->gps.old.location_count].timestamp = getCurrentTimestamp();
                                document->gps.old.location[document->gps.old.location_count].value.longitude_m = (double)(((((data_left >> 8) & 0x0000FFFF) << 8) * 10000) + (((data_left & 0x000000FF) * 0xFF) << 8) + ((data_right >> 24) & 0x000000FF)) / 10000.0;
                                document->gps.old.location[document->gps.old.location_count].value.longitude_o = (data_right >> 16) & 0x000000FF;
                                document->gps.old.location[document->gps.old.location_count].value.altitude = data_right & 0x0000FFFF;

                                document->gps.old.location[document->gps.old.location_count].value.latitude_m = 0;
                                document->gps.old.location[document->gps.old.location_count].value.latitude_o = 0;
                                document->gps.old.location[document->gps.old.location_count].value.speed = 0;

                                lat_done = 0;
                                lon_done = 1;
                            }
                        }
                        else if (lat_done)
                        {
                            if (abs(document->gps.old.location[document->gps.old.location_count].timestamp - getCurrentTimestamp()) < 25)
                            {
                                document->gps.old.location[document->gps.old.location_count].value.longitude_m = (double)(((((data_left >> 8) & 0x0000FFFF) << 8) * 10000) + (((data_left & 0x000000FF) * 0xFF) << 8) + ((data_right >> 24) & 0x000000FF)) / 10000.0;
                                document->gps.old.location[document->gps.old.location_count].value.longitude_o = (data_right >> 16) & 0x000000FF;
                                document->gps.old.location[document->gps.old.location_count].value.altitude = data_right & 0x0000FFFF;

                                ++(document->gps.old.location_count);

                                lat_done = 0;
                                lon_done = 0;
                            }
                            else
                            {
                                ++(document->gps.old.location_count);

                                if (document->gps.old.location_count < document->gps.old.location_size) {
                                    document->gps.old.location[document->gps.old.location_count].timestamp = getCurrentTimestamp();
                                    document->gps.old.location[document->gps.old.location_count].value.longitude_m = (double)(((((data_left >> 8) & 0x0000FFFF) << 8) * 10000) + (((data_left & 0x000000FF) * 0xFF) << 8) + ((data_right >> 24) & 0x000000FF)) / 10000.0;
                                    document->gps.old.location[document->gps.old.location_count].value.longitude_o = (data_right >> 16) & 0x000000FF;
                                    document->gps.old.location[document->gps.old.location_count].value.altitude = data_right & 0x0000FFFF;

                                    document->gps.old.location[document->gps.old.location_count].value.latitude_m = 0;
                                    document->gps.old.location[document->gps.old.location_count].value.latitude_o = 0;
                                    document->gps.old.location[document->gps.old.location_count].value.speed = 0;

                                    lon_done = 1;
                                    lat_done = 0;
                                }
                            }
                        }
                        else {
                            if (document->gps.old.location_count < document->gps.old.location_size) {
                                document->gps.old.location[document->gps.old.location_count].timestamp = getCurrentTimestamp();
                                document->gps.old.location[document->gps.old.location_count].value.longitude_m = (double)(((((data_left >> 8) & 0x0000FFFF) << 8) * 10000) + (((data_left & 0x000000FF) * 0xFF) << 8) + ((data_right >> 24) & 0x000000FF)) / 10000.0;
                                document->gps.old.location[document->gps.old.location_count].value.longitude_o = (data_right >> 16) & 0x000000FF;
                                document->gps.old.location[document->gps.old.location_count].value.altitude = data_right & 0x0000FFFF;

                                document->gps.old.location[document->gps.old.location_count].value.latitude_m = 0;
                                document->gps.old.location[document->gps.old.location_count].value.latitude_o = 0;
                                document->gps.old.location[document->gps.old.location_count].value.speed = 0;

                                lon_done = 1;
                                lat_done = 0;
                            }
                        }
                        break;

                    case TIME_FB:
                        if (document->gps.old.time_count < document->gps.old.time_size) {
                            document->gps.old.time[document->gps.old.time_count].timestamp = getCurrentTimestamp();
                            document->gps.old.time[document->gps.old.time_count].value.hours = ((((data_left >> 16) & 0x000000FF) - 48) * 10) + (((data_left >> 8) & 0x000000FF) - 48);
                            document->gps.old.time[document->gps.old.time_count].value.minutes = (((data_left & 0x000000FF) - 48) * 10) + (((data_right >> 24) & 0x000000FF) - 48);
                            document->gps.old.time[document->gps.old.time_count].value.seconds = ((((data_right >> 16) & 0x000000FF) - 48) * 10) + (((data_right >> 8) & 0x000000FF) - 48);
                            ++(document->gps.old.time_count);
                        }
                        break;

                    case TTM_FB:
                        if (document->gps.old.true_track_mode_count < document->gps.old.true_track_mode_size) {
                            document->gps.old.true_track_mode[document->gps.old.true_track_mode_count].timestamp = getCurrentTimestamp();
                            document->gps.old.true_track_mode[document->gps.old.true_track_mode_count].value = (data_left >> 8) & 0x0000FFFF;
                            ++(document->gps.old.true_track_mode_count);
                        }
                        break;

                    case FRONT_WHEELS_FB_SPEED_FB:
                        if (document->front_wheels_encoders.right.speed_count < document->front_wheels_encoders.right.speed_size) {
                            document->front_wheels_encoders.right.speed[document->front_wheels_encoders.right.speed_count].timestamp = getCurrentTimestamp();
                            document->front_wheels_encoders.right.speed[document->front_wheels_encoders.right.speed_count].value.speed = ((data_left >> 8) & 0x0000FFFF) * ((data_left & 0x000000FF) == 0 ? 1 : -1);
                            document->front_wheels_encoders.right.speed[document->front_wheels_encoders.right.speed_count].value.error_flag = (data_right >> 8) & 0x000000FF;
                            ++(document->front_wheels_encoders.right.speed_count);
                        }
                        break;

                    case FRONT_WHEELS_FB_SPEED_RADS_FB:
                        if (document->front_wheels_encoders.right.speed_rads_count < document->front_wheels_encoders.right.speed_rads_size) {
                            document->front_wheels_encoders.right.speed_rads[document->front_wheels_encoders.right.speed_rads_count].timestamp = getCurrentTimestamp();
                            document->front_wheels_encoders.right.speed_rads[document->front_wheels_encoders.right.speed_rads_count].value = (data_left & 0x00FFFFFF) / 10000;
                            ++(document->front_wheels_encoders.right.speed_rads_count);
                        }
                        break;

                    case FRONT_WHEELS_FB_SPEED_ANGLE_FB:
                        if (document->front_wheels_encoders.right.angle_count < document->front_wheels_encoders.right.angle_size) { 
                            document->front_wheels_encoders.right.angle[document->front_wheels_encoders.right.angle_count].timestamp = getCurrentTimestamp();
                            document->front_wheels_encoders.right.angle[document->front_wheels_encoders.right.angle_count].value.angle_0 = ((data_left >> 8) & 0x0000FFFF) / 100;
                            document->front_wheels_encoders.right.angle[document->front_wheels_encoders.right.angle_count].value.angle_1 = (((data_left & 0x000000FF) * 0xFF) + ((data_right >> 24) & 0x000000FF)) / 100;
                            document->front_wheels_encoders.right.angle[document->front_wheels_encoders.right.angle_count].value.angle_delta = ((data_right >> 8) & 0x0000FFFF) / 100;
                            ++(document->front_wheels_encoders.right.angle_count);
                        }
                        break;

                    case DISTANCE_FB:
                        if (document->distance_count < document->distance_size) {
                            document->distance[document->distance_count].timestamp = getCurrentTimestamp();
                            document->distance[document->distance_count].value.meters = (data_left >> 8) & 0x0000FFFF;
                            document->distance[document->distance_count].value.rotations = ((data_left & 0x000000FF) * 0xFF) + ((data_right >> 24) & 0x000000FF);
                            document->distance[document->distance_count].value.angle = (data_right >> 16) & 0x000000F;
                            document->distance[document->distance_count].value.clock_period = (data_right >> 8) & 0x000000F;
                            ++(document->distance_count);
                        }
                        break;
                }
                break;

            case (FRONT_WHEELS_ENCODER_LEFT_ID):
                switch (first_byte)
                {
                    case FRONT_WHEELS_FB_SPEED_FB:
                        if (document->front_wheels_encoders.left.speed_count < document->front_wheels_encoders.left.speed_size) {
                            document->front_wheels_encoders.left.speed[document->front_wheels_encoders.left.speed_count].timestamp = getCurrentTimestamp();
                            document->front_wheels_encoders.left.speed[document->front_wheels_encoders.left.speed_count].value.speed = ((data_left >> 8) & 0x0000FFFF) * ((data_left & 0x000000FF) == 0 ? 1 : -1);
                            document->front_wheels_encoders.left.speed[document->front_wheels_encoders.left.speed_count].value.error_flag = (data_right >> 8) & 0x000000FF;
                            ++(document->front_wheels_encoders.left.speed_count);
                        }
                        break;

                    case FRONT_WHEELS_FB_SPEED_RADS_FB:
                        if (document->front_wheels_encoders.left.speed_rads_count < document->front_wheels_encoders.left.speed_rads_size) {
                            document->front_wheels_encoders.left.speed_rads[document->front_wheels_encoders.left.speed_rads_count].timestamp = getCurrentTimestamp();
                            document->front_wheels_encoders.left.speed_rads[document->front_wheels_encoders.left.speed_rads_count].value = (data_left & 0x00FFFFFF) / 10000;
                            ++(document->front_wheels_encoders.left.speed_rads_count);
                        }
                        break;

                    case FRONT_WHEELS_FB_SPEED_ANGLE_FB:
                        if (document->front_wheels_encoders.left.angle_count < document->front_wheels_encoders.left.angle_size) { 
                            document->front_wheels_encoders.left.angle[document->front_wheels_encoders.left.angle_count].timestamp = getCurrentTimestamp();
                            document->front_wheels_encoders.left.angle[document->front_wheels_encoders.left.angle_count].value.angle_0 = ((data_left >> 8) & 0x0000FFFF) / 100;
                            document->front_wheels_encoders.left.angle[document->front_wheels_encoders.left.angle_count].value.angle_1 = (((data_left & 0x000000FF) * 0xFF) + ((data_right >> 24) & 0x000000FF)) / 100;
                            document->front_wheels_encoders.left.angle[document->front_wheels_encoders.left.angle_count].value.angle_delta = ((data_right >> 8) & 0x0000FFFF) / 100;
                            ++(document->front_wheels_encoders.left.angle_count);
                        }
                        break;
                }
                break;

            case (BMS_LV_ID):
                if (document->bms_lv.values_count < document->bms_lv.values_size) {
                    document->bms_lv.values[document->bms_lv.values_count].timestamp = getCurrentTimestamp();
                    document->bms_lv.values[document->bms_lv.values_count].value.voltage = (double)((data_left >> 24) & 255) / 10.0;
                    document->bms_lv.values[document->bms_lv.values_count].value.temperature = (double)((data_left >> 8) & 255) / 5.0;
                    ++(document->bms_lv.values_count);
                }
                break;

            case (WHEEL_ID):
                if (first_byte == GEARS_FB)
                {
                    if (document->steering_wheel.gears_count < document->steering_wheel.gears_size) {
                        document->steering_wheel.gears[document->steering_wheel.gears_count].timestamp = getCurrentTimestamp();
                        document->steering_wheel.gears[document->steering_wheel.gears_count].value.control = (data_left >> 16) & 0xFF;
                        document->steering_wheel.gears[document->steering_wheel.gears_count].value.cooling = (data_left >> 8) & 0xFF;
                        document->steering_wheel.gears[document->steering_wheel.gears_count].value.map = (data_left)&0xFF;
                    }
                }
                else if (first_byte == MARKER_FB)
                {
                    document->steering_wheel.marker = 1;
                }
                else if (first_byte == SIGNAL_FB)
                {
                    int status = (data_left >> 16) & 0xFF;
                    int pilot_index = (data_left >> 8) & 0xFF;
                    int race_index = data_left & 0xFF;

                    switch (status) {
                        case 0:
                            *outcome = GATHER_IDLE;
                            break;
                        case 1:
                            if (pilot_index >= condition.session.pilots_count) {
                                logWarning("Error in structure: invalid pilot from wheel");
                                *outcome = GATHER_ERROR;
                            }
                            else if (race_index >= condition.session.races_count) {
                                logWarning("Error in structure: invalid race from wheel");
                                *outcome = GATHER_ERROR;
                            }
                            else {
                                condition.session.selected_pilot = pilot_index;
                                condition.session.selected_race = race_index;
                                *outcome = GATHER_ENABLE;
                            }
                            break;
                        default:
                            logWarning("Error in structure: invalid status from wheel");
                            *outcome = GATHER_ERROR;
                            break;
                    }
                }
                break;

        }

        // Get current milliesecond to check condition
        clock_gettime(CLOCK_MONOTONIC, &t_end);
        end = getMillisecondsFromTimespec(t_end);
    }
    while (end - start < condition.structure.sending_rate);

    // Return NULL
    return NULL;
}

static void* gatherGps(void *args) {
    // Execute only if GPS is plugged
    if (condition.gps.plugged) {

        // Get arguments from void pointer
        gather_gps_args_t* arguments = (gather_gps_args_t*) args;
        data_t* document = arguments->document;

        // Init timer variables
        struct timespec t_start, t_end;
        double start, end;
        clock_gettime(CLOCK_MONOTONIC, &t_start);
        start = getMillisecondsFromTimespec(t_start);

        gps_struct* gps_data;

        // Parse gps messages for x milliseconds
        do {
            gps_data = readGPS();
            if (gps_data != NULL) {

                if (gps_data->gga && gps_data->gga->status && document->gps.new.gga_count < document->gps.new.gga_size) {
                    document->gps.new.gga[document->gps.new.gga_count].timestamp = getCurrentTimestamp();

                    document->gps.new.gga[document->gps.new.gga_count].value.latitude_safe = gps_data->gga->latitude;
                    document->gps.new.gga[document->gps.new.gga_count].value.longitude_safe = gps_data->gga->longitude;
                    document->gps.new.gga[document->gps.new.gga_count].value.latitude = parseNmeaCoord(gps_data->gga->latitude);
                    document->gps.new.gga[document->gps.new.gga_count].value.longitude = parseNmeaCoord(gps_data->gga->longitude);
                    document->gps.new.gga[document->gps.new.gga_count].value.altitude = gps_data->gga->altitude;
                    document->gps.new.gga[document->gps.new.gga_count].value.ns_indicator = gps_data->gga->ns_indicator;
                    document->gps.new.gga[document->gps.new.gga_count].value.ew_indicator = gps_data->gga->ew_indicator;
                    document->gps.new.gga[document->gps.new.gga_count].value.utc_time = gps_data->gga->utc_time;

                    ++(document->gps.new.gga_count);
                }

                if (gps_data->gll && gps_data->gll->status && document->gps.new.gll_count < document->gps.new.gll_size) {
                    document->gps.new.gll[document->gps.new.gll_count].timestamp = getCurrentTimestamp();

                    document->gps.new.gll[document->gps.new.gll_count].value.latitude = parseNmeaCoord(gps_data->gll->latitude);
                    document->gps.new.gll[document->gps.new.gll_count].value.longitude = parseNmeaCoord(gps_data->gll->longitude);
                    document->gps.new.gll[document->gps.new.gll_count].value.ns_indicator = gps_data->gll->ns_indicator;
                    document->gps.new.gll[document->gps.new.gll_count].value.ew_indicator = gps_data->gll->ew_indicator;
                    document->gps.new.gll[document->gps.new.gll_count].value.utc_time = gps_data->gll->utc_time;

                    ++(document->gps.new.gll_count);
                }

                if (gps_data->vtg && document->gps.new.vtg_count < document->gps.new.vtg_size) {
                    document->gps.new.vtg[document->gps.new.vtg_count].timestamp = getCurrentTimestamp();

                    document->gps.new.vtg[document->gps.new.vtg_count].value.ground_speed_knots = gps_data->vtg->ground_speed_knots;
                    document->gps.new.vtg[document->gps.new.vtg_count].value.ground_speed_human = gps_data->vtg->ground_speed_human;

                    ++(document->gps.new.vtg_count);
                }

                if (gps_data->rmc && gps_data->rmc->status && document->gps.new.rmc_count < document->gps.new.rmc_size) {
                    document->gps.new.rmc[document->gps.new.rmc_count].timestamp = getCurrentTimestamp();

                    document->gps.new.rmc[document->gps.new.rmc_count].value.latitude = parseNmeaCoord(gps_data->rmc->latitude);
                    document->gps.new.rmc[document->gps.new.rmc_count].value.longitude = parseNmeaCoord(gps_data->rmc->longitude);
                    document->gps.new.rmc[document->gps.new.rmc_count].value.ns_indicator = gps_data->rmc->ns_indicator;
                    document->gps.new.rmc[document->gps.new.rmc_count].value.ew_indicator = gps_data->rmc->ew_indicator;
                    document->gps.new.rmc[document->gps.new.rmc_count].value.utc_time = gps_data->rmc->utc_time;
                    document->gps.new.rmc[document->gps.new.rmc_count].value.date = gps_data->rmc->date;
                    document->gps.new.rmc[document->gps.new.rmc_count].value.ground_speed_knots = gps_data->rmc->ground_speed_knots;

                    ++(document->gps.new.rmc_count);
                } 
            }
            gpsFree(gps_data);

            // Get current milliesecond to check condition
            clock_gettime(CLOCK_MONOTONIC, &t_end);
            end = getMillisecondsFromTimespec(t_end);
        }
        while (end - start < condition.structure.sending_rate);

    }

    // Return NULL
    return NULL;
}