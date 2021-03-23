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
			case (ID_HV_VOLTAGE):
				if (document->bms_hv.voltage_count < document->bms_hv.voltage_size) {
					HV_VOLTAGE_struct_t parsed = HV_VOLTAGE_as_root(data);
					document->bms_hv.voltage[document->bms_hv.voltage_count].timestamp = getCurrentTimestamp();
					document->bms_hv.voltage[document->bms_hv.voltage_count].value.bus_voltage = parsed->bus_voltage;
					document->bms_hv.voltage[document->bms_hv.voltage_count].value.pack_voltage = parsed->pack_voltage;
					document->bms_hv.voltage[document->bms_hv.voltage_count].value.max_cell_voltage = parsed->max_cell_voltage;
					document->bms_hv.voltage[document->bms_hv.voltage_count].value.min_cell_voltage = parsed->min_cell_voltage;
					++(document->bms_hv.voltage_count);
				}
				break;

			case (ID_HV_TEMP):
				if (document->bms_hv.temperature_count < document->bms_hv.temperature_size) {
					HV_TEMP_struct_t parsed = HV_TEMP_as_root(data);
					document->bms_hv.temperature[document->bms_hv.temperature_count].timestamp = getCurrentTimestamp();
					document->bms_hv.temperature[document->bms_hv.temperature_count].value.average_temp = parsed->average_temp;
					document->bms_hv.temperature[document->bms_hv.temperature_count].value.max_temp = parsed->max_temp;
					document->bms_hv.temperature[document->bms_hv.temperature_count].value.min_temp = parsed->min_temp;
					++(document->bms_hv.temperature_count);
				}
				break;

			case (ID_HV_CURRENT):
				if (document->bms_hv.current_count < document->bms_hv.current_size) {
					HV_CURRENT_struct_t parsed = HV_CURRENT_as_root(data);
					document->bms_hv.current[document->bms_hv.current_count].timestamp = getCurrentTimestamp();
					document->bms_hv.current[document->bms_hv.current_count].value.current = parsed->current;
					document->bms_hv.current[document->bms_hv.current_count].value.power = parsed->power;
					++(document->bms_hv.current_count);
				}
				break;

			case (ID_HV_ERROR):
				if (document->bms_hv.errors_count < document->bms_hv.errors_size) {
					HV_ERROR_struct_t parsed = HV_ERROR_as_root(data);
					document->bms_hv.errors[document->bms_hv.errors_count].timestamp = getCurrentTimestamp();
					document->bms_hv.errors[document->bms_hv.errors_count].value.active = parsed->active;
					document->bms_hv.errors[document->bms_hv.errors_count].value.error_code = parsed->error_code;
					document->bms_hv.errors[document->bms_hv.errors_count].value.error_index = parsed->error_index;
					++(document->bms_hv.errors_count);
				}
				break;
		}

		// Unlock document
		pthread_mutex_unlock(&condition.structure.threads.data_head_mutex);
	}

	return NULL;
}
