#include "gather_can_primary_service.h"

#ifndef IDS_H
#define IDS_H

#define NETWORK_VERSION 1.2f

/* TOPIC ACU */
#define TOPIC_ACU_MASK 0b00000011111
#define TOPIC_ACU_FILTER 0b00000000000
#define ID_TS_STATUS 0b00000000000
#define ID_STEER_STATUS 0b00000100000
#define ID_SET_CAR_STATUS 0b00001000000
#define ID_GET_CAR_STATUS 0b00001100000

/* TOPIC ACUnSTEER */
#define TOPIC_ACUnSTEER_MASK 0b00000011111
#define TOPIC_ACUnSTEER_FILTER 0b00000000001
#define ID_HV_VOLTAGE 0b01100000001
#define ID_HV_CURRENT 0b01100100001
#define ID_HV_TEMP 0b01101000001
#define ID_HV_ERROR 0b00100000001

/* TOPIC BMS_HV */
#define TOPIC_BMS_HV_MASK 0b00000011111
#define TOPIC_BMS_HV_FILTER 0b00000000010
#define ID_SET_TS_STATUS 0b00000000010

/* TOPIC STEER */
#define TOPIC_STEER_MASK 0b00000011111
#define TOPIC_STEER_FILTER 0b00000000011
#define ID_TLM_STATUS 0b00100000011
#define ID_CAR_STATUS 0b00000000011

/* TOPIC TLM */
#define TOPIC_TLM_MASK 0b00000011111
#define TOPIC_TLM_FILTER 0b00000000100
#define ID_SET_TLM_STATUS 0b00100000100

#endif

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
					HV_VOLTAGE* value = (HV_VOLTAGE*) malloc(sizeof(HV_VOLTAGE));
					deserialize_HV_VOLTAGE(data, 8, value);
					document->bms_hv.voltage[document->bms_hv.voltage_count].timestamp = getCurrentTimestamp();
					document->bms_hv.voltage[document->bms_hv.voltage_count].value.bus_voltage = value->bus_voltage;
					document->bms_hv.voltage[document->bms_hv.voltage_count].value.pack_voltage = value->pack_voltage;
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
