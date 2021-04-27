#include "gather_sender_service.h"

/* INTERNAL VARIABLES AND STRUCTS */

pthread_t gather_sender_thread;
pthread_attr_t gather_sender_thread_attr;

/* INTERNAL FUNCTIONS SIGNATURES */

static void* senderSend(void *args);

/* EXPORTED FUNCTIONS */

void gatherSenderStartThread() {
	pthread_attr_init(&gather_sender_thread_attr);
	pthread_attr_setdetachstate(&gather_sender_thread_attr, PTHREAD_CREATE_JOINABLE);
	pthread_create(&gather_sender_thread, &gather_sender_thread_attr, &senderSend, NULL);
	pthread_attr_destroy(&gather_sender_thread_attr);
}

void gatherSenderStopThread() {
	pthread_cancel(gather_sender_thread);
}

/* INTERNAL FUNCTIONS DEFINITIONS */

static void* senderSend(void *args) {
    // Get enabled state
    // int enabled = (int) ((long) args);

    while (1) {
        // Waits until it must flush the toilet
        debugGeneric("{SENDER} Waiting for flushing toilet");
        printf("--- SENDER ce l'ha %d \n", condition.structure.threads.flush_toilet_mutex.__data.__owner);
        puts("--- SENDER locking");
        pthread_mutex_lock(&condition.structure.threads.flush_toilet_mutex);
        puts("--- SENDER locked");
        printf("--- SENDER e ora ce l'ha %d \n", condition.structure.threads.flush_toilet_mutex.__data.__owner);
        while (!condition.structure.flush_toilet) {
            puts("--- SENDER waiting");
            pthread_cond_wait(&condition.structure.threads.flush_toilet_cond, &condition.structure.threads.flush_toilet_mutex);
            puts("--- SENDER waited");
        }
        condition.structure.flush_toilet = 0;
        puts("--- SENDER unlocking");
        pthread_mutex_unlock(&condition.structure.threads.flush_toilet_mutex);
        puts("--- SENDER unlocked");

        // Locks data_tail
        debugGeneric("{SENDER} Locking data tail");
        pthread_mutex_lock(&condition.structure.threads.data_tail_mutex);

        if (condition.structure.data_tail != NULL) {
            // Get the bson document from data_tail
            debugGeneric("{SENDER} Creating bson document");
            bson_t* bson_document;  
            gatherDataToBson(condition.structure.data_tail, &bson_document);

            // Sends the data over mosquitto
            debugGeneric("{SENDER} Sending over mosquitto");
            mosquittoSend(bson_document);
            
            if (condition.structure.enabled) {
                debugGeneric("{SENDER} Inserting to mongo");
                mongoInsert(bson_document);
                size_t size; bson_as_relaxed_extended_json(bson_document, &size);
                successInsertion(size);
                mosquittoLogInsertion(size);
            }

            // Destroys the bson document
            debugGeneric("{SENDER} Destroying bson document");
            bson_destroy(bson_document);

            // Deletes the data_tail document and replaces it with a new one
            debugGeneric("{SENDER} Deleting data tail");
            gatherDeleteData(condition.structure.data_tail);
        }

        // Resets the data_tail document and replaces it with a new one
        debugGeneric("{SENDER} Resetting data tail");
        condition.structure.data_tail = gatherCreateData();

        // Unlocks data tail
        debugGeneric("{SENDER} Unlocking data tail");
        pthread_mutex_unlock(&condition.structure.threads.data_tail_mutex);

        // Toilet is flushed
        debugGeneric("{SENDER} Setting toilet flushed to true");
        pthread_mutex_lock(&condition.structure.threads.toilet_flushed_mutex);
        condition.structure.toilet_flushed = 1;
        pthread_cond_signal(&condition.structure.threads.toilet_flushed_cond);
        pthread_mutex_unlock(&condition.structure.threads.toilet_flushed_mutex);
    }
}

