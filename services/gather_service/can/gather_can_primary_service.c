#include "gather_can_primary_service.h"

/* INTERNAL VARIABLES AND STRUCTS */

static pthread_t _gather_can_thread;
static pthread_attr_t _gather_can_thread_attr;

/* INTERNAL FUNCTIONS SIGNATURES */

static void* _parseCanMessages();

/* EXPORTED FUNCTIONS */

void gatherCanPrimaryStartThread() {
	pthread_attr_init(&_gather_can_thread_attr);
	pthread_attr_setdetachstate(&_gather_can_thread_attr, PTHREAD_CREATE_JOINABLE);
	pthread_create(&_gather_can_thread, &_gather_can_thread_attr, &_parseCanMessages, NULL);
	pthread_attr_destroy(&_gather_can_thread_attr);
}

void gatherCanPrimaryStopThread() {
	pthread_cancel(_gather_can_thread);
}

/* INTERNAL FUNCTIONS DEFINITIONS */

static void* _parseCanMessages() {
	// Declare used variables
	data_t* document;
	int id;
	char* data;

	while (1) {
		// Read can
		canReadPrimary(&id, &data);

		// Lock document
		pthread_mutex_lock(&condition.structure.threads.data_head_mutex);

		// Create reference to data_head
		document = condition.structure.data_head;

		switch (id) {
			case (ID_SET_TLM_STATUS): {
				Primary_SET_TLM_STATUS* message = (Primary_SET_TLM_STATUS*)malloc(sizeof(Primary_SET_TLM_STATUS));
				deserialize_Primary_SET_TLM_STATUS(data, message);

				Primary_Tlm_Status status = message->tlm_status;
				int circuit = message->circuit;
				int pilot_index = message->pilot;
				int race_index = message->race;

				switch (status) {
					case Primary_Tlm_Status_ON:
						if (condition.structure.enabled == 1) {
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
						break;

					case Primary_Tlm_Status_OFF:
						if (condition.structure.enabled == 0) {
							logWarning("Error in structure: telemetry already disabled");
						}
						else {
							pthread_mutex_lock(&condition.structure.threads.toggle_state_mutex);
							condition.structure.toggle_state = 1;
							pthread_mutex_unlock(&condition.structure.threads.toggle_state_mutex);
						}
						break;
					default:
						logWarning("Error in structure: invalid status from wheel");
				}

				break;
			}
			case (ID_IMU_GYRO): {
				int count = document->imu.gyro_count;
				if (count < document->imu.gyro_size) {
					Primary_IMU_GYRO* message = (Primary_IMU_GYRO*) malloc(sizeof(Primary_IMU_GYRO));
					deserialize_Primary_IMU_GYRO(data, message);
					
					
					document->imu.gyro[count].timestamp = getCurrentTimestamp();
					document->imu.gyro[count].value.x = (message->x > 32767 ? message->x - 65535 : message->x) / 100.0;
					document->imu.gyro[count].value.y = (message->y > 32767 ? message->y - 65535 : message->y) / 100.0;
					document->imu.gyro[count].value.z = (message->z > 32767 ? message->z - 65535 : message->z) / 100.0;
					++document->imu.gyro_count;
				}
				break;
			}
			
			case (ID_IMU_ACCEL): {
				int count = document->imu.accel_count;
				if (count < document->imu.accel_size) {
					Primary_IMU_ACCEL* message = (Primary_IMU_ACCEL*) malloc(sizeof(Primary_IMU_ACCEL));
					deserialize_Primary_IMU_ACCEL(data, message);
					
					
					document->imu.accel[count].timestamp = getCurrentTimestamp();
					document->imu.accel[count].value.x = (message->x > 32767 ? message->x - 65535 : message->x) / 100.0;
					document->imu.accel[count].value.y = (message->y > 32767 ? message->y - 65535 : message->y) / 100.0;
					document->imu.accel[count].value.z = (message->z > 32767 ? message->z - 65535 : message->z) / 100.0;
					++document->imu.accel_count;
				}
				break;
			}
			
			case (ID_INVERTER_RIGHT_SPEED): {
				int count = document->inverters.right.speed_count;
				if (count < document->inverters.right.speed_size) {
					Primary_INVERTER_RIGHT_SPEED* message = (Primary_INVERTER_RIGHT_SPEED*) malloc(sizeof(Primary_INVERTER_RIGHT_SPEED));
					deserialize_Primary_INVERTER_RIGHT_SPEED(data, message);
					
					
					document->inverters.right.speed[count].timestamp = getCurrentTimestamp();
					document->inverters.right.speed[count].value = message->value;
					++document->inverters.right.speed_count;
				}
				break;
			}
			
			case (ID_INVERTER_RIGHT_TEMPERATURE_IGBT): {
				int count = document->inverters.right.temperature_igbt_count;
				if (count < document->inverters.right.temperature_igbt_size) {
					Primary_INVERTER_RIGHT_TEMPERATURE_IGBT* message = (Primary_INVERTER_RIGHT_TEMPERATURE_IGBT*) malloc(sizeof(Primary_INVERTER_RIGHT_TEMPERATURE_IGBT));
					deserialize_Primary_INVERTER_RIGHT_TEMPERATURE_IGBT(data, message);
					
					
					document->inverters.right.temperature_igbt[count].timestamp = getCurrentTimestamp();
					document->inverters.right.temperature_igbt[count].value = message->value;
					++document->inverters.right.temperature_igbt_count;
				}
				break;
			}
			
			case (ID_INVERTER_RIGHT_TEMPERATURE_MOTORS): {
				int count = document->inverters.right.temperature_motors_count;
				if (count < document->inverters.right.temperature_motors_size) {
					Primary_INVERTER_RIGHT_TEMPERATURE_MOTORS* message = (Primary_INVERTER_RIGHT_TEMPERATURE_MOTORS*) malloc(sizeof(Primary_INVERTER_RIGHT_TEMPERATURE_MOTORS));
					deserialize_Primary_INVERTER_RIGHT_TEMPERATURE_MOTORS(data, message);
					
					
					document->inverters.right.temperature_motors[count].timestamp = getCurrentTimestamp();
					document->inverters.right.temperature_motors[count].value = message->value;
					++document->inverters.right.temperature_motors_count;
				}
				break;
			}
			
			case (ID_INVERTER_RIGHT_TORQUE): {
				int count = document->inverters.right.torque_count;
				if (count < document->inverters.right.torque_size) {
					Primary_INVERTER_RIGHT_TORQUE* message = (Primary_INVERTER_RIGHT_TORQUE*) malloc(sizeof(Primary_INVERTER_RIGHT_TORQUE));
					deserialize_Primary_INVERTER_RIGHT_TORQUE(data, message);
					
					
					document->inverters.right.torque[count].timestamp = getCurrentTimestamp();
					document->inverters.right.torque[count].value = message->value;
					++document->inverters.right.torque_count;
				}
				break;
			}
			
			case (ID_INVERTER_LEFT_SPEED): {
				int count = document->inverters.left.speed_count;
				if (count < document->inverters.left.speed_size) {
					Primary_INVERTER_LEFT_SPEED* message = (Primary_INVERTER_LEFT_SPEED*) malloc(sizeof(Primary_INVERTER_LEFT_SPEED));
					deserialize_Primary_INVERTER_LEFT_SPEED(data, message);
					
					
					document->inverters.left.speed[count].timestamp = getCurrentTimestamp();
					document->inverters.left.speed[count].value = message->value;
					++document->inverters.left.speed_count;
				}
				break;
			}
			
			case (ID_INVERTER_LEFT_TEMPERATURE_IGBT): {
				int count = document->inverters.left.temperature_igbt_count;
				if (count < document->inverters.left.temperature_igbt_size) {
					Primary_INVERTER_LEFT_TEMPERATURE_IGBT* message = (Primary_INVERTER_LEFT_TEMPERATURE_IGBT*) malloc(sizeof(Primary_INVERTER_LEFT_TEMPERATURE_IGBT));
					deserialize_Primary_INVERTER_LEFT_TEMPERATURE_IGBT(data, message);
					
					
					document->inverters.left.temperature_igbt[count].timestamp = getCurrentTimestamp();
					document->inverters.left.temperature_igbt[count].value = message->value;
					++document->inverters.left.temperature_igbt_count;
				}
				break;
			}
			
			case (ID_INVERTER_LEFT_TEMPERATURE_MOTORS): {
				int count = document->inverters.left.temperature_motors_count;
				if (count < document->inverters.left.temperature_motors_size) {
					Primary_INVERTER_LEFT_TEMPERATURE_MOTORS* message = (Primary_INVERTER_LEFT_TEMPERATURE_MOTORS*) malloc(sizeof(Primary_INVERTER_LEFT_TEMPERATURE_MOTORS));
					deserialize_Primary_INVERTER_LEFT_TEMPERATURE_MOTORS(data, message);
					
					
					document->inverters.left.temperature_motors[count].timestamp = getCurrentTimestamp();
					document->inverters.left.temperature_motors[count].value = message->value;
					++document->inverters.left.temperature_motors_count;
				}
				break;
			}
			
			case (ID_INVERTER_LEFT_TORQUE): {
				int count = document->inverters.left.torque_count;
				if (count < document->inverters.left.torque_size) {
					Primary_INVERTER_LEFT_TORQUE* message = (Primary_INVERTER_LEFT_TORQUE*) malloc(sizeof(Primary_INVERTER_LEFT_TORQUE));
					deserialize_Primary_INVERTER_LEFT_TORQUE(data, message);
					
					
					document->inverters.left.torque[count].timestamp = getCurrentTimestamp();
					document->inverters.left.torque[count].value = message->value;
					++document->inverters.left.torque_count;
				}
				break;
			}
			
			case (ID_PEDALS_BRAKE): {
				int count = document->pedals.brake_count;
				if (count < document->pedals.brake_size) {
					Primary_PEDALS_BRAKE* message = (Primary_PEDALS_BRAKE*) malloc(sizeof(Primary_PEDALS_BRAKE));
					deserialize_Primary_PEDALS_BRAKE(data, message);
					
					
					document->pedals.brake[count].timestamp = getCurrentTimestamp();
					document->pedals.brake[count].value.is_breaking = message->is_breaking;
					document->pedals.brake[count].value.pressure_front = message->pressure_front / 500.0;
					document->pedals.brake[count].value.pressure_back = message->pressure_back / 500.0;
					++document->pedals.brake_count;
				}
				break;
			}
			
			case (ID_PEDALS_THROTTLE): {
				int count = document->pedals.throttle_count;
				if (count < document->pedals.throttle_size) {
					Primary_PEDALS_THROTTLE* message = (Primary_PEDALS_THROTTLE*) malloc(sizeof(Primary_PEDALS_THROTTLE));
					deserialize_Primary_PEDALS_THROTTLE(data, message);
					
					
					document->pedals.throttle[count].timestamp = getCurrentTimestamp();
					document->pedals.throttle[count].value = message->value;
					++document->pedals.throttle_count;
				}
				break;
			}
			
			case (ID_STEERING_WHEEL_ENCODER): {
				int count = document->steering_wheel.encoder_count;
				if (count < document->steering_wheel.encoder_size) {
					Primary_STEERING_WHEEL_ENCODER* message = (Primary_STEERING_WHEEL_ENCODER*) malloc(sizeof(Primary_STEERING_WHEEL_ENCODER));
					deserialize_Primary_STEERING_WHEEL_ENCODER(data, message);
					
					
					document->steering_wheel.encoder[count].timestamp = getCurrentTimestamp();
					document->steering_wheel.encoder[count].value = message->value;
					++document->steering_wheel.encoder_count;
				}
				break;
			}
			
			
		}

		// Unlock document
		pthread_mutex_unlock(&condition.structure.threads.data_head_mutex);
	}

	return NULL;
}

// case (WHEEL_ID):
// 		if (first_byte == GEARS_FB) {
// 			if (document->steering_wheel.gears_count < document->steering_wheel.gears_size) {
// 				document->steering_wheel.gears[document->steering_wheel.gears_count].timestamp = getCurrentTimestamp();
// 				document->steering_wheel.gears[document->steering_wheel.gears_count].value.control = (data_left >> 16) & 0xFF;
// 				document->steering_wheel.gears[document->steering_wheel.gears_count].value.cooling = (data_left >> 8) & 0xFF;
// 				document->steering_wheel.gears[document->steering_wheel.gears_count].value.map = (data_left) & 0xFF;
// 			}
// 		} else if (first_byte == MARKER_FB) {
// 			document->steering_wheel.marker = 1;
// 		} else if (first_byte == SIGNAL_FB) {
// 			int status = (data_left >> 16) & 0xFF;
// 			int pilot_index = (data_left >> 8) & 0xFF;
// 			int race_index = data_left & 0xFF;


// 			else if (status == 1) {
// 				if (condition.structure.enabled == 1) {
// 					logWarning("Error in structure: telemetry already enabled");
// 				}
// 				else {
// 					if (pilot_index >= condition.session.pilots_count) {
// 						logWarning("Error in structure: invalid pilot from wheel. Using default pilot.");
// 						pilot_index = 0;
// 					}
// 					if (race_index >= condition.session.races_count) {
// 						logWarning("Error in structure: invalid race from wheel. Using default race.");
// 						race_index = 0;
// 					}

// 					condition.session.selected_pilot = pilot_index;
// 					condition.session.selected_race = race_index;

// 					pthread_mutex_lock(&condition.structure.threads.toggle_state_mutex);
// 					condition.structure.toggle_state = 1;
// 					pthread_mutex_unlock(&condition.structure.threads.toggle_state_mutex);
// 				}
// 			}
// 			else {
// 				logWarning("Error in structure: invalid status from wheel");
// 			}
// 		}
// 		break;
// }