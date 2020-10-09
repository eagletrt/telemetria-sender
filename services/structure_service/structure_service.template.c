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
	// {{GENERATE_STRUCTURE_ALLOCATOR}}
	return data;
}

void structureToBson(data_t *data, bson_t** bson_document) {
	//{{GENERATE_BSON}}
}

void structureDelete(data_t *data) {
	//{{GENERATE_STRUCTURE_DEALLOCATOR}}
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

static void resetGpsElement(data_t *document, int index) {
    document->gps.new[index].timestamp = getCurrentTimestamp();
    document->gps.new[index].value.latitude_GGA_safe = 0.0;
    document->gps.new[index].value.longitude_GGA_safe = 0.0;
    document->gps.new[index].value.latitude_GGA = 0.0;
    document->gps.new[index].value.longitude_GGA = 0.0;
    document->gps.new[index].value.altitude_GGA = 0.0;
    document->gps.new[index].value.ns_indicator_GGA = "";
    document->gps.new[index].value.ew_indicator_GGA = "";
    document->gps.new[index].value.utc_time_GGA = "";
    document->gps.new[index].value.latitude_GLL = 0.0;
    document->gps.new[index].value.longitude_GLL= 0.0;
    document->gps.new[index].value.ns_indicator_GLL = "";
    document->gps.new[index].value.ew_indicator_GLL = "";
    document->gps.new[index].value.utc_time_GLL = "";
    document->gps.new[index].value.ground_speed_human_VTG = 0.0;
    document->gps.new[index].value.ground_speed_knots_VTG = 0.0;
    document->gps.new[index].value.latitude_RMC = 0.0;
    document->gps.new[index].value.longitude_RMC = 0.0;
    document->gps.new[index].value.ns_indicator_RMC = "";
    document->gps.new[index].value.ew_indicator_RMC = "";
    document->gps.new[index].value.utc_time_RMC = "";
    document->gps.new[index].value.date_RMC = "";
    document->gps.new[index].value.ground_speed_knots_RMC = 0.0;
}

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
                    case INVERTER_RIGHT_SPEED_FB:
                        if (document->inverters.right.speed_count < document->inverters.right.speed_size) {
                            byte_left = (data_left >> 8) & 0x000000FF;
                            byte_right = (data_left >> 16) & 0x000000FF;
                            temp = byte_left * 256 + byte_right;
                            document->inverters.right.speed[document->inverters.right.speed_count].timestamp = getCurrentTimestamp();
                            document->inverters.right.speed[document->inverters.right.speed_count++].value = (temp >= 32768 ? temp - 65536 : temp);
                        }
                        break;

                    case INVERTER_RIGHT_TEMPERATURE_IGBT_FB:
                        if (document->inverters.right.temperature_igbt_count < document->inverters.right.temperature_igbt_size) {
                            byte_left = (data_left >> 8) & 0x000000FF;
                            byte_right = (data_left >> 16) & 0x000000FF;
                            temp = byte_left * 256 + byte_right;
                            document->inverters.right.temperature_igbt[document->inverters.right.temperature_igbt_count].timestamp = getCurrentTimestamp();
                            document->inverters.right.temperature_igbt[document->inverters.right.temperature_igbt_count++].value = (temp >= 32768 ? temp - 65536 : temp);
                        }
                        break;

                    case INVERTER_RIGHT_TEMPERATURE_MOTORS_FB:
                    if (document->inverters.right.temperature_motors_count < document->inverters.right.temperature_motors_size) {
                        byte_left = (data_left >> 8) & 0x000000FF;
                        byte_right = (data_left >> 16) & 0x000000FF;
                        temp = byte_left * 256 + byte_right;
                        document->inverters.right.temperature_motors[document->inverters.right.temperature_motors_count].timestamp = getCurrentTimestamp();
                        document->inverters.right.temperature_motors[document->inverters.right.temperature_motors_count++].value = (temp >= 32768 ? temp - 65536 : temp);
                    }
                    break;
                }
                break;

            case (INVERTER_LEFT_ID):
                switch (first_byte)
                {
                    case INVERTER_LEFT_SPEED_FB: 
                        if (document->inverters.left.speed_count < document->inverters.left.speed_size) {
                            byte_left = (data_left >> 8) & 0x000000FF;
                            byte_right = (data_left >> 16) & 0x000000FF;
                            temp = byte_left * 256 + byte_right;
                            document->inverters.left.speed[document->inverters.left.speed_count].timestamp = getCurrentTimestamp();
                            document->inverters.left.speed[document->inverters.left.speed_count++].value = (temp >= 32768 ? temp - 65536 : temp);
                        }
                        break;

                    case INVERTER_LEFT_TEMPERATURE_IGBT_FB:
                        if (document->inverters.left.temperature_igbt_count < document->inverters.left.temperature_igbt_size) {
                            byte_left = (data_left >> 8) & 0x000000FF;
                            byte_right = (data_left >> 16) & 0x000000FF;
                            temp = byte_left * 256 + byte_right;
                            document->inverters.left.temperature_igbt[document->inverters.left.temperature_igbt_count].timestamp = getCurrentTimestamp();
                            document->inverters.left.temperature_igbt[document->inverters.left.temperature_igbt_count++].value = (temp >= 32768 ? temp - 65536 : temp);
                        }
                        break;

                    case INVERTER_LEFT_TEMPERATURE_MOTORS_FB:
                        if (document->inverters.left.temperature_motors_count < document->inverters.left.temperature_motors_size) {
                            byte_left = (data_left >> 8) & 0x000000FF;
                            byte_right = (data_left >> 16) & 0x000000FF;
                            temp = byte_left * 256 + byte_right;
                            document->inverters.left.temperature_motors[document->inverters.left.temperature_motors_count].timestamp = getCurrentTimestamp();
                            document->inverters.left.temperature_motors[document->inverters.left.temperature_motors_count++].value = (temp >= 32768 ? temp - 65536 : temp);
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
                            document->bms_hv.voltage[document->bms_hv.voltage_count++].value.min = (double)(data_right & 0x0000FFFF) / 10000;
                        }
                        break;

                    case TEMPERATURE_FB:
                        if (document->bms_hv.temperature_count < document->bms_hv.temperature_size) {
                            document->bms_hv.temperature[document->bms_hv.temperature_count].timestamp = getCurrentTimestamp();
                            document->bms_hv.temperature[document->bms_hv.temperature_count].value.average = ((data_left >> 8) & 0x0000FFFF) / 100;
                            document->bms_hv.temperature[document->bms_hv.temperature_count].value.max = (((data_left & 0x000000FF) * 256 + ((data_right >> 24) & 0x000000FF))) / 100;
                            document->bms_hv.temperature[document->bms_hv.temperature_count++].value.min = ((data_right >> 8) & 0x0000FFFF) / 100;
                        }
                        break;

                    case CURRENT_FB:
                        if (document->bms_hv.current_count < document->bms_hv.current_size) {
                            document->bms_hv.current[document->bms_hv.current_count].timestamp = getCurrentTimestamp();
                            document->bms_hv.current[document->bms_hv.current_count].value.current = (double)((data_left >> 8) & 0x0000FFFF) / 10;
                            document->bms_hv.current[document->bms_hv.current_count++].value.pow = (double)((data_left & 0x000000FF) * 256 + ((data_right >> 24) & 0x000000FF));
                        }
                        break;

                    case ERRORS_FB:
                        if (document->bms_hv.errors_count < document->bms_hv.errors_size) {
                            document->bms_hv.errors[document->bms_hv.errors_count].timestamp = getCurrentTimestamp();
                            document->bms_hv.errors[document->bms_hv.errors_count].value.fault_id = ((data_left >> 16) & 0x000000FF);
                            document->bms_hv.errors[document->bms_hv.errors_count++].value.fault_index = ((data_left >> 8) & 0x000000FF) / 10;
                        }
                        break;

                    case WARNINGS_FB:
                        if (document->bms_hv.warnings_count < document->bms_hv.warnings_size) {
                            document->bms_hv.warnings[document->bms_hv.warnings_count].timestamp = getCurrentTimestamp();
                            document->bms_hv.warnings[document->bms_hv.warnings_count].value.fault_id = ((data_left >> 16) & 0x000000FF);
                            document->bms_hv.warnings[document->bms_hv.warnings_count++].value.fault_index = ((data_left >> 8) & 0x000000FF) / 10;
                        }
                        break;
                }
                break;

            case (PEDALS_ID):
                if (first_byte == THROTTLE_FB && document->throttle_count < document->throttle_size) {
                    document->throttle[document->throttle_count].timestamp = getCurrentTimestamp();
                    document->throttle[document->throttle_count].value = ((data_left >> 16) & 255);

                    document->throttle_count++;
                }
                else if (first_byte == BRAKE_FB && document->brake_count < document->brake_size) {
                    document->brake[document->brake_count].timestamp = getCurrentTimestamp();
                    document->brake[document->brake_count].value = ((data_left >> 16) & 255);

                    document->brake_count++;
                }
                break;

            case (IMU_SWE_ID):
                switch (first_byte)
                {
                    case IMU_GYRO_FB:
                        if (document->imu_gyro_count < document->imu_gyro_size) {
                            document->imu_gyro[document->imu_gyro_count].timestamp = getCurrentTimestamp();
                            document->imu_gyro[document->imu_gyro_count].value.x = (double)((data_left >> 8) & 0x0000FFFF);
                            document->imu_gyro[document->imu_gyro_count].value.y = (double)((data_left & 0x000000FF) * 0xFF) + ((data_right >> 24) & 0x000000FF);
                            document->imu_gyro[document->imu_gyro_count].value.z = (double)((data_right >> 8) & 0x0000FFFF);
                            document->imu_gyro[document->imu_gyro_count].value.scale = ((data_right)&0x000000FF) * 10;

                            document->imu_gyro[document->imu_gyro_count].value.x /= 10.0;
                            document->imu_gyro[document->imu_gyro_count].value.y /= 10.0;
                            document->imu_gyro[document->imu_gyro_count].value.z /= 10.0;

                            document->imu_gyro[document->imu_gyro_count].value.x -= document->imu_gyro[document->imu_gyro_count].value.scale;
                            document->imu_gyro[document->imu_gyro_count].value.y -= document->imu_gyro[document->imu_gyro_count].value.scale;
                            document->imu_gyro[document->imu_gyro_count].value.z -= document->imu_gyro[document->imu_gyro_count].value.scale;

                            document->imu_gyro_count++;
                        }
                        break;

                    case IMU_ACCEL_FB:
                        if (document->imu_accel_count < document->imu_accel_size) {
                            document->imu_accel[document->imu_accel_count].timestamp = getCurrentTimestamp();
                            document->imu_accel[document->imu_accel_count].value.x = (double)((data_left >> 8) & 0x0000FFFF);
                            document->imu_accel[document->imu_accel_count].value.y = (double)((data_left & 0x000000FF) * 0xFF) + ((data_right >> 24) & 0x000000FF);
                            document->imu_accel[document->imu_accel_count].value.z = (double)((data_right >> 8) & 0x0000FFFF);
                            document->imu_accel[document->imu_accel_count].value.scale = (data_right)&0x000000FF;

                            document->imu_accel[document->imu_accel_count].value.x /= 100.0;
                            document->imu_accel[document->imu_accel_count].value.y /= 100.0;
                            document->imu_accel[document->imu_accel_count].value.z /= 100.0;

                            document->imu_accel[document->imu_accel_count].value.x -= document->imu_accel[document->imu_accel_count].value.scale;
                            document->imu_accel[document->imu_accel_count].value.y -= document->imu_accel[document->imu_accel_count].value.scale;
                            document->imu_accel[document->imu_accel_count].value.z -= document->imu_accel[document->imu_accel_count].value.scale;

                            document->imu_accel_count++;
                        }
                        break;

                    case SWE_FB:
                        if (document->steering_wheel.encoder_count < document->steering_wheel.encoder_size) {
                            document->steering_wheel.encoder[document->steering_wheel.encoder_count].timestamp = getCurrentTimestamp();
                            document->steering_wheel.encoder[document->steering_wheel.encoder_count++].value = ((data_left >> 16) & 255);
                        }
                        break;
                }
                break;

            case (GPS_FWE_ID):
                switch (first_byte)
                {
                    case LATSPD_FB:
                        if (lat_done)
                        {
                            ++document->gps.old.location_count;

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

                                document->gps.old.location_count++;

                                lat_done = 0;
                                lon_done = 0;
                            }
                            else
                            {
                                document->gps.old.location_count++;

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
                            document->gps.old.location_count++;

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

                                document->gps.old.location_count++;

                                lat_done = 0;
                                lon_done = 0;
                            }
                            else
                            {
                                document->gps.old.location_count++;

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
                            document->gps.old.time[document->gps.old.time_count++].value.seconds = ((((data_right >> 16) & 0x000000FF) - 48) * 10) + (((data_right >> 8) & 0x000000FF) - 48);
                        }
                        break;

                    case TTM_FB:
                        if (document->gps.old.true_track_mode_count < document->gps.old.true_track_mode_size) {
                            document->gps.old.true_track_mode[document->gps.old.true_track_mode_count].timestamp = getCurrentTimestamp();
                            document->gps.old.true_track_mode[document->gps.old.true_track_mode_count++].value = (data_left >> 8) & 0x0000FFFF;
                        }
                        break;

                    case FRONT_WHEELS_FB:
                        if (document->front_wheels_encoder_count < document->front_wheels_encoder_size) {
                            document->front_wheels_encoder[document->front_wheels_encoder_count].timestamp = getCurrentTimestamp();
                            document->front_wheels_encoder[document->front_wheels_encoder_count].value.speed = ((data_left >> 8) & 0x0000FFFF) * ((data_left & 0x000000FF) == 0 ? 1 : -1);
                            document->front_wheels_encoder[document->front_wheels_encoder_count].value.speedms = (((data_right >> 16) & 0x0000FFFF) * ((data_left & 0x000000FF) == 0 ? 1 : -1)) / 100;

                            document->front_wheels_encoder_count++;
                        }
                        break;

                    case DISTANCE_FB:
                        if (document->distance_count < document->distance_size) {
                            document->distance[document->distance_count].timestamp = getCurrentTimestamp();
                            document->distance[document->distance_count].value.meters = (data_left >> 8) & 0x0000FFFF;
                            document->distance[document->distance_count].value.rotations = ((data_left & 0x000000FF) * 0xFF) + ((data_right >> 24) & 0x000000FF);
                            document->distance[document->distance_count].value.angle = (data_right >> 16) & 0x000000F;
                            document->distance[document->distance_count++].value.clock_period = (data_right >> 8) & 0x000000F;
                        }
                        break;
                }
                break;

            case (BMS_LV_ID):
                if (document->bms_lv.values_count < document->bms_lv.values_size) {
                    document->bms_lv.values[document->bms_lv.values_count].timestamp = getCurrentTimestamp();
                    document->bms_lv.values[document->bms_lv.values_count].value.voltage = (double)((data_left >> 24) & 255) / 10.0;
                    document->bms_lv.values[document->bms_lv.values_count++].value.temperature = (double)((data_left >> 8) & 255) / 5.0;
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

        // Declare other used variables and reset gps element
        int gga_done = 0, gll_done = 0, vtg_done = 0, rmc_done = 0;
        gps_struct* gps_data;
        resetGpsElement(document, document->gps.new_count);

        // Parse gps messages for x milliseconds
        do {
            gps_data = readGPS();
            if (gps_data != NULL && document->gps.new_count < 480) {
                if (gps_data->gga && gps_data->gga->status) {
                    if (gga_done) {
                        ++(document->gps.new_count);
                        resetGpsElement(document, document->gps.new_count);
                        gll_done = 0;
                        vtg_done = 0;
                        rmc_done = 0;
                        document->gps.new[document->gps.new_count].value.latitude_GGA_safe = gps_data->gga->latitude;
                        document->gps.new[document->gps.new_count].value.longitude_GGA_safe = gps_data->gga->longitude;
                        document->gps.new[document->gps.new_count].value.latitude_GGA = parseNmeaCoord(gps_data->gga->latitude);
                        document->gps.new[document->gps.new_count].value.longitude_GGA = parseNmeaCoord(gps_data->gga->longitude);
                        document->gps.new[document->gps.new_count].value.altitude_GGA = gps_data->gga->altitude;
                        document->gps.new[document->gps.new_count].value.ns_indicator_GGA = gps_data->gga->ns_indicator;
                        document->gps.new[document->gps.new_count].value.ew_indicator_GGA = gps_data->gga->ew_indicator;
                        document->gps.new[document->gps.new_count].value.utc_time_GGA = gps_data->gga->utc_time;
                    }
                    else {
                        gga_done = 1;
                        document->gps.new[document->gps.new_count].value.latitude_GGA_safe = gps_data->gga->latitude;
                        document->gps.new[document->gps.new_count].value.longitude_GGA_safe = gps_data->gga->longitude;
                        document->gps.new[document->gps.new_count].value.latitude_GGA = parseNmeaCoord(gps_data->gga->latitude);
                        document->gps.new[document->gps.new_count].value.longitude_GGA = parseNmeaCoord(gps_data->gga->longitude);
                        document->gps.new[document->gps.new_count].value.altitude_GGA = gps_data->gga->altitude;
                        document->gps.new[document->gps.new_count].value.ns_indicator_GGA = gps_data->gga->ns_indicator;
                        document->gps.new[document->gps.new_count].value.ew_indicator_GGA = gps_data->gga->ew_indicator;
                        document->gps.new[document->gps.new_count].value.utc_time_GGA = gps_data->gga->utc_time;
                    }
                }

                if (gps_data->gll && gps_data->gll->status) {
                    if (gll_done) {
                        ++(document->gps.new_count);
                        resetGpsElement(document, document->gps.new_count);
                        gga_done = 0;
                        vtg_done = 0;
                        rmc_done = 0;
                        document->gps.new[document->gps.new_count].value.latitude_GLL = parseNmeaCoord(gps_data->gll->latitude);
                        document->gps.new[document->gps.new_count].value.longitude_GLL = parseNmeaCoord(gps_data->gll->longitude);
                        document->gps.new[document->gps.new_count].value.ns_indicator_GLL = gps_data->gll->ns_indicator;
                        document->gps.new[document->gps.new_count].value.ew_indicator_GLL = gps_data->gll->ew_indicator;
                        document->gps.new[document->gps.new_count].value.utc_time_GLL = gps_data->gll->utc_time;
                    }
                    else {
                        gll_done = 1;
                        document->gps.new[document->gps.new_count].value.latitude_GLL = parseNmeaCoord(gps_data->gll->latitude);
                        document->gps.new[document->gps.new_count].value.longitude_GLL = parseNmeaCoord(gps_data->gll->longitude);
                        document->gps.new[document->gps.new_count].value.ns_indicator_GLL = gps_data->gll->ns_indicator;
                        document->gps.new[document->gps.new_count].value.ew_indicator_GLL = gps_data->gll->ew_indicator;
                        document->gps.new[document->gps.new_count].value.utc_time_GLL = gps_data->gll->utc_time;
                    }
                }

                if (gps_data->vtg) {
                    if (vtg_done) {
                        ++(document->gps.new_count);
                        resetGpsElement(document, document->gps.new_count);
                        gga_done = 0;
                        gll_done = 0;
                        rmc_done = 0;
                        document->gps.new[document->gps.new_count].value.ground_speed_knots_VTG = gps_data->vtg->ground_speed_knots;
                        document->gps.new[document->gps.new_count].value.ground_speed_human_VTG = gps_data->vtg->ground_speed_human;
                    }
                    else {
                        vtg_done = 1;
                        document->gps.new[document->gps.new_count].value.ground_speed_knots_VTG = gps_data->vtg->ground_speed_knots;
                        document->gps.new[document->gps.new_count].value.ground_speed_human_VTG = gps_data->vtg->ground_speed_human;
                    }
                }

                if (gps_data->rmc && gps_data->rmc->status) {
                    if (rmc_done) {
                        ++(document->gps.new_count);
                        resetGpsElement(document, document->gps.new_count);
                        gll_done = 0;
                        vtg_done = 0;
                        gga_done = 0;
                        document->gps.new[document->gps.new_count].value.latitude_RMC = parseNmeaCoord(gps_data->rmc->latitude);
                        document->gps.new[document->gps.new_count].value.longitude_RMC = parseNmeaCoord(gps_data->rmc->longitude);
                        document->gps.new[document->gps.new_count].value.ground_speed_knots_RMC = gps_data->rmc->ground_speed_knots;
                        document->gps.new[document->gps.new_count].value.ns_indicator_RMC = gps_data->rmc->ns_indicator;
                        document->gps.new[document->gps.new_count].value.ew_indicator_RMC = gps_data->rmc->ew_indicator;
                        document->gps.new[document->gps.new_count].value.utc_time_RMC = gps_data->rmc->utc_time;
                        document->gps.new[document->gps.new_count].value.date_RMC = gps_data->rmc->date;
                    }
                    else {
                        rmc_done = 1;
                        document->gps.new[document->gps.new_count].value.latitude_RMC = parseNmeaCoord(gps_data->rmc->latitude);
                        document->gps.new[document->gps.new_count].value.longitude_RMC = parseNmeaCoord(gps_data->rmc->longitude);
                        document->gps.new[document->gps.new_count].value.ground_speed_knots_RMC = gps_data->rmc->ground_speed_knots;
                        document->gps.new[document->gps.new_count].value.ns_indicator_RMC = gps_data->rmc->ns_indicator;
                        document->gps.new[document->gps.new_count].value.ew_indicator_RMC = gps_data->rmc->ew_indicator;
                        document->gps.new[document->gps.new_count].value.utc_time_RMC = gps_data->rmc->utc_time;
                        document->gps.new[document->gps.new_count].value.date_RMC = gps_data->rmc->date;
                    }
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