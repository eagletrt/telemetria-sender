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
					int bus_voltage = HV_VOLTAGE_bus_voltage(data);
					int pack_voltage = HV_VOLTAGE_pack_voltage(data);
					int max_cell_voltage = HV_VOLTAGE_max_cell_voltage(data);
					int min_cell_voltage = HV_VOLTAGE_min_cell_voltage(data);

					document->bms_hv.voltage[document->bms_hv.voltage_count].timestamp = getCurrentTimestamp();
					document->bms_hv.voltage[document->bms_hv.voltage_count].value.bus_voltage = bus_voltage;
					document->bms_hv.voltage[document->bms_hv.voltage_count].value.pack_voltage = pack_voltage;
					document->bms_hv.voltage[document->bms_hv.voltage_count].value.max_cell_voltage = max_cell_voltage;
					document->bms_hv.voltage[document->bms_hv.voltage_count].value.min_cell_voltage = min_cell_voltage;
					++(document->bms_hv.voltage_count);
				}
				break;

			case (ID_HV_TEMP):
				if (document->bms_hv.temperature_count < document->bms_hv.temperature_size) {
					int average_temp = HV_TEMP_average_temp(data);
					int min_temp = HV_TEMP_max_temp(data);
					int max_temp = HV_TEMP_min_temp(data);

					document->bms_hv.temperature[document->bms_hv.temperature_count].timestamp = getCurrentTimestamp();
					document->bms_hv.temperature[document->bms_hv.temperature_count].value.average_temp = average_temp;
					document->bms_hv.temperature[document->bms_hv.temperature_count].value.max_temp = max_temp;
					document->bms_hv.temperature[document->bms_hv.temperature_count].value.min_temp = min_temp;
					++(document->bms_hv.temperature_count);
				}
				break;

			case (ID_HV_CURRENT):
				if (document->bms_hv.current_count < document->bms_hv.current_size) {
					int current = HV_CURRENT_current(data);
					int power = HV_CURRENT_power(data);

					document->bms_hv.current[document->bms_hv.current_count].timestamp = getCurrentTimestamp();
					document->bms_hv.current[document->bms_hv.current_count].value.current = current;
					document->bms_hv.current[document->bms_hv.current_count].value.power = power;
					++(document->bms_hv.current_count);
				}
				break;

			case (ID_HV_ERROR):
				if (document->bms_hv.errors_count < document->bms_hv.errors_size) {
					int active = HV_ERROR_active(data);
					int error_code = HV_ERROR_error_code(data);
					int error_index = HV_ERROR_error_index(data);

					document->bms_hv.errors[document->bms_hv.errors_count].timestamp = getCurrentTimestamp();
					document->bms_hv.errors[document->bms_hv.errors_count].value.active = active;
					document->bms_hv.errors[document->bms_hv.errors_count].value.error_code = error_code;
					document->bms_hv.errors[document->bms_hv.errors_count].value.error_index = error_index;
					++(document->bms_hv.errors_count);
				}
				break;
		}

		// Unlock document
		pthread_mutex_unlock(&condition.structure.threads.data_head_mutex);
	}

	return NULL;
}
