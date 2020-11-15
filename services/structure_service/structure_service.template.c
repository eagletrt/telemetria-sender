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
                    document->gps.new.gll[document->gps.new.gll_count].value.latitude = parseNmeaCoord(gps_data->gll->latitude);
                    document->gps.new.gll[document->gps.new.gll_count].value.longitude = parseNmeaCoord(gps_data->gll->longitude);
                    document->gps.new.gll[document->gps.new.gll_count].value.ns_indicator = gps_data->gll->ns_indicator;
                    document->gps.new.gll[document->gps.new.gll_count].value.ew_indicator = gps_data->gll->ew_indicator;
                    document->gps.new.gll[document->gps.new.gll_count].value.utc_time = gps_data->gll->utc_time;
                    ++(document->gps.new.gll_count);
                }

                if (gps_data->vtg && document->gps.new.vtg_count < document->gps.new.vtg_size) {
                    document->gps.new.vtg[document->gps.new.vtg_count].value.ground_speed_knots = gps_data->vtg->ground_speed_knots;
                    document->gps.new.vtg[document->gps.new.vtg_count].value.ground_speed_human = gps_data->vtg->ground_speed_human;
                    ++(document->gps.new.vtg_count);
                }

                if (gps_data->rmc && gps_data->rmc->status && document->gps.new.rmc_count < document->gps.new.rmc_size) {
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