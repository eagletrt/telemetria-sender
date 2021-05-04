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
				deserialize_Primary_SET_TLM_STATUS(data, 8, message);

				Primary_Tlm_Status status = message->tlm_status;
				int circuit = message->circuit;
				int pilot_index = message->driver;
				int race_index = message->race_type;

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
			case (ID_HV_VOLTAGE): {
				int count = document->bms_hv.voltage_count;
				if (count < document->bms_hv.voltage_size) {
					Primary_HV_VOLTAGE* message = (Primary_HV_VOLTAGE*) malloc(sizeof(Primary_HV_VOLTAGE));
					deserialize_Primary_HV_VOLTAGE(data, 8, message);
					
					
					document->bms_hv.voltage[count].timestamp = getCurrentTimestamp();
					document->bms_hv.voltage[count].value.pack = message->pack_voltage;
					document->bms_hv.voltage[count].value.bus = message->bus_voltage;
					document->bms_hv.voltage[count].value.max = message->max_cell_voltage;
					document->bms_hv.voltage[count].value.min = message->min_cell_voltage;
					++document->bms_hv.voltage_count;
				}
				break;
			}
			
			case (ID_HV_CURRENT): {
				int count = document->bms_hv.current_count;
				if (count < document->bms_hv.current_size) {
					Primary_HV_CURRENT* message = (Primary_HV_CURRENT*) malloc(sizeof(Primary_HV_CURRENT));
					deserialize_Primary_HV_CURRENT(data, 8, message);
					
					
					document->bms_hv.current[count].timestamp = getCurrentTimestamp();
					document->bms_hv.current[count].value.current = message->current;
					document->bms_hv.current[count].value.power = message->power;
					++document->bms_hv.current_count;
				}
				break;
			}
			
			case (ID_HV_TEMP): {
				int count = document->bms_hv.temperature_count;
				if (count < document->bms_hv.temperature_size) {
					Primary_HV_TEMP* message = (Primary_HV_TEMP*) malloc(sizeof(Primary_HV_TEMP));
					deserialize_Primary_HV_TEMP(data, 8, message);
					
					
					document->bms_hv.temperature[count].timestamp = getCurrentTimestamp();
					document->bms_hv.temperature[count].value.avg = message->average_temp;
					document->bms_hv.temperature[count].value.max = message->max_temp;
					document->bms_hv.temperature[count].value.min = message->min_temp;
					++document->bms_hv.temperature_count;
				}
				break;
			}
			
			case (ID_LV_CURRENT): {
				int count = document->bms_lv.current_count;
				if (count < document->bms_lv.current_size) {
					Primary_LV_CURRENT* message = (Primary_LV_CURRENT*) malloc(sizeof(Primary_LV_CURRENT));
					deserialize_Primary_LV_CURRENT(data, 8, message);
					
					
					document->bms_lv.current[count].timestamp = getCurrentTimestamp();
					document->bms_lv.current[count].value = message->current;
					++document->bms_lv.current_count;
				}
				break;
			}
			
			case (ID_LV_VOLTAGE): {
				int count = document->bms_lv.voltage_count;
				if (count < document->bms_lv.voltage_size) {
					Primary_LV_VOLTAGE* message = (Primary_LV_VOLTAGE*) malloc(sizeof(Primary_LV_VOLTAGE));
					deserialize_Primary_LV_VOLTAGE(data, 8, message);
					
					
					document->bms_lv.voltage[count].timestamp = getCurrentTimestamp();
					document->bms_lv.voltage[count].value.total_voltage = message->total_voltage;
					document->bms_lv.voltage[count].value.voltage_1 = message->voltage_1;
					document->bms_lv.voltage[count].value.voltage_2 = message->voltage_2;
					document->bms_lv.voltage[count].value.voltage_3 = message->voltage_3;
					document->bms_lv.voltage[count].value.voltage_4 = message->voltage_4;
					++document->bms_lv.voltage_count;
				}
				break;
			}
			
			case (ID_LV_TEMPERATURE): {
				int count = document->bms_lv.temperature_count;
				if (count < document->bms_lv.temperature_size) {
					Primary_LV_TEMPERATURE* message = (Primary_LV_TEMPERATURE*) malloc(sizeof(Primary_LV_TEMPERATURE));
					deserialize_Primary_LV_TEMPERATURE(data, 8, message);
					
					
					document->bms_lv.temperature[count].timestamp = getCurrentTimestamp();
					document->bms_lv.temperature[count].value.battery = message->battery_temperature;
					document->bms_lv.temperature[count].value.dcdc = message->dcdc_temperature;
					++document->bms_lv.temperature_count;
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