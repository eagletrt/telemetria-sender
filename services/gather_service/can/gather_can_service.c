#include "gather_can_service.h"

/* INTERNAL VARIABLES AND STRUCTS */

pthread_t gather_can_thread;
pthread_attr_t gather_can_thread_attr;

/* INTERNAL FUNCTIONS SIGNATURES */

static void* parseCanMessages(void *args);

/* EXPORTED FUNCTIONS */

void gatherCanStartThread(int enabled) {
	pthread_attr_init(&gather_can_thread_attr);
	pthread_attr_setdetachstate(&gather_can_thread_attr, PTHREAD_CREATE_JOINABLE);
	pthread_create(&gather_can_thread, &gather_can_thread_attr, &parseCanMessages, (void*) ((long) enabled));
	pthread_attr_destroy(&gather_can_thread_attr);
}

void gatherCanStopThread() {
	pthread_cancel(gather_can_thread);
}

/* INTERNAL FUNCTIONS DEFINITIONS */

static void* parseCanMessages(void *args) {
	// Getting enabled arg
	int enabled = (int) ((long) args);

    // Declare used variables
    data_t* document;
    int id = 0, data_left, data_right, first_byte;
    int byte_left, byte_right, temp;

	while (1) {
		// Assign used variables
        id = 0;

		// Read can
		canRead(&id, &data_left, &data_right);
		first_byte = ((data_left >> 24) & 255);

        // Lock document
        pthread_mutex_lock(&condition.structure.threads.data_head_mutex);

        // Create reference to data_head
        document = condition.structure.data_head;

		// Parse message in base of the id
		switch (id) {
			case (INVERTER_RIGHT_ID):
				switch (first_byte) {
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
							//TODO: very different uguale a python
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
							//TODO: very different uguale a python
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
				switch (first_byte) {
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
							//TODO: very different uguale a python
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
							//TODO: very different uguale a python
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
				switch (first_byte) {
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
				} else if (first_byte == BRAKE_FB && document->pedals.brake_count < document->pedals.brake_size) {
					document->pedals.brake[document->pedals.brake_count].timestamp = getCurrentTimestamp();
					document->pedals.brake[document->pedals.brake_count].value.is_breaking = ((data_left >> 16) & 0x000000FF);
					document->pedals.brake[document->pedals.brake_count].value.pressure_front = ((data_left & 0x0000FF00) + ((data_right >> 24) & 0x000000FF)) / 500;
					document->pedals.brake[document->pedals.brake_count].value.pressure_back = (((data_right >> 8) & 0x0000FF00) + (data_right & 0x000000FF)) / 500;

					++(document->pedals.brake_count);
				}
				break;

			case (IMU_SWE_OLD_ID):
				switch (first_byte) {
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
							//TODO: to change canlog
							//document->steering_wheel.encoder[document->steering_wheel.encoder_count].value = ((data_left >> 16) & 0x000000FF);
							document->steering_wheel.encoder[document->steering_wheel.encoder_count].value = ((data_left >> 8) & 0x0000FFFF) / 100.0;
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
				switch (first_byte) {
					case FRONT_WHEELS_FB_SPEED_FB:
						if (document->front_wheels_encoders.right.speed_count < document->front_wheels_encoders.right.speed_size) {
							document->front_wheels_encoders.right.speed[document->front_wheels_encoders.right.speed_count].timestamp = getCurrentTimestamp();
							//TODO: important check / 1
							document->front_wheels_encoders.right.speed[document->front_wheels_encoders.right.speed_count].value.speed = ((data_left >> 8) & 0x0000FFFF) * ((data_left & 0x000000FF) == 0 ? 1.0 : -1.0);
							document->front_wheels_encoders.right.speed[document->front_wheels_encoders.right.speed_count].value.error_flag = (data_right >> 8) & 0x000000FF;
							++(document->front_wheels_encoders.right.speed_count);
						}
						break;

					case FRONT_WHEELS_FB_SPEED_RADS_FB:
						if (document->front_wheels_encoders.right.speed_rads_count < document->front_wheels_encoders.right.speed_rads_size) {
							document->front_wheels_encoders.right.speed_rads[document->front_wheels_encoders.right.speed_rads_count].timestamp = getCurrentTimestamp();
							//TODO: check / 1000.0 and not / 1000 and negative
							document->front_wheels_encoders.right.speed_rads[document->front_wheels_encoders.right.speed_rads_count].value = (data_left & 0x00FFFFFF) / (((data_right & 0x000000FF) == 1) ? -10000.0 : 10000.0);
							++(document->front_wheels_encoders.right.speed_rads_count);
						}
						break;

					case FRONT_WHEELS_FB_SPEED_ANGLE_FB:
						if (document->front_wheels_encoders.right.angle_count < document->front_wheels_encoders.right.angle_size) {
							document->front_wheels_encoders.right.angle[document->front_wheels_encoders.right.angle_count].timestamp = getCurrentTimestamp();
							//TODO: check / 100.0 and not / 100
							document->front_wheels_encoders.right.angle[document->front_wheels_encoders.right.angle_count].value.angle_0 = ((data_left >> 8) & 0x0000FFFF) / 100.0;
							document->front_wheels_encoders.right.angle[document->front_wheels_encoders.right.angle_count].value.angle_1 = (((data_left & 0x000000FF) * 0xFF) + ((data_right >> 24) & 0x000000FF)) / 100.0;
							document->front_wheels_encoders.right.angle[document->front_wheels_encoders.right.angle_count].value.angle_delta = ((data_right >> 8) & 0x0000FFFF) / 100.0;
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
				switch (first_byte) {
					case FRONT_WHEELS_FB_SPEED_FB:
						if (document->front_wheels_encoders.left.speed_count < document->front_wheels_encoders.left.speed_size) {
							document->front_wheels_encoders.left.speed[document->front_wheels_encoders.left.speed_count].timestamp = getCurrentTimestamp();
							//TODO: IMPORTANT check double
							document->front_wheels_encoders.left.speed[document->front_wheels_encoders.left.speed_count].value.speed = ((data_left >> 8) & 0x0000FFFF) * ((data_left & 0x000000FF) == 0 ? 1 : -1);
							document->front_wheels_encoders.left.speed[document->front_wheels_encoders.left.speed_count].value.error_flag = (data_right >> 8) & 0x000000FF;
							++(document->front_wheels_encoders.left.speed_count);
						}
						break;

					case FRONT_WHEELS_FB_SPEED_RADS_FB:
						if (document->front_wheels_encoders.left.speed_rads_count < document->front_wheels_encoders.left.speed_rads_size) {
							document->front_wheels_encoders.left.speed_rads[document->front_wheels_encoders.left.speed_rads_count].timestamp = getCurrentTimestamp();
							//TODO: check / 1000.0 and not / 1000 and negative
							document->front_wheels_encoders.left.speed_rads[document->front_wheels_encoders.left.speed_rads_count].value = (data_left & 0x00FFFFFF) / (((data_right & 0x000000FF) == 1) ? -10000.0 : 10000.0);
							++(document->front_wheels_encoders.left.speed_rads_count);
						}
						break;

					case FRONT_WHEELS_FB_SPEED_ANGLE_FB:
						if (document->front_wheels_encoders.left.angle_count < document->front_wheels_encoders.left.angle_size) {
							document->front_wheels_encoders.left.angle[document->front_wheels_encoders.left.angle_count].timestamp = getCurrentTimestamp();
							//TODO: check / 100.0 and not / 100
							document->front_wheels_encoders.left.angle[document->front_wheels_encoders.left.angle_count].value.angle_0 = ((data_left >> 8) & 0x0000FFFF) / 100.0;
							document->front_wheels_encoders.left.angle[document->front_wheels_encoders.left.angle_count].value.angle_1 = (((data_left & 0x000000FF) * 0xFF) + ((data_right >> 24) & 0x000000FF)) / 100.0;
							document->front_wheels_encoders.left.angle[document->front_wheels_encoders.left.angle_count].value.angle_delta = ((data_right >> 8) & 0x0000FFFF) / 100.0;
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
				if (first_byte == GEARS_FB) {
					if (document->steering_wheel.gears_count < document->steering_wheel.gears_size) {
						document->steering_wheel.gears[document->steering_wheel.gears_count].timestamp = getCurrentTimestamp();
						document->steering_wheel.gears[document->steering_wheel.gears_count].value.control = (data_left >> 16) & 0xFF;
						document->steering_wheel.gears[document->steering_wheel.gears_count].value.cooling = (data_left >> 8) & 0xFF;
						document->steering_wheel.gears[document->steering_wheel.gears_count].value.map = (data_left)&0xFF;
					}
				} else if (first_byte == MARKER_FB) {
					document->steering_wheel.marker = 1;
				} else if (first_byte == SIGNAL_FB) {
					int status = (data_left >> 16) & 0xFF;
					int pilot_index = (data_left >> 8) & 0xFF;
					int race_index = data_left & 0xFF;

					if (status == 0) {
						if (enabled == 0) {
							logWarning("Error in structure: telemetry already disabled");
						}
						else {
							pthread_mutex_lock(&condition.structure.threads.toggle_state_mutex);
							condition.structure.toggle_state = 1;
							pthread_mutex_unlock(&condition.structure.threads.toggle_state_mutex);
						}
					}
					else if (status == 1) {
						if (enabled == 1) {
							logWarning("Error in structure: telemetry already enabled");
						}
						else {
							if (pilot_index >= condition.session.pilots_count) {
								logWarning("Error in structure: invalid pilot from wheel. Using default pilot.");
								pilot_index = 0;
							} 
							if (race_index >= condition.session.races_count) {
								logWarning("Error in structure: invalid race from wheel. Using default race.");
								race_index = 0;
							} 

							condition.session.selected_pilot = pilot_index;
							condition.session.selected_race = race_index;
							
							pthread_mutex_lock(&condition.structure.threads.toggle_state_mutex);
							condition.structure.toggle_state = 1;
							pthread_mutex_unlock(&condition.structure.threads.toggle_state_mutex);
						}
					}
					else {
						logWarning("Error in structure: invalid status from wheel");
					}
				}
				break;
		}

        // Unlock document
        pthread_mutex_unlock(&condition.structure.threads.data_head_mutex);
	}

	return NULL;
}
