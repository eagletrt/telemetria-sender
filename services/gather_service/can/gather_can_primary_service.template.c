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
			// {{GENERATE_STRUCTURE_CAN_GATHERER_PRIMARY}}
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

		// 			if (status == 0) {
		// 				if (condition.structure.enabled == 0) {
		// 					logWarning("Error in structure: telemetry already disabled");
		// 				}
		// 				else {
		// 					pthread_mutex_lock(&condition.structure.threads.toggle_state_mutex);
		// 					condition.structure.toggle_state = 1;
		// 					pthread_mutex_unlock(&condition.structure.threads.toggle_state_mutex);
		// 				}
		// 			}
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