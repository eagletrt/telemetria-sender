#include "state_machine.h"

result_codes (*state[])() = { init_state, restart_state, idle_state, enabled_state, exit_state };

transition_t state_transitions[] = {
    {   INIT,       INITIALIZED,    IDLE },
    {   INIT,       ERROR,          EXIT },
    {   IDLE,       REPEAT,         IDLE },
    {   IDLE,       TOGGLE,         RESTART },
    {   IDLE,       ERROR,          EXIT },
    {   ENABLED,    REPEAT,         ENABLED },
    {   ENABLED,    TOGGLE,         RESTART },
    {   ENABLED,    ERROR,          EXIT },
    {   RESTART,    DISABLE,        IDLE },
    {   RESTART,    ENABLE,         ENABLED },
    {   RESTART,    ERROR,          EXIT }
};


result_codes init_state() {
    infoStartingUp();

    debugConfigPath();
    config_code config_outcome = handleConfig();
    if (config_outcome != CONFIG_OK) {
        errorParsingConfig();
        return ERROR;
    }

    can_code can_primary_outcome = canSetupPrimary();
    if (can_primary_outcome != CAN_SERVICE_OK) {
        char* message = canErrorMessage(can_primary_outcome);
        errorGeneric(message);
        return ERROR;
    }

    can_code can_secondary_outcome = canSetupSecondary();
    if (can_secondary_outcome != CAN_SERVICE_OK) {
        char* message = canErrorMessage(can_secondary_outcome);
        errorGeneric(message);
        return ERROR;
    }

    mongo_code mongo_outcome = mongoSetup();
    if (mongo_outcome != MONGO_OK) {
        char* message = mongoErrorMessage(mongo_outcome);
        errorGeneric(message);
        return ERROR;
    }

    mosquitto_code mosquitto_outcome = mosquittoSetup();
    if (mosquitto_outcome != MOSQUITTO_OK) {
        char* message = mosquittoErrorMessage(mosquitto_outcome);
        errorGeneric(message);
        return ERROR;
    }

    if (condition.gps.plugged) {
        infoGpsPlugged();
        condition.gps.port = openGPSPort();
        if (condition.gps.port < 0) {
            errorOpeningGPS();
            return ERROR;
        }
        if (condition.gps.simulated) {
            int outcome = prepareSimulatedPort();
            if (outcome < 0) {
                errorOpeningGPS();
                return ERROR;
            }
        }
        debugGpsPort();
    }

    gatherSetup();
    gatherCanPrimaryStartThread();
    gatherCanSecondaryStartThread();
    gatherGpsStartThread();
    gatherSenderStartThread();
    
    successStartedUp();
    mosquittoLogStartup();

    return INITIALIZED;
}

result_codes restart_state() {
    gatherSetupRestart();

    return condition.structure.enabled ? ENABLE : DISABLE;
}

result_codes idle_state() { 
    debugGeneric("{MASTER} Resetting structure id");
    gatherResetDataId();

    debugGeneric("{MASTER} Waiting the milliseconds");
    gatherMasterWait();

    debugGeneric("{MASTER} Swapping data head and data tail");
    gatherMasterSwap();

    debugGeneric("{MASTER} Resetting toilet flushed");
    gatherMasterResetToiletFlushed();

    debugGeneric("{MASTER} Enabling flush toilet");
    gatherMasterEnableFlushToilet();

    if (condition.structure.toggle_state) {
        debugGeneric("{MASTER} Waiting for toilet to be flushed");
        gatherMasterWaitToiletFlushed();
        debugGeneric("{MASTER} Set telemetry enabled to true");
        condition.structure.enabled = 1;
        gatherStartNewSession(1);
        gatherAnswerWheel(1);
        return TOGGLE;
    }
    else {
        return REPEAT;
    }
}

result_codes enabled_state() {
    debugGeneric("{MASTER} Waiting the milliseconds");
    gatherMasterWait();

    debugGeneric("{MASTER} Swapping data head and data tail");
    gatherMasterSwap();

    debugGeneric("{MASTER} Resetting toilet flushed");
    gatherMasterResetToiletFlushed();

    debugGeneric("{MASTER} Enabling flush toilet");
    gatherMasterEnableFlushToilet();

    if (condition.structure.toggle_state) {
        gatherMasterWaitToiletFlushed();
        condition.structure.enabled = 0;
        gatherAnswerWheel(0);
        return TOGGLE;
    }
    else {
        return REPEAT;
    }
}

result_codes exit_state() {
    infoQuitting();
    mosquittoLogQuit();
    mongoQuit();
    mosquittoQuit();
    successQuitting();
}

state_codes lookup_transitions(state_codes current_state, result_codes result_code) {
    int n = sizeof(state_transitions) / sizeof(transition_t);
    char* state_labels[5] = { "INIT", "RESTART", "IDLE", "ENABLED", "EXIT" }; 
    
    transition_t transition;
    for (int i = 0; i < n; ++i) {
        transition = state_transitions[i];
        if (transition.from_state == current_state && transition.result_code == result_code) {
            infoTransition(transition.from_state, transition.to_state, state_labels);
            mosquittoLogTransition(transition.from_state, transition.to_state, state_labels);
            return transition.to_state;
        }
    }

    return EXIT;
}