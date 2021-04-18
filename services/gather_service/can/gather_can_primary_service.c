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
					Primary_HV_VOLTAGE* value = (Primary_HV_VOLTAGE*) malloc(sizeof(Primary_HV_VOLTAGE));
					deserialize_Primary_HV_VOLTAGE(data, 8, value);
					document->bms_hv.voltage[document->bms_hv.voltage_count].timestamp = getCurrentTimestamp();
					document->bms_hv.voltage[document->bms_hv.voltage_count].value.pack_voltage = value->pack_voltage;
					document->bms_hv.voltage[document->bms_hv.voltage_count].value.bus_voltage = value->bus_voltage;
					document->bms_hv.voltage[document->bms_hv.voltage_count].value.max_cell_voltage = value->max_cell_voltage;
					document->bms_hv.voltage[document->bms_hv.voltage_count].value.min_cell_voltage = value->min_cell_voltage;
					++(document->bms_hv.voltage_count);
				}
				break;
		}

		// Unlock document
		pthread_mutex_unlock(&condition.structure.threads.data_head_mutex);
	}

	return NULL;
}
