#include <stdio.h>
#include <string.h>
#include <mosquitto.h>
#include <bson.h>
#include "version.h"

int main(int argc, char const *argv[]) {
    printf("Version %s\n", GIT_COMMIT_HASH);

    printf("\nTesting Mosquitto\n");
    {
      struct mosquitto *m;
      int status;
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
    }

    printf("\nTesting BSON\n");
    {
      bson_t *document;
      size_t len;
      document = BCON_NEW (
        "idx", BCON_INT32(1),
        "name", BCON_UTF8("test") 
      );
      printf("Document:\n%s\nlength: %d\n", bson_as_relaxed_extended_json(document, &len), len);
    }

    return 0;
}
