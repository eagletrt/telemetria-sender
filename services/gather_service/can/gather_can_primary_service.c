#include "gather_can_primary_service.h"

/* INTERNAL VARIABLES AND STRUCTS */

static pthread_t _gather_can_thread;
static pthread_attr_t _gather_can_thread_attr;

/* INTERNAL FUNCTIONS SIGNATURES */

static void* _parseCanMessages(void* args);

/* EXPORTED FUNCTIONS */

void gatherCanPrimaryStartThread(int enabled) {
	pthread_attr_init(&_gather_can_thread_attr);
	pthread_attr_setdetachstate(&_gather_can_thread_attr, PTHREAD_CREATE_JOINABLE);
	pthread_create(&_gather_can_thread, &_gather_can_thread_attr, &_parseCanMessages, (void*)((long)enabled));
	pthread_attr_destroy(&_gather_can_thread_attr);
}

void gatherCanPrimaryStopThread() {
	pthread_cancel(_gather_can_thread);
}

/* INTERNAL FUNCTIONS DEFINITIONS */

static void* _parseCanMessages(void* args) {
	// Getting enabled arg
	int enabled = (int)((long)args);

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
			case (ID_HV_TEMP): {
				int count = document->bms_hv.temperature_count;
				if (count < document->bms_hv.temperature_size) {
					Primary_HV_TEMP* message = (Primary_HV_TEMP*) malloc(sizeof(Primary_HV_TEMP));
					deserialize_Primary_HV_TEMP(data, 8, message);
					
					
					document->bms_hv.temperature[count].timestamp = getCurrentTimestamp();
					document->bms_hv.temperature[count].value.average_temp = message->average_temp;
					document->bms_hv.temperature[count].value.max_temp = message->max_temp;
					document->bms_hv.temperature[count].value.min_temp = message->min_temp;
					++document->bms_hv.temperature_count;
				}
				break;
			}
			
			case (ID_HV_VOLTAGE): {
				int count = document->bms_hv.voltage_count;
				if (count < document->bms_hv.voltage_size) {
					Primary_HV_VOLTAGE* message = (Primary_HV_VOLTAGE*) malloc(sizeof(Primary_HV_VOLTAGE));
					deserialize_Primary_HV_VOLTAGE(data, 8, message);
					
					
					document->bms_hv.voltage[count].timestamp = getCurrentTimestamp();
					document->bms_hv.voltage[count].value.pack_voltage = message->pack_voltage;
					document->bms_hv.voltage[count].value.bus_voltage = message->bus_voltage;
					document->bms_hv.voltage[count].value.min_cell_voltage = message->min_cell_voltage;
					document->bms_hv.voltage[count].value.max_cell_voltage = message->max_cell_voltage;
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
			
			case (ID_HV_ERROR): {
				int count = document->bms_hv.errors_count;
				if (count < document->bms_hv.errors_size) {
					Primary_HV_ERROR* message = (Primary_HV_ERROR*) malloc(sizeof(Primary_HV_ERROR));
					deserialize_Primary_HV_ERROR(data, 8, message);
					
					
					document->bms_hv.errors[count].timestamp = getCurrentTimestamp();
					document->bms_hv.errors[count].value.error_code = message->error_code;
					document->bms_hv.errors[count].value.error_index = message->error_index;
					document->bms_hv.errors[count].value.active = message->active;
					++document->bms_hv.errors_count;
				}
				break;
			}
			
			
		}

		// Unlock document
		pthread_mutex_unlock(&condition.structure.threads.data_head_mutex);
	}

	return NULL;
}
