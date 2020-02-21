#include "config_service.h"

/* INTERNAL FUNCTIONS */

static void updateCondition(const config_t* config) {
    condition.mqtt.host = strdup(config->mqtt.host);
    condition.mqtt.port = config->mqtt.port;
    condition.mqtt.data_topic = strdup(config->mqtt.data_topic);
    condition.mqtt.log_topic = strdup(config->mqtt.log_topic);

    condition.mongodb.host = strdup(config->mongodb.host);
    condition.mongodb.port = config->mongodb.port;
    condition.mongodb.db = strdup(config->mongodb.db);

    condition.can.can_interface = strdup(config->can_interface);
    condition.gps_plugged = config->gps_plugged;
    condition.verbose = config->verbose;

    condition.session.pilots = cloneStringsArray(config->pilots, config->pilots_count);
    condition.session.pilots_count = config->pilots_count;

    condition.session.races = cloneStringsArray(config->races, config->races_count);
    condition.session.races_count = config->races_count;

    condition.structure.sending_rate = config->sending_rate;
}

/* EXPORTED FUNCTIONS */

config_code handleConfig() {
    config_code result;

    config_t* config = newConfig();
    parseConfigFile(condition.config_path, &config);
    
    if (config != NULL) {
        updateCondition(config);
        printConfig(config);
        deleteConfig(config);
        result = CONFIG_OK;
    }
    else {
        result = CONFIG_ERROR;
    }

    return result;
}