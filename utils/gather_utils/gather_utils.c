#include "gather_utils.h"

data_t* gatherCreateData() {
	data_t* data = (data_t*) malloc(sizeof(data_t));
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
	data->imu.acceleration_size = 500;
	data->imu.acceleration = (imu_acceleration_data*) malloc(sizeof(imu_acceleration_data) * data->imu.acceleration_size);
	data->imu.acceleration_count = 0;
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
	
	return data;
}

void gatherDeleteData(data_t *data) {
	free(data->bms_hv.temperature);
	free(data->bms_hv.voltage);
	free(data->bms_hv.current);
	free(data->bms_hv.errors);
	free(data->imu.acceleration);
	free(data->gps.gga);
	free(data->gps.gll);
	free(data->gps.vtg);
	free(data->gps.rmc);
	free(data);
	
}

void gatherDataToBson(data_t *data, bson_t** bson_document) {
	*bson_document = bson_new();
	bson_t *children = (bson_t*)malloc(sizeof(bson_t) * 4);
	BSON_APPEND_INT32(*bson_document, "id", data->id);
	BSON_APPEND_INT64(*bson_document, "timestamp", data->timestamp);
	BSON_APPEND_UTF8(*bson_document, "sessionName", data->sessionName);
	BSON_APPEND_DOCUMENT_BEGIN(*bson_document, "bms_hv", &children[0]);
	BSON_APPEND_ARRAY_BEGIN(&children[0], "temperature", &children[1]);
	for (int i = 0; i < (data->bms_hv.temperature_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[1], "0", &children[2]);
		BSON_APPEND_INT64(&children[2], "timestamp", data->bms_hv.temperature[i].timestamp);
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "value", &children[3]);
		BSON_APPEND_INT32(&children[3], "average_temp", data->bms_hv.temperature[i].value.average_temp);
		BSON_APPEND_INT32(&children[3], "max_temp", data->bms_hv.temperature[i].value.max_temp);
		BSON_APPEND_INT32(&children[3], "min_temp", data->bms_hv.temperature[i].value.min_temp);
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
		BSON_APPEND_INT32(&children[3], "pack_voltage", data->bms_hv.voltage[i].value.pack_voltage);
		BSON_APPEND_INT32(&children[3], "bus_voltage", data->bms_hv.voltage[i].value.bus_voltage);
		BSON_APPEND_INT32(&children[3], "min_cell_voltage", data->bms_hv.voltage[i].value.min_cell_voltage);
		BSON_APPEND_INT32(&children[3], "max_cell_voltage", data->bms_hv.voltage[i].value.max_cell_voltage);
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
		BSON_APPEND_INT32(&children[3], "current", data->bms_hv.current[i].value.current);
		BSON_APPEND_INT32(&children[3], "power", data->bms_hv.current[i].value.power);
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
		BSON_APPEND_INT32(&children[3], "error_code", data->bms_hv.errors[i].value.error_code);
		BSON_APPEND_INT32(&children[3], "error_index", data->bms_hv.errors[i].value.error_index);
		BSON_APPEND_INT32(&children[3], "active", data->bms_hv.errors[i].value.active);
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
	BSON_APPEND_ARRAY_BEGIN(&children[0], "acceleration", &children[1]);
	for (int i = 0; i < (data->imu.acceleration_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[1], "0", &children[2]);
		BSON_APPEND_INT64(&children[2], "timestamp", data->imu.acceleration[i].timestamp);
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "value", &children[3]);
		BSON_APPEND_INT32(&children[3], "accel_x", data->imu.acceleration[i].value.accel_x);
		BSON_APPEND_INT32(&children[3], "accel_y", data->imu.acceleration[i].value.accel_y);
		BSON_APPEND_INT32(&children[3], "accel_z", data->imu.acceleration[i].value.accel_z);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
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
	
}