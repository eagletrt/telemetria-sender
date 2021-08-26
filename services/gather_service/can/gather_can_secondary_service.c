#include "gather_can_secondary_service.h"

/* INTERNAL VARIABLES AND STRUCTS */

static pthread_t _gather_can_thread;
static pthread_attr_t _gather_can_thread_attr;

/* INTERNAL FUNCTIONS SIGNATURES */

static void* _parseCanMessages();

/* EXPORTED FUNCTIONS */

void gatherCanSecondaryStartThread() {
	pthread_attr_init(&_gather_can_thread_attr);
	pthread_attr_setdetachstate(&_gather_can_thread_attr, PTHREAD_CREATE_JOINABLE);
	pthread_create(&_gather_can_thread, &_gather_can_thread_attr, &_parseCanMessages, NULL);
	pthread_attr_destroy(&_gather_can_thread_attr);
}

void gatherCanSecondaryStopThread() {
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
		canReadSecondary(&id, &data);

		// Lock document
		pthread_mutex_lock(&condition.structure.threads.data_head_mutex);

		// Create reference to data_head
		document = condition.structure.data_head;

		switch (id) {
			case (ID_BMS_HV_VOLTAGE): {
				int count = document->bms_hv.voltage_count;
				if (count < document->bms_hv.voltage_size) {
					Secondary_BMS_HV_VOLTAGE* message = (Secondary_BMS_HV_VOLTAGE*) malloc(sizeof(Secondary_BMS_HV_VOLTAGE));
					deserialize_Secondary_BMS_HV_VOLTAGE(data, message);
					
					
					document->bms_hv.voltage[count].timestamp = getCurrentTimestamp();
					document->bms_hv.voltage[count].value.total = message->total / 10000;
					document->bms_hv.voltage[count].value.max = message->max / 10000;
					document->bms_hv.voltage[count].value.min = message->min / 10000;
					++document->bms_hv.voltage_count;
				}
				break;
			}
			
			case (ID_BMS_HV_TEMPERATURE): {
				int count = document->bms_hv.temperature_count;
				if (count < document->bms_hv.temperature_size) {
					Secondary_BMS_HV_TEMPERATURE* message = (Secondary_BMS_HV_TEMPERATURE*) malloc(sizeof(Secondary_BMS_HV_TEMPERATURE));
					deserialize_Secondary_BMS_HV_TEMPERATURE(data, message);
					
					
					document->bms_hv.temperature[count].timestamp = getCurrentTimestamp();
					document->bms_hv.temperature[count].value.average = message->average / 100;
					document->bms_hv.temperature[count].value.max = message->max / 100;
					document->bms_hv.temperature[count].value.min = message->min / 100;
					++document->bms_hv.temperature_count;
				}
				break;
			}
			
			case (ID_BMS_HV_CURRENT): {
				int count = document->bms_hv.current_count;
				if (count < document->bms_hv.current_size) {
					Secondary_BMS_HV_CURRENT* message = (Secondary_BMS_HV_CURRENT*) malloc(sizeof(Secondary_BMS_HV_CURRENT));
					deserialize_Secondary_BMS_HV_CURRENT(data, message);
					
					
					document->bms_hv.current[count].timestamp = getCurrentTimestamp();
					document->bms_hv.current[count].value.current = message->current;
					document->bms_hv.current[count].value.pow = message->pow / 10;
					++document->bms_hv.current_count;
				}
				break;
			}
			
			case (ID_BMS_HV_ERRORS): {
				int count = document->bms_hv.errors_count;
				if (count < document->bms_hv.errors_size) {
					Secondary_BMS_HV_ERRORS* message = (Secondary_BMS_HV_ERRORS*) malloc(sizeof(Secondary_BMS_HV_ERRORS));
					deserialize_Secondary_BMS_HV_ERRORS(data, message);
					
					
					document->bms_hv.errors[count].timestamp = getCurrentTimestamp();
					document->bms_hv.errors[count].value.fault_id = message->fault_id;
					document->bms_hv.errors[count].value.fault_index = message->fault_index;
					++document->bms_hv.errors_count;
				}
				break;
			}
			
			case (ID_BMS_HV_WARNINGS): {
				int count = document->bms_hv.warnings_count;
				if (count < document->bms_hv.warnings_size) {
					Secondary_BMS_HV_WARNINGS* message = (Secondary_BMS_HV_WARNINGS*) malloc(sizeof(Secondary_BMS_HV_WARNINGS));
					deserialize_Secondary_BMS_HV_WARNINGS(data, message);
					
					
					document->bms_hv.warnings[count].timestamp = getCurrentTimestamp();
					document->bms_hv.warnings[count].value.fault_id = message->fault_id;
					document->bms_hv.warnings[count].value.fault_index = message->fault_index;
					++document->bms_hv.warnings_count;
				}
				break;
			}
			
			
		}

		// Unlock document
		pthread_mutex_unlock(&condition.structure.threads.data_head_mutex);
	}

	return NULL;
}