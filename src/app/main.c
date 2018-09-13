#include <stdio.h>
#include <string.h>
#include <mosquitto.h>
#include "version.h"

int main(int argc, char const *argv[]) {
    struct mosquitto *m;
    int status;
    printf("Version %s\n", GIT_COMMIT_HASH);
    mosquitto_lib_init();
    m = mosquitto_new(NULL, true, NULL);
    status = mosquitto_connect(m, "localhost", 1883, 60);
    if ( status == MOSQ_ERR_SUCCESS ) {
    }
    else if ( status == MOSQ_ERR_INVAL ) {
      fprintf(stderr, "Error connecting\n");
    }
    else if ( status == MOSQ_ERR_ERRNO ) {
      perror("MQTT");
    }
    mosquitto_destroy(m);
    mosquitto_lib_cleanup();
    return 0;
}
