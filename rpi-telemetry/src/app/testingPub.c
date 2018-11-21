/**********************************************************************
 * pub.c - collect data from a CAN plugin and send them via MQTT      *
 **********************************************************************/
#include "../plugin.h"
#include "../utils.h"
#include "../version.h"
#include <bson.h>
#include <dlfcn.h>
#include <mosquitto.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

int (*get_data)(can_data_t *data);

typedef struct {
  config_t *cfg;
  bool mqtt_connected;
} userdata_t;

void mq_connect(struct mosquitto *m, void *userdata, int rc) {
  userdata_t *ud = (userdata_t *)userdata;
  ud->mqtt_connected = true;
  fprintf(stderr, "Connected to broker\n");
}

void mq_disconnect(struct mosquitto *m, void *userdata, int rc) {
  userdata_t *ud = (userdata_t *)userdata;
  ud->mqtt_connected = false;
  fprintf(stderr, "Disconnected from broker (%d)\n", rc);
}

int main(int argc, char const *argv[]) {
  const uint8_t *data;
  size_t jlen, blen;
  bson_t *bdoc;
  struct mosquitto *m;
  int status, i, j;
  void *dlhandle = NULL;
  can_data_t can_data = {0};
  userdata_t ud = {NULL, false};
  FILE *cache;

  printf("%s Version %s\n", argv[0], GIT_COMMIT_HASH);

  // Connect to data provider plugin
  if (argc != 2) {
    fprintf(stderr, "Exactly one argument needed (config file path)\n");
    exit(EXIT_FAILURE);
  }
  if ((ud.cfg = new_config(argv[1], CFG_PUB)) == NULL) {
    exit(EXIT_FAILURE);
  }
  dlhandle = dlopen(ud.cfg->plugin_path, RTLD_LOCAL | RTLD_LAZY);
  if (!dlhandle) {
#ifdef __APPLE__
    perror("dlopen");
#else
    fprintf(stderr, "dlopen error: %s\n", dlerror());
#endif
    exit(EXIT_FAILURE);
  }
  if ((get_data = dlsym(dlhandle, "get_data")) == NULL) {
#ifdef __APPLE__
    perror("dlsym");
#else
    fprintf(stderr, "dlsym error: %s\n", dlerror());
#endif
    exit(EXIT_FAILURE);
  }

  // Mosquitto initialize
  mosquitto_lib_init();
  m = mosquitto_new(argv[0], false, &ud);
  mosquitto_connect_callback_set(m, mq_connect);
  mosquitto_disconnect_callback_set(m, mq_disconnect);

  status = mosquitto_connect(m, ud.cfg->broker_host, ud.cfg->broker_port, 10);
  if (status == MOSQ_ERR_INVAL) {
    fprintf(stderr, "Error connecting\n");
    exit(EXIT_FAILURE);
  } else if (status == MOSQ_ERR_ERRNO) {
    perror("MQTT");
    exit(EXIT_FAILURE);
  }

  // cache file
  // create the file if not exist
  cache = fopen(ud.cfg->cache_path, "a+");
  fclose(cache);
  // open the file in update mode
  cache = fopen(ud.cfg->cache_path, "rb+");
  // if the file is empty reserve first 4 bytes in file for a persistent seek address
  if (ftell(cache) == 0) {
    uint32_t zero = sizeof(uint32_t);
    printf("first seek address should be: %zu\n", zero); 
    fwrite(&zero, sizeof(uint32_t), 1, cache);
    fflush(cache);
  }
  // then, when flushing the cache, the seek address has to be updated
  // after every read: read seek address from first 4 bytes; move to that
  // address; check if the next char is "$"; do n=atoi() of the next 5 chars as
  // string; read the buffer of the next n bytes, and call bson_new_from_data();
  // update the seek address with ftell() and loop again.

  // Wait for connection
  while (ud.mqtt_connected == false) {
    mosquitto_loop(m, 10, 1);
    usleep(1E3);
  }

  ///////////Testing cache///////////
  printf("\n\nTesting cache\n");
  for (j = 0; j < 2; j++) {
    // Create an example document (input doc, will come from CAN)
    get_data(&can_data);
    can_data_to_bson(&can_data, &bdoc, ud.cfg->plugin_path);
    data = bson_get_data(bdoc);
    blen = bdoc->len;

    //cache data locally, since the MQTT link is not available
    printf("LOOP:%i\n",j+1);
    printf("sono a:%i\n",ftell(cache));
    fseek(cache, 0, SEEK_END);//set the file position of the stream to tail
    printf("mi sposto a tail:%i\n",ftell(cache));
    fprintf(cache, "$%05zu", blen); //add length to header
    printf("dopo aver scritto length sono a:%i\n",ftell(cache));
    printf("ho scritto length:$%05zu\n",blen);

    fwrite(data, blen, 1, cache); //write data
    printf("dopo aver scritto data sono a:%i\n",ftell(cache));
    fflush(cache);
    printf("> cached data:\n");
    print_buffer(stdout, data, blen);
    fflush(stdout);

    bson_destroy(bdoc);
    // throttle this loop
    wait_next(100E6);
  }

  ///////////Testing flush cache + publish to check if it works///////////
  printf("\n\nTesting flush cache\n");
  for (j = 0; j < 2; j++) {
    // trigger mosquitto callbacks
    mosquitto_loop(m, 1, 1);

    printf("start flush section\n");
    //flush section//
    printf("sono a:%i\n",ftell(cache));
    fseek(cache, 0, SEEK_SET); //set the file position of the stream to head
    printf("mi sposto a head:%i\n",ftell(cache));
    uint32_t seekAddress;
    fread(&seekAddress, sizeof(uint32_t), 1, cache); //read seek address
    printf("dopo aver letto seek sono a:%i\n",ftell(cache));
    printf("seek address: %i\n",seekAddress);
    fseek(cache, seekAddress, SEEK_SET); //sets the file position of the stream to seekAddress
    printf("mi sposto a seek e sono a:%i\n",ftell(cache));
    char c[5] = {0};
    fread(&c, sizeof(char), 1, cache); //read the next char to check if its $
    printf("leggo 1 char e sono a:%i\n",ftell(cache));
    printf("char che ho letto:%s\n",c);
    if (c[0] == '$') {
      fread(&c, sizeof(char), 5, cache);
      int blen = atoi(c); //size of the buffer to read
      printf("leggo blen e sono a:%i\n",ftell(cache));
      printf("blen letta:%i\n",blen);
      uint8_t *buffer = (uint8_t *)malloc(blen); //allocate temporary buffer to contain the read data
      printf("done malloc with len:%i\n",blen);
      fread(buffer, blen, 1, cache); //read data
      printf("leggo data e sono a:%i\n",ftell(cache));
      bdoc = bson_new_from_data(buffer, blen); //store data into bson
      free(buffer); //deallocate the buffer as we are done with it
      seekAddress = ftell(cache); 
      printf("il nuovo seek é:%i\n",seekAddress);
      fseek(cache, 0, SEEK_SET); //sets the file position of the stream to start of file
      printf("mi sposto alla testa del file e sono a:%i\n",ftell(cache));
      printf("size of seekAddress:%i\n",sizeof(seekAddress));
      fwrite(&seekAddress, sizeof(uint32_t), 1, cache); //write the new seek address
      printf("scrivo nuovo seekAddress e sono a:%i\n",ftell(cache));
      //test what i have written
      fseek(cache,-sizeof(seekAddress), SEEK_CUR);
      printf("torno indietro di 4 per vedere cosa ho scritto e sono a:%i\n",ftell(cache));
      fread(&seekAddress, sizeof(uint32_t), 1, cache); //read seek address //TODO ho bisogno di aprire il file in mod read/write e non in mod append
      printf("ho letto seek address: %i\n",seekAddress);
      printf("dopo aver letto seek sono a:%i\n",ftell(cache));
    }
    //end flush section//
    printf("end flush section\n");

    data = bson_get_data(bdoc);
    // dump it to a data buffer
    printf("> Raw data buffer:\n");
    print_buffer(stdout, data, blen);
    // Send BSON data as a buffer via MQTT
    mosquitto_publish(m, NULL, ud.cfg->mqtt_topic, blen, data, 0, false);
    printf("> Sent %zu bytes.\n\n", blen);
    bson_destroy(bdoc);

    wait_next(100E6);
  }

  //closing
  printf("> Clean exit\n");

  fclose(cache);
  mosquitto_destroy(m);
  mosquitto_lib_cleanup();

  return 0;
}