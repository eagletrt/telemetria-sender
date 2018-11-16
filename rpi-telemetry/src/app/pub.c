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
  // reserve first 4 bytes in file for a persistent seek address
  cache = fopen(ud.cfg->cache_path, "a+");
  if (ftell(cache) == 0) {
    uint32_t zero = sizeof(uint32_t) + 1;
    fwrite(&zero, sizeof(uint32_t), 1, cache);
    fflush(cache);
  }
  // then, when flushing the cache, the seek address has to be updated
  // after every read: read seek address from first 4 bytes; move to that
  // address; chek if the next char is "$"; do n=atoi() of the next 5 chars as
  // string; read the buffer of the next n bytes, and call bson_new_from_data();
  // update the seek address with ftell() and loop again.

  // Wait for connection
  while (ud.mqtt_connected == false) {
    mosquitto_loop(m, 10, 1);
    usleep(1E3);
  }

  //open the socket
  int socket;
  struct sockaddr_can addr1;

  char* name = (char*) malloc(sizeof(char)*5);
  strcpy(name,"vcan0");

  socket open_can_socket(name,&addr1);
  //socket opened

  printf("Testing BSON and Mosquitto pub\n\n");
  for (j = 0; j < 1000; j++) {
    // trigger mosquitto callbacks
    mosquitto_loop(m, 1, 1);

    // Data Gathering
    // Timer setting

    // trigger must be setted in ms, data are sent every end of timer
    int msec = 0, trigger = 500;
    clock_t before = clock();

    do {
      int id = 0; 
      int data1 = 0;
      int data2 = 0;

      receive_can_compact(socket,&id,&data1,&data2);

      msec = (clock() - before) * 1000 / CLOCKS_PER_SEC;
    } while ( msec < trigger );

    // Create an example document (input doc, will come from CAN)
    get_data(&can_data);
    can_data_to_bson(&can_data, &bdoc, ud.cfg->plugin_path);
    data = bson_get_data(bdoc);
    blen = bdoc->len;

    if (ud.mqtt_connected) {
      char *json = bson_as_json(bdoc, &jlen);
      printf("> Original doc as JSON (%zu bytes):\n%s\n", jlen, json);
      free(json);
      // dump it to a data buffer
      printf("> Raw data buffer:\n");
      print_buffer(stdout, data, blen);
      // Send BSON data as a buffer via MQTT
      mosquitto_publish(m, NULL, ud.cfg->mqtt_topic, blen, data, 0, false);
      printf("> Sent %zu bytes.\n\n", blen);
    } 
    else {
      // cache data locally, since the MQTT link is not available
      fprintf(cache, "$%05zu", blen);
      fwrite(data, blen, 1, cache);
      fflush(cache);
      printf(".");
      fflush(stdout);
      // try to reconnect
      mosquitto_reconnect(m);
    }
    bson_destroy(bdoc);

    // throttle this loop
    wait_next(100E6);
  }

  printf("> Clean exit\n");

  fclose(cache);
  mosquitto_destroy(m);
  mosquitto_lib_cleanup();

  return 0;
}



                                                    
//  _____ _____ _____    _____               _____     
// |   __|   __|     |  |  |  |___ _ _ _ ___|_   _|___ 
// |   __|__   | | | |  |     | . | | | |___| | | | . |
// |__|  |_____|_|_|_|  |__|__|___|_____|     |_| |___|
                                                    
#if 0
// all ego-state vars go in here:
typedef struct {
  bool running;
} state_data_t;

// list of valid states, plus end_index. Enforce first index to 0 for 
// portability
typedef enum {
  INIT = 0,
  EVAL_STATUS,
  RUNNING,
  IDLE,
  STOP,
  FLUSH_CACHE,
  CACHE,
  PUBLISH,
  NUM_STATES
} state_t;

// state function template signature
typedef state_t state_func_t(state_data_t *);

// declaration of state functions
static state_t do_state_init(state_data_t *data);
static state_t do_state_eval_status(state_data_t *data);

// state table
state_func_t * const state_table[NUM_STATES] = {
  do_state_init,
  do_state_eval_status
};

// FSM entry function, to be caled in loop
static state_t run_state(state_t current_state, state_data_t *data) {
  state_t new_state = state_table[current_state](data);
  return new_state;
}

state_t do_state_init(state_data_t *state) {
  state->running = true;
  return EVAL_STATUS;
}



// Then, in main, do the following
// state_data_t data = {false};
// state_t cur_state = INIT
// do {
//   cur_state = run_state(cur_state, &data);
// } while (state.running)

#endif