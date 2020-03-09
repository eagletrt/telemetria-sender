#include "log_service.h"

void debugGpsPort() {
    if (condition.verbose) {
        char *message;
        asprintf(&message, "Gps port is %d", condition.gps.port);
        logDebug(message);
        free(message);
    }
}

void debugGeneric(char* message) {
    if (condition.verbose) {
        logDebug(message);
    }
}

void infoConfigPath() {
    char *message;
    asprintf(&message, "Config file path is %s", condition.config_path);
    logInfo(message);
    free(message);
}

void infoTransition(int from, int to, char** labels) {  
  if (from != to) {
    char* from_label = strdup(labels[from]);
    char* to_label = strdup(labels[to]);
    char* message;
    asprintf(&message, "Transition from %s to %s", from_label, to_label);
    logInfo(message);   
    free(from_label);
    free(to_label);
    free(message);
  }
}

void infoStartingUp() {
    logInfo("Starting telemetry up");
}

void infoGpsPlugged() {
    logInfo("Gps is plugged to telemetry");
}

void infoNewSession() {
    char* message;
	asprintf(&message, "Created new session %s", condition.mongodb.instance->session_name);
    logInfo(message);
    free(message);
}

void infoQuitting() {
    logInfo("Quitting telemetry");
}

void successStartedUp() {
    logSuccess("Started up telemetry");
}

void successInsertion(int size) {
    char* message;
	asprintf(&message, "Inserted document, size: %d B", size);
    logSuccess(message);
    free(message);
}

void successQuitting() {
    logSuccess("Quitted telemetry");
}

void errorParsingConfig() {
    char *message;
    asprintf(&message, "Error in parsing config file %s", condition.config_path);
    logError(message);
    free(message);
}

void errorGatheringData() {
    logError("Error in gathering data");
}

void errorOpeningGPS() {
    logError("Error in opening GPS port");
}

void errorGeneric(char* message) {
    logError(message);
    free(message);
}