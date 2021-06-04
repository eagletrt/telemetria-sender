#include "gather_utils.h"

data_t* gatherCreateData() {
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
	data->gps.gga_size = 500;
	data->gps.gga = (gps_gga_data*) malloc(sizeof(gps_gga_data) * data->gps.gga_size);
	data->gps.gga_count = 0;
	data->gps.gll_size = 500;
	data->gps.gll = (gps_gll_data*) malloc(sizeof(gps_gll_data) * data->gps.gll_size);
	data->gps.gll_count = 0;
	data->gps.vtg_size = 500;
	data->gps.vtg = (gps_vtg_data*) malloc(sizeof(gps_vtg_data) * data->gps.vtg_size);
	data->gps.vtg_count = 0;
	data->gps.rmc_size = 500;
	data->gps.rmc = (gps_rmc_data*) malloc(sizeof(gps_rmc_data) * data->gps.rmc_size);
	data->gps.rmc_count = 0;
	data->laps.laps_size = 500;
	data->laps.laps = (laps_laps_data*) malloc(sizeof(laps_laps_data) * data->laps.laps_size);
	data->laps.laps_count = 0;
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
	data->encoders.average_speed_size = 500;
	data->encoders.average_speed = (encoders_average_speed_data*) malloc(sizeof(encoders_average_speed_data) * data->encoders.average_speed_size);
	data->encoders.average_speed_count = 0;
	data->encoders.right_rads_size = 500;
	data->encoders.right_rads = (encoders_right_rads_data*) malloc(sizeof(encoders_right_rads_data) * data->encoders.right_rads_size);
	data->encoders.right_rads_count = 0;
	data->encoders.rotations_and_km_size = 500;
	data->encoders.rotations_and_km = (encoders_rotations_and_km_data*) malloc(sizeof(encoders_rotations_and_km_data) * data->encoders.rotations_and_km_size);
	data->encoders.rotations_and_km_count = 0;
	data->encoders.speed_kmh_size = 500;
	data->encoders.speed_kmh = (encoders_speed_kmh_data*) malloc(sizeof(encoders_speed_kmh_data) * data->encoders.speed_kmh_size);
	data->encoders.speed_kmh_count = 0;
	data->encoders.angles_size = 500;
	data->encoders.angles = (encoders_angles_data*) malloc(sizeof(encoders_angles_data) * data->encoders.angles_size);
	data->encoders.angles_count = 0;
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

void gatherDeleteData(data_t *data) {
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
	free(data->gps.gga);
	free(data->gps.gll);
	free(data->gps.vtg);
	free(data->gps.rmc);
	free(data->laps.laps);
	free(data->imu_old.gyro);
	free(data->imu_old.accel);
	free(data->imu.gyro);
	free(data->imu.accel);
	free(data->encoders.average_speed);
	free(data->encoders.right_rads);
	free(data->encoders.rotations_and_km);
	free(data->encoders.speed_kmh);
	free(data->encoders.angles);
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

void gatherDataToBson(data_t *data, bson_t** bson_document) {
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
	BSON_APPEND_ARRAY_BEGIN(&children[0], "gga", &children[1]);
	for (int i = 0; i < (data->gps.gga_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[1], "0", &children[2]);
		BSON_APPEND_INT64(&children[2], "timestamp", data->gps.gga[i].timestamp);
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "value", &children[3]);
		BSON_APPEND_DOUBLE(&children[3], "latitude_safe", data->gps.gga[i].value.latitude_safe);
		BSON_APPEND_DOUBLE(&children[3], "longitude_safe", data->gps.gga[i].value.longitude_safe);
		BSON_APPEND_DOUBLE(&children[3], "latitude", data->gps.gga[i].value.latitude);
		BSON_APPEND_DOUBLE(&children[3], "longitude", data->gps.gga[i].value.longitude);
		BSON_APPEND_DOUBLE(&children[3], "altitude", data->gps.gga[i].value.altitude);
		BSON_APPEND_UTF8(&children[3], "ns_indicator", data->gps.gga[i].value.ns_indicator);
		BSON_APPEND_UTF8(&children[3], "ew_indicator", data->gps.gga[i].value.ew_indicator);
		BSON_APPEND_UTF8(&children[3], "utc_time", data->gps.gga[i].value.utc_time);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
		bson_append_document_end(&children[1], &children[2]);
		bson_destroy(&children[2]);
	}
	bson_append_array_end(&children[0], &children[1]);
	bson_destroy(&children[1]);
	BSON_APPEND_ARRAY_BEGIN(&children[0], "gll", &children[1]);
	for (int i = 0; i < (data->gps.gll_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[1], "0", &children[2]);
		BSON_APPEND_INT64(&children[2], "timestamp", data->gps.gll[i].timestamp);
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "value", &children[3]);
		BSON_APPEND_DOUBLE(&children[3], "latitude", data->gps.gll[i].value.latitude);
		BSON_APPEND_DOUBLE(&children[3], "longitude", data->gps.gll[i].value.longitude);
		BSON_APPEND_UTF8(&children[3], "ns_indicator", data->gps.gll[i].value.ns_indicator);
		BSON_APPEND_UTF8(&children[3], "ew_indicator", data->gps.gll[i].value.ew_indicator);
		BSON_APPEND_UTF8(&children[3], "utc_time", data->gps.gll[i].value.utc_time);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
		bson_append_document_end(&children[1], &children[2]);
		bson_destroy(&children[2]);
	}
	bson_append_array_end(&children[0], &children[1]);
	bson_destroy(&children[1]);
	BSON_APPEND_ARRAY_BEGIN(&children[0], "vtg", &children[1]);
	for (int i = 0; i < (data->gps.vtg_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[1], "0", &children[2]);
		BSON_APPEND_INT64(&children[2], "timestamp", data->gps.vtg[i].timestamp);
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "value", &children[3]);
		BSON_APPEND_DOUBLE(&children[3], "ground_speed_knots", data->gps.vtg[i].value.ground_speed_knots);
		BSON_APPEND_DOUBLE(&children[3], "ground_speed_human", data->gps.vtg[i].value.ground_speed_human);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
		bson_append_document_end(&children[1], &children[2]);
		bson_destroy(&children[2]);
	}
	bson_append_array_end(&children[0], &children[1]);
	bson_destroy(&children[1]);
	BSON_APPEND_ARRAY_BEGIN(&children[0], "rmc", &children[1]);
	for (int i = 0; i < (data->gps.rmc_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[1], "0", &children[2]);
		BSON_APPEND_INT64(&children[2], "timestamp", data->gps.rmc[i].timestamp);
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "value", &children[3]);
		BSON_APPEND_DOUBLE(&children[3], "latitude", data->gps.rmc[i].value.latitude);
		BSON_APPEND_DOUBLE(&children[3], "longitude", data->gps.rmc[i].value.longitude);
		BSON_APPEND_UTF8(&children[3], "ns_indicator", data->gps.rmc[i].value.ns_indicator);
		BSON_APPEND_UTF8(&children[3], "ew_indicator", data->gps.rmc[i].value.ew_indicator);
		BSON_APPEND_UTF8(&children[3], "utc_time", data->gps.rmc[i].value.utc_time);
		BSON_APPEND_UTF8(&children[3], "date", data->gps.rmc[i].value.date);
		BSON_APPEND_DOUBLE(&children[3], "ground_speed_knots", data->gps.rmc[i].value.ground_speed_knots);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
		bson_append_document_end(&children[1], &children[2]);
		bson_destroy(&children[2]);
	}
	bson_append_array_end(&children[0], &children[1]);
	bson_destroy(&children[1]);
	bson_append_document_end(*bson_document, &children[0]);
	bson_destroy(&children[0]);
	BSON_APPEND_DOCUMENT_BEGIN(*bson_document, "laps", &children[0]);
	BSON_APPEND_ARRAY_BEGIN(&children[0], "laps", &children[1]);
	for (int i = 0; i < (data->laps.laps_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[1], "0", &children[2]);
		BSON_APPEND_INT64(&children[2], "timestamp", data->laps.laps[i].timestamp);
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "value", &children[3]);
		BSON_APPEND_INT32(&children[3], "lap", data->laps.laps[i].value.lap);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
		bson_append_document_end(&children[1], &children[2]);
		bson_destroy(&children[2]);
	}
	bson_append_array_end(&children[0], &children[1]);
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
	BSON_APPEND_DOCUMENT_BEGIN(*bson_document, "encoders", &children[0]);
	BSON_APPEND_ARRAY_BEGIN(&children[0], "average_speed", &children[1]);
	for (int i = 0; i < (data->encoders.average_speed_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[1], "0", &children[2]);
		BSON_APPEND_INT64(&children[2], "timestamp", data->encoders.average_speed[i].timestamp);
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "value", &children[3]);
		BSON_APPEND_INT32(&children[3], "left", data->encoders.average_speed[i].value.left);
		BSON_APPEND_INT32(&children[3], "right", data->encoders.average_speed[i].value.right);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
		bson_append_document_end(&children[1], &children[2]);
		bson_destroy(&children[2]);
	}
	bson_append_array_end(&children[0], &children[1]);
	bson_destroy(&children[1]);
	BSON_APPEND_ARRAY_BEGIN(&children[0], "right_rads", &children[1]);
	for (int i = 0; i < (data->encoders.right_rads_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[1], "0", &children[2]);
		BSON_APPEND_INT64(&children[2], "timestamp", data->encoders.right_rads[i].timestamp);
		BSON_APPEND_INT32(&children[2], "value", data->encoders.right_rads[i].value);
		bson_append_document_end(&children[1], &children[2]);
		bson_destroy(&children[2]);
	}
	bson_append_array_end(&children[0], &children[1]);
	bson_destroy(&children[1]);
	BSON_APPEND_ARRAY_BEGIN(&children[0], "rotations_and_km", &children[1]);
	for (int i = 0; i < (data->encoders.rotations_and_km_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[1], "0", &children[2]);
		BSON_APPEND_INT64(&children[2], "timestamp", data->encoders.rotations_and_km[i].timestamp);
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "value", &children[3]);
		BSON_APPEND_INT32(&children[3], "rotation", data->encoders.rotations_and_km[i].value.rotation);
		BSON_APPEND_INT32(&children[3], "km", data->encoders.rotations_and_km[i].value.km);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
		bson_append_document_end(&children[1], &children[2]);
		bson_destroy(&children[2]);
	}
	bson_append_array_end(&children[0], &children[1]);
	bson_destroy(&children[1]);
	BSON_APPEND_ARRAY_BEGIN(&children[0], "speed_kmh", &children[1]);
	for (int i = 0; i < (data->encoders.speed_kmh_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[1], "0", &children[2]);
		BSON_APPEND_INT64(&children[2], "timestamp", data->encoders.speed_kmh[i].timestamp);
		BSON_APPEND_DOUBLE(&children[2], "value", data->encoders.speed_kmh[i].value);
		bson_append_document_end(&children[1], &children[2]);
		bson_destroy(&children[2]);
	}
	bson_append_array_end(&children[0], &children[1]);
	bson_destroy(&children[1]);
	BSON_APPEND_ARRAY_BEGIN(&children[0], "angles", &children[1]);
	for (int i = 0; i < (data->encoders.angles_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[1], "0", &children[2]);
		BSON_APPEND_INT64(&children[2], "timestamp", data->encoders.angles[i].timestamp);
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "value", &children[3]);
		BSON_APPEND_DOUBLE(&children[3], "la", data->encoders.angles[i].value.la);
		BSON_APPEND_DOUBLE(&children[3], "ra", data->encoders.angles[i].value.ra);
		BSON_APPEND_DOUBLE(&children[3], "da", data->encoders.angles[i].value.da);
		BSON_APPEND_INT32(&children[3], "frequencyLeft", data->encoders.angles[i].value.frequencyLeft);
		BSON_APPEND_INT32(&children[3], "frequencyRight", data->encoders.angles[i].value.frequencyRight);
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