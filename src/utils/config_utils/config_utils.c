#include "config_utils.h"

/* INTERNAL FUNCTIONS SIGNATURES AND CONSTANTS */

static const int MAX_TOKENS_COUNT = 100;
static int getJsonString(FILE *config_file, char **json_string);
static int getJsonTokens(const char* json_string, int json_length, jsmntok_t** json_tokens);
static char* extractString(jsmntok_t token, const char *json_string);
static char* getStringValue(const jsmntok_t *json_tokens, const char *json_string, int *i);
static int getIntValue(const jsmntok_t *json_tokens, const char *json_string, int *i);
static char** getArrayValue(const jsmntok_t *json_tokens, const char *json_string, int *size, int *i);
static void parseMqttObject(const jsmntok_t *json_tokens, const char *json_string, config_t *config, int *i);
static void parseMongodbObject(const jsmntok_t *json_tokens, const char *json_string, config_t *config, int *i);
static void parseJsonTokens(const jsmntok_t *json_tokens, int tokens_length, const char *json_string, config_t *config);

/* EXPORTED FUNCTIONS */

config_t* newConfig() {
    config_t *config = (config_t*) malloc(sizeof(config_t));
    
    config->mqtt.host = strdup("localhost");
    config->mqtt.port = 1883;
    config->mqtt.data_topic = strdup("telemetria");
    config->mqtt.log_topic = strdup("telemetria_log");

    config->mongodb.host = strdup("localhost");
    config->mongodb.port = 27017;
    config->mongodb.db = strdup("eagle_test");
    config->mongodb.collection = strdup("chimera");

    config->can_interface = strdup("can0");
    config->sending_rate = 500;
    config->gps.plugged = 0;
    config->gps.simulated = 0;
    config->gps.interface = strdup("/dev/ttyACM0");
    config->verbose = 1;

    config->pilots_count = 3;
    config->pilots = (char**) malloc(sizeof(char*) * config->pilots_count);
    config->pilots[0] = strdup("default");
    config->pilots[1] = strdup("Ivan");
    config->pilots[2] = strdup("Gastaldello");

    config->races_count = 5;
    config->races = (char**) malloc(sizeof(char*) * config->races_count);
    config->races[0] = strdup("default");
    config->races[1] = strdup("Autocross");
    config->races[2] = strdup("Skidpad");
    config->races[3] = strdup("Endurance");
    config->races[4] = strdup("Acceleration");

    config->circuits_count = 4;
    config->circuits = (char**) malloc(sizeof(char*) * config->circuits_count);
    config->circuits[0] = strdup("default");
    config->circuits[1] = strdup("Vadena");
    config->circuits[2] = strdup("Varano");
    config->circuits[3] = strdup("Povo");

    return config;
}

void parseConfigFile(const char* path, config_t **config) {
    FILE *config_file = fopen(path, "r");

    if (config_file == NULL) {
        *config = NULL;
    }
    else {
        char *json_string;
        jsmntok_t *json_tokens;
        int json_length, tokens_length;
        json_length = getJsonString(config_file, &json_string);
        fclose(config_file);
        tokens_length = getJsonTokens(json_string, json_length, &json_tokens);
        parseJsonTokens(json_tokens, tokens_length, json_string, *config);
    }
}

void deleteConfig(config_t *config) {
    free(config->mqtt.host);
    free(config->mqtt.data_topic);
    free(config->mqtt.log_topic);

    free(config->mongodb.host);
    free(config->mongodb.db);
    free(config->mongodb.collection);

    free(config->gps.interface);

    free(config->can_interface);
    freeStringsArray(config->pilots, &config->pilots_count);
    freeStringsArray(config->races, &config->races_count);
    freeStringsArray(config->circuits, &config->circuits_count);
}

void printConfig(const config_t* config) {
    printf("config->mqtt.host:\t%s\n", config->mqtt.host);
    printf("config->mqtt.port:\t%d\n", config->mqtt.port);
    printf("config->mqtt.data_topic:\t%s\n", config->mqtt.data_topic);
    printf("config->mqtt.log_topic:\t%s\n", config->mqtt.log_topic);
    printf("config->mongodb.host:\t%s\n", config->mongodb.host);
    printf("config->mongodb.port:\t%d\n", config->mongodb.port);
    printf("config->mongodb.db:\t%s\n", config->mongodb.db);
    printf("config->mongodb.collection:\t%s\n", config->mongodb.collection);
    printf("config->gps.plugged:\t%d\n", config->gps.plugged);
    printf("config->gps.simulated:\t%d\n", config->gps.simulated);
    printf("config->gps.interface:\t%s\n", config->gps.interface);
    printf("config->can_interface:\t%s\n", config->can_interface);
    printf("config->sending_rate:\t%d\n", config->sending_rate);
    printf("config->verbose:\t%d\n", config->verbose);
    printf("config->pilots: ");
    printStringsArray(config->pilots, config->pilots_count);
    printf("config->races: ");
    printStringsArray(config->races, config->races_count);
    printf("config->circuits: ");
    printStringsArray(config->circuits, config->circuits_count);
}

/* INTERNAL FUNCTIONS DEFINITIONS */

static int getJsonString(FILE *config_file, char **json_string) {
    char* line;
    size_t lineLength = NULL;
    int json_length = 0;

    getline(&line, &lineLength, config_file);
    json_length += lineLength;
    *json_string = (char*) malloc(sizeof(char) * json_length);
	strcpy(*json_string, line);

    while (getline(&line, &lineLength, config_file) != -1) {       	
		json_length += lineLength;
		*json_string = (char*) realloc(*json_string, sizeof(char) * json_length);
		strcat(*json_string, line);
    }

    return json_length;
}

static int getJsonTokens(const char* json_string, int json_length, jsmntok_t** json_tokens) {
    jsmn_parser parser;
    jsmn_init(&parser);
    *json_tokens = (jsmntok_t*) malloc(sizeof(jsmntok_t) * MAX_TOKENS_COUNT);
    int tokens_length = jsmn_parse(&parser, json_string, json_length, *json_tokens, MAX_TOKENS_COUNT);
    return tokens_length;
}

static char* extractString(jsmntok_t token, const char *json_string) {
    unsigned int length = token.end - token.start;
    char* key = malloc(sizeof(char) * (length + 1));
	memcpy(key, &json_string[token.start], length);
	key[length] = '\0';
    return key;
}

static char* getStringValue(const jsmntok_t *json_tokens, const char *json_string, int *i) {
    ++(*i);
    const jsmntok_t token = json_tokens[*i];
    return extractString(token, json_string);
}

static int getIntValue(const jsmntok_t *json_tokens, const char *json_string, int *i) {
    ++(*i);
    const jsmntok_t token = json_tokens[*i];
    return atoi(extractString(token, json_string));
}

static char** getArrayValue(const jsmntok_t *json_tokens, const char *json_string, int *size, int *i) {
    char **string_array;

    ++(*i);
    *size = json_tokens[*i].size;
    string_array = (char**) malloc(sizeof(char*) * *size);

    for (int j = 0; j < *size; ++j) {
        ++(*i);
        char* child = extractString(json_tokens[*i], json_string);
        string_array[j] = child;
    }

    return string_array;
}

static void parseMqttObject(const jsmntok_t *json_tokens, const char *json_string, config_t *config, int *i) {
    ++(*i);
    int size = json_tokens[*i].size;

    for (int j = 0; j < size; ++j) {
        ++(*i);
        char* key = extractString(json_tokens[*i], json_string);
        if (strcmp(key, "host") == 0) {
            free(config->mqtt.host);
            config->mqtt.host = getStringValue(json_tokens, json_string, i);
        }
        else if (strcmp(key, "port") == 0) {
            config->mqtt.port = getIntValue(json_tokens, json_string, i);
        }
        else if (strcmp(key, "data_topic") == 0) {
            free(config->mqtt.data_topic);
            config->mqtt.data_topic = getStringValue(json_tokens, json_string, i);
        } 
        else if (strcmp(key, "log_topic") == 0) {
            free(config->mqtt.log_topic);
            config->mqtt.log_topic = getStringValue(json_tokens, json_string, i);
        }
        else {
            ++(*i);
            jsmntok_t token = json_tokens[*i];
            switch (token.type)
            {
                case JSMN_ARRAY:
                    *i += token.size;
                    break;
                case JSMN_OBJECT:
                    *i += 2 * token.size;
                    break;
            }
        } 
    }
}

static void parseMongodbObject(const jsmntok_t *json_tokens, const char *json_string, config_t *config, int *i) {
    ++(*i);
    int size = json_tokens[*i].size;

    for (int j = 0; j < size; ++j) {
        ++(*i);
        char* key = extractString(json_tokens[*i], json_string);
        if (strcmp(key, "host") == 0) {
            free(config->mongodb.host);
            config->mongodb.host = getStringValue(json_tokens, json_string, i);
        }
        else if (strcmp(key, "port") == 0) {
            config->mongodb.port = getIntValue(json_tokens, json_string, i);
        }
        else if (strcmp(key, "db") == 0) {
            free(config->mongodb.db);
            config->mongodb.db = getStringValue(json_tokens, json_string, i);
        }
        else if (strcmp(key, "collection") == 0) {
            free(config->mongodb.collection);
            config->mongodb.collection = getStringValue(json_tokens, json_string, i);
        } 
        else {
            ++(*i);
            jsmntok_t token = json_tokens[*i];
            switch (token.type)
            {
                case JSMN_ARRAY:
                    *i += token.size;
                    break;
                case JSMN_OBJECT:
                    *i += 2 * token.size;
                    break;
            }
        }
    }
}

static void parseGpsObject(const jsmntok_t *json_tokens, const char *json_string, config_t *config, int *i) {
    ++(*i);
    int size = json_tokens[*i].size;

    for (int j = 0; j < size; ++j) {
        ++(*i);
        char* key = extractString(json_tokens[*i], json_string);
        if (strcmp(key, "interface") == 0) {
            free(config->gps.interface);
            config->gps.interface = getStringValue(json_tokens, json_string, i);
        }
        else if (strcmp(key, "plugged") == 0) {
            config->gps.plugged = getIntValue(json_tokens, json_string, i);
        }
        else if (strcmp(key, "simulated") == 0) {
            config->gps.simulated = getIntValue(json_tokens, json_string, i);
        }
        else {
            ++(*i);
            jsmntok_t token = json_tokens[*i];
            switch (token.type)
            {
                case JSMN_ARRAY:
                    *i += token.size;
                    break;
                case JSMN_OBJECT:
                    *i += 2 * token.size;
                    break;
            }
        }
    }
}

static void parseJsonTokens(const jsmntok_t *json_tokens, int tokens_length, const char *json_string, config_t *config) {
    for (int i = 1; i < tokens_length; ++i) {
        char* key = extractString(json_tokens[i], json_string);

        if (strcmp(key, "can_interface") == 0) {
            free(config->can_interface);
            config->can_interface = getStringValue(json_tokens, json_string, &i);
        }
        else if (strcmp(key, "sending_rate") == 0) {
            config->sending_rate = getIntValue(json_tokens, json_string, &i);
        } 
        else if (strcmp(key, "verbose") == 0) {
            config->verbose = getIntValue(json_tokens, json_string, &i);
        } 
        else if (strcmp(key, "pilots") == 0) {
            freeStringsArray(config->pilots, &config->pilots_count);
            config->pilots = getArrayValue(json_tokens, json_string, &config->pilots_count, &i);
        }
        else if (strcmp(key, "races") == 0) {
            freeStringsArray(config->races, &config->races_count);
            config->races = getArrayValue(json_tokens, json_string, &config->races_count, &i);
        }
        else if (strcmp(key, "circuits") == 0) {
            freeStringsArray(config->circuits, &config->circuits_count);
            config->circuits = getArrayValue(json_tokens, json_string, &config->circuits_count, &i);
        }
        else if (strcmp(key, "mqtt") == 0) {
            parseMqttObject(json_tokens, json_string, config, &i);
        }
        else if (strcmp(key, "mongodb") == 0) {
            parseMongodbObject(json_tokens, json_string, config, &i);
        }
        else if (strcmp(key, "gps") == 0) {
            parseGpsObject(json_tokens, json_string, config, &i);
        }
        else {
            ++i;
            jsmntok_t token = json_tokens[i];
            switch (token.type)
            {
                case JSMN_ARRAY:
                    i += token.size;
                    break;
                case JSMN_OBJECT:
                    i += 2 * token.size;
                    break;
            }
        }
    }
}