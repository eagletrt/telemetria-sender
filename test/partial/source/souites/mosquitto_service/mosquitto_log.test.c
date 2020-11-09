#include <bson.h>
#include <mongoc.h>
#include "../../../../../services/mosquitto_service/mosquitto_service.h"
#include "../../../../../state_machine/state_machine_condition.h"


condition_t condition;
/*
    1 host
    2 port
    3 data_topic
    4 message
*/
int main(int argc, char *argv[]) {
    condition.mqtt.host = (char *)malloc((strlen(argv[1]) + 1) * sizeof(char));
    condition.mqtt.log_topic = (char *)malloc((strlen(argv[3]) + 1) * sizeof(char));
    strcpy(condition.mqtt.host, argv[1]);
    strcpy(condition.mqtt.log_topic, argv[3]);
    condition.mqtt.port = atoi(argv[2]);

    char *msg = (char *)malloc((strlen(argv[4]) + 1) * sizeof(char));
    strcpy(msg, argv[4]);

    mosquittoSetup();
    int res = mosquittoLog(msg);
    
    free(condition.mqtt.host);
    free(condition.mqtt.data_topic);
    free(msg);
    return res;
}