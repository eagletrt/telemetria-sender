#include "gather_can_service.h"

/* INTERNAL VARIABLES AND STRUCTS */

pthread_t gather_can_thread;
pthread_attr_t gather_can_thread_attr;

/* INTERNAL FUNCTIONS SIGNATURES */

static void* parseCanMessages(void* args);

/* EXPORTED FUNCTIONS */

void gatherCanStartThread() {
	pthread_attr_init(&gather_can_thread_attr);
	pthread_attr_setdetachstate(&gather_can_thread_attr, PTHREAD_CREATE_JOINABLE);
	pthread_create(&gather_can_thread, &gather_can_thread_attr, &parseCanMessages, NULL);
	pthread_setname_np(gather_can_thread, "tlm_can_gather");
	pthread_attr_destroy(&gather_can_thread_attr);
}

void gatherCanStopThread() {
	pthread_cancel(gather_can_thread);
}

/* INTERNAL FUNCTIONS DEFINITIONS */

static void* parseCanMessages(void* args) {
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
							byte_left = data_left & 0x0000FF00;
							byte_right = (data_left >> 16) & 0x000000FF;
							temp = byte_left + byte_right;
							document->inverters.right.speed[document->inverters.right.speed_count].timestamp = getCurrentTimestamp();
							document->inverters.right.speed[document->inverters.right.speed_count].value = (temp >= 32768 ? temp - 65536 : temp);
							++(document->inverters.right.speed_count);
						}
						break;

					case INVERTER_TEMPERATURE_IGBT_FB:
						if (document->inverters.right.temperature_igbt_count < document->inverters.right.temperature_igbt_size) {
							byte_left = data_left & 0x0000FF00;
							byte_right = (data_left >> 16) & 0x000000FF;
							temp = byte_left + byte_right;
							document->inverters.right.temperature_igbt[document->inverters.right.temperature_igbt_count].timestamp = getCurrentTimestamp();
							document->inverters.right.temperature_igbt[document->inverters.right.temperature_igbt_count].value = (temp - 15797.0) / 112.1182;
							++(document->inverters.right.temperature_igbt_count);
						}
						break;

					case INVERTER_TEMPERATURE_MOTORS_FB:
						if (document->inverters.right.temperature_motors_count < document->inverters.right.temperature_motors_size) {
							byte_left = data_left & 0x0000FF00;
							byte_right = (data_left >> 16) & 0x000000FF;
							temp = byte_left + byte_right;
							document->inverters.right.temperature_motors[document->inverters.right.temperature_motors_count].timestamp = getCurrentTimestamp();
							document->inverters.right.temperature_motors[document->inverters.right.temperature_motors_count].value = (temp - 9393.9) / 55.1;
							++(document->inverters.right.temperature_motors_count);
						}
						break;
					case INVERTER_TORQUE_FB:
						if (document->inverters.right.torque_count < document->inverters.right.torque_size) {
							byte_left = data_left & 0x0000FF00;
							byte_right = (data_left >> 16) & 0x000000FF;
							temp = byte_left + byte_right;
							document->inverters.right.torque[document->inverters.right.torque_count].timestamp = getCurrentTimestamp();
							document->inverters.right.torque[document->inverters.right.torque_count].value = (temp - 9393.9) / 55.1;
							++(document->inverters.right.torque_count);
						}
						break;
				}
				break;

			case (INVERTER_LEFT_ID):
				switch (first_byte) {
					case INVERTER_SPEED_FB:
						if (document->inverters.left.speed_count < document->inverters.left.speed_size) {
							byte_left = data_left & 0x0000FF00;
							byte_right = (data_left >> 16) & 0x000000FF;
							temp = byte_left + byte_right;
							document->inverters.left.speed[document->inverters.left.speed_count].timestamp = getCurrentTimestamp();
							document->inverters.left.speed[document->inverters.left.speed_count].value = (temp >= 32768 ? temp - 65536 : temp);
							++(document->inverters.left.speed_count);
						}
						break;

					case INVERTER_TEMPERATURE_IGBT_FB:
						if (document->inverters.left.temperature_igbt_count < document->inverters.left.temperature_igbt_size) {
							byte_left = data_left & 0x0000FF00;
							byte_right = (data_left >> 16) & 0x000000FF;
							temp = byte_left + byte_right;
							document->inverters.left.temperature_igbt[document->inverters.left.temperature_igbt_count].timestamp = getCurrentTimestamp();
							document->inverters.left.temperature_igbt[document->inverters.left.temperature_igbt_count].value = (temp - 15797) / 112.1182;
							++(document->inverters.left.temperature_igbt_count);
						}
						break;

					case INVERTER_TEMPERATURE_MOTORS_FB:
						if (document->inverters.left.temperature_motors_count < document->inverters.left.temperature_motors_size) {
							byte_left = data_left & 0x0000FF00;
							byte_right = (data_left >> 16) & 0x000000FF;
							temp = byte_left + byte_right;
							document->inverters.left.temperature_motors[document->inverters.left.temperature_motors_count].timestamp = getCurrentTimestamp();
							document->inverters.left.temperature_motors[document->inverters.left.temperature_motors_count].value = (temp - 9393.9) / 55.1;
							++(document->inverters.left.temperature_motors_count);
						}
						break;
					case INVERTER_TORQUE_FB:
						if (document->inverters.left.torque_count < document->inverters.left.torque_size) {
							byte_left = data_left & 0x0000FF00;
							byte_right = (data_left >> 16) & 0x000000FF;
							temp = byte_left + byte_right;
							document->inverters.left.torque[document->inverters.left.torque_count].timestamp = getCurrentTimestamp();
							document->inverters.left.torque[document->inverters.left.torque_count].value = (temp - 9393.9) / 55.1;
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
							document->bms_hv.voltage[document->bms_hv.voltage_count].value.total = (double)(data_left & 0x00FFFFFF) / 10000.0;
							document->bms_hv.voltage[document->bms_hv.voltage_count].value.max = (double)((data_right >> 16) & 0x0000FFFF) / 10000.0;
							document->bms_hv.voltage[document->bms_hv.voltage_count].value.min = (double)(data_right & 0x0000FFFF) / 10000.0;
							++(document->bms_hv.voltage_count);
						}
						break;

					case TEMPERATURE_FB:
						if (document->bms_hv.temperature_count < document->bms_hv.temperature_size) {
							document->bms_hv.temperature[document->bms_hv.temperature_count].timestamp = getCurrentTimestamp();
							document->bms_hv.temperature[document->bms_hv.temperature_count].value.average = ((data_left >> 8) & 0x0000FFFF) / 100.0;
							document->bms_hv.temperature[document->bms_hv.temperature_count].value.max = (((data_left & 0x000000FF) * 256.0 + ((data_right >> 24) & 0x000000FF))) / 100.0;
							document->bms_hv.temperature[document->bms_hv.temperature_count].value.min = ((data_right >> 8) & 0x0000FFFF) / 100.0;
							++(document->bms_hv.temperature_count);
						}
						break;

					case CURRENT_FB:
						if (document->bms_hv.current_count < document->bms_hv.current_size) {
							document->bms_hv.current[document->bms_hv.current_count].timestamp = getCurrentTimestamp();
							document->bms_hv.current[document->bms_hv.current_count].value.current = (double)((data_left >> 8) & 0x0000FFFF) / 10.0;
							document->bms_hv.current[document->bms_hv.current_count].value.pow = (double)((data_left & 0x000000FF) * 256.0 + ((data_right >> 24) & 0x000000FF));
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
					document->pedals.brake[document->pedals.brake_count].value.pressure_front = ((data_left & 0x0000FF00) + ((data_right >> 24) & 0x000000FF)) / 500.0;
					document->pedals.brake[document->pedals.brake_count].value.pressure_back = (((data_right >> 8) & 0x0000FF00) + (data_right & 0x000000FF)) / 500.0;

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
							document->imu_old.gyro[document->imu_old.gyro_count].value.scale = ((data_right)&0x000000FF) * 10.0;

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
					document->imu.gyro[document->imu.gyro_count].value.x = (temp > 32768 ? temp - 65536 : temp);

					temp = (data_left & 0x0000FFFF);
					document->imu.gyro[document->imu.gyro_count].value.y = (temp > 32768 ? temp - 65536 : temp);

					temp = ((data_right >> 16) & 0x0000FFFF);
					document->imu.gyro[document->imu.gyro_count].value.z = (temp > 32768 ? temp - 65536 : temp);

					document->imu.gyro[document->imu.gyro_count].value.x /= 100.0;
					document->imu.gyro[document->imu.gyro_count].value.y /= 100.0;
					document->imu.gyro[document->imu.gyro_count].value.z /= 100.0;

					++(document->imu.gyro_count);
				}
				break;

			case (IMU_ACCEL_ID):
				if (document->imu.accel_count < document->imu.accel_size) {
					document->imu.accel[document->imu.accel_count].timestamp = getCurrentTimestamp();
					temp = ((data_left >> 16) & 0x0000FFFF);
					document->imu.accel[document->imu.accel_count].value.x = (temp > 32768 ? temp - 65536 : temp);

					temp = (data_left & 0x0000FFFF);
					document->imu.accel[document->imu.accel_count].value.y = (temp > 32768 ? temp - 65536 : temp);

					temp = ((data_right >> 16) & 0x0000FFFF);
					document->imu.accel[document->imu.accel_count].value.z = (temp > 32768 ? temp - 65536 : temp);

					document->imu.accel[document->imu.accel_count].value.x /= 100.0;
					document->imu.accel[document->imu.accel_count].value.y /= 100.0;
					document->imu.accel[document->imu.accel_count].value.z /= 100.0;

					++(document->imu.accel_count);
				}
				break;

			case (FRONT_WHEELS_ENCODERS_ROTATION_AND_KM_ID):
				if (document->front_wheels_encoders.rotation_and_km_count < document->front_wheels_encoders.speed_rads_size) {
					document->front_wheels_encoders.rotation_and_km[document->front_wheels_encoders.rotation_and_km_count].timestamp = getCurrentTimestamp();
					document->front_wheels_encoders.rotation_and_km[document->front_wheels_encoders.rotation_and_km_count].value.rotations = ((data_left >> 8) & 0x00FFFFFF) * (((data_right >> 8) & 0x000000FF) == 0 ? 1.0 : -1.0);
					document->front_wheels_encoders.rotation_and_km[document->front_wheels_encoders.rotation_and_km_count].value.km = (((data_left & 0x000000FF) >> 16) + ((data_left >> 16) & 0x0000FFFF)) * ((data_right & 0x000000FF) == 0 ? 1.0 : -1.0);
					++(document->front_wheels_encoders.rotation_and_km_count);
				}
				break;
				
			case (FRONT_WHEELS_ENCODERS_SPEED_RADS_ID):
				if (document->front_wheels_encoders.speed_rads_count < document->front_wheels_encoders.speed_rads_size) {
					document->front_wheels_encoders.speed_rads[document->front_wheels_encoders.speed_rads_count].timestamp = getCurrentTimestamp();
					document->front_wheels_encoders.speed_rads[document->front_wheels_encoders.speed_rads_count].value.left = ((data_left >> 8) & 0x00FFFFFF);
					document->front_wheels_encoders.speed_rads[document->front_wheels_encoders.speed_rads_count].value.right = (((data_left & 0x000000FF) >> 16) + ((data_left >> 16) & 0x0000FFFF));
					++(document->front_wheels_encoders.speed_rads_count);
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
						if (condition.structure.enabled == 0) {
							logWarning("Error in structure: telemetry already disabled");
						} else {
							pthread_mutex_lock(&condition.structure.threads.toggle_state_mutex);
							condition.structure.toggle_state = 1;
							pthread_mutex_unlock(&condition.structure.threads.toggle_state_mutex);
						}
					} else if (status == 1) {
						if (condition.structure.enabled == 1) {
							logWarning("Error in structure: telemetry already enabled");
						} else {
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
					} else {
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
