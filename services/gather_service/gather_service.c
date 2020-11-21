#include "gather_service.h"

/* INTERNAL FUNCTIONS SIGNATURES */

static void initMutexesAndConds();
static void destroyMutexesAndConds();

/* EXPORTED FUNCTIONS */

void gatherSetup() {
    condition.structure.flush_toilet = 1;
    condition.structure.toilet_flushed = 0;
    condition.structure.enabled = 0;
    condition.structure.toggle_state = 0;
    condition.structure.data_head = gatherCreateData();
    condition.structure.data_tail = NULL;

    initMutexesAndConds();
}

void gatherSetupRestart() {
    condition.structure.flush_toilet = 0;
    condition.structure.toilet_flushed = 0;
    condition.structure.toggle_state = 0;

    destroyMutexesAndConds();
    initMutexesAndConds();

    if (condition.structure.data_head != NULL) {
        gatherDeleteData(condition.structure.data_head);
    }
    condition.structure.data_head = gatherCreateData();

    if (condition.structure.data_tail != NULL) {
        gatherDeleteData(condition.structure.data_tail);
    }
    condition.structure.data_tail = gatherCreateData();
}

void gatherResetDataId() {
    condition.structure.id = 0;
}

/* INTERNAL FUNCTIONS DEFINITIONS */

static void initMutexesAndConds() {
    pthread_mutex_init(&condition.structure.threads.data_head_mutex, NULL);
    pthread_mutex_init(&condition.structure.threads.data_tail_mutex, NULL);
    pthread_mutex_init(&condition.structure.threads.flush_toilet_mutex, NULL);
    pthread_mutex_init(&condition.structure.threads.toilet_flushed_mutex, NULL);
    pthread_mutex_init(&condition.structure.threads.toggle_state_mutex, NULL);
    pthread_cond_init(&condition.structure.threads.flush_toilet_cond, NULL);
    pthread_cond_init(&condition.structure.threads.toilet_flushed_cond, NULL);
}

static void destroyMutexesAndConds() {
    pthread_mutex_destroy(&condition.structure.threads.data_head_mutex);
    pthread_mutex_destroy(&condition.structure.threads.data_tail_mutex);
    pthread_mutex_destroy(&condition.structure.threads.flush_toilet_mutex);
    pthread_mutex_destroy(&condition.structure.threads.toilet_flushed_mutex);
    pthread_mutex_destroy(&condition.structure.threads.toggle_state_mutex);
    pthread_cond_destroy(&condition.structure.threads.flush_toilet_cond);
    pthread_cond_destroy(&condition.structure.threads.toilet_flushed_cond);
}
