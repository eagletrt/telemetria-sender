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
			case (ID_IMU_ACCELERATION): {
				int count = document->imu.acceleration_count;
				if (count < document->imu.acceleration_size) {
					Secondary_IMU_ACCELERATION* message = (Secondary_IMU_ACCELERATION*) malloc(sizeof(Secondary_IMU_ACCELERATION));
					deserialize_Secondary_IMU_ACCELERATION(data, 8, message);
					
					
					document->imu.acceleration[count].timestamp = getCurrentTimestamp();
					document->imu.acceleration[count].value.accel_x = message->accel_x;
					document->imu.acceleration[count].value.accel_y = message->accel_y;
					document->imu.acceleration[count].value.accel_z = message->accel_z;
					++document->imu.acceleration_count;
				}
				break;
			}
			
			
		}

		// Unlock document
		pthread_mutex_unlock(&condition.structure.threads.data_head_mutex);
	}

	return NULL;
}