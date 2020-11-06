#include <bson.h>
#include <mongoc.h>
#include "../../../../../services/mosquitto_service/mosquitto_service.h"
#include "../../../../../state_machine/state_machine_condition.h"


condition_t condition;
/*
    1 host
    2 port
*/
int main(int argc, char *argv[]) {
    condition.mqtt.host = (char *)malloc((strlen(argv[1]) + 1) * sizeof(char));
    strcpy(condition.mqtt.host, argv[1]);
    condition.mqtt.port = atoi(argv[2]);

    int res = mosquittoSetup();
    free(condition.mqtt.host);
    return 0;
}