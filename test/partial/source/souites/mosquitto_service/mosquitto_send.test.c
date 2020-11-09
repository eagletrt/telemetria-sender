#include <bson.h>
#include <mongoc.h>
#include "../../../../../services/mosquitto_service/mosquitto_service.h"
#include "../../../../../state_machine/state_machine_condition.h"


condition_t condition;
/*
    1 host
    2 port
    3 data_topic
    4 json
*/
int main(int argc, char *argv[]) {
    condition.mqtt.host = (char *)malloc((strlen(argv[1]) + 1) * sizeof(char));
    condition.mqtt.data_topic = (char *)malloc((strlen(argv[3]) + 1) * sizeof(char));
    strcpy(condition.mqtt.host, argv[1]);
    strcpy(condition.mqtt.data_topic, argv[3]);
    condition.mqtt.port = atoi(argv[2]);

    bson_t *b;
    bson_error_t error;
    b = bson_new_from_json (argv[4], -1, &error);

    mosquittoSetup();
    int res = mosquittoSend(b);
    
    free(condition.mqtt.host);
    free(condition.mqtt.data_topic);
    free(b);
    return res;
}