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
  config_t *cfg; //pub and sub configuration file
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

//all ego-state vars go in here: data used by the FSM to run and evaluate each state
typedef struct {
  bool running;
  bool carIsConnected;
  bool carIsMoving;
  struct mosquitto *m;
  const uint8_t *data;
  size_t jlen, blen;
  bson_t *bdoc;
  int status, i, j;
  can_data_t can_data;
  userdata_t ud;
  FILE *cache;
} state_data_t;

// list of valid states, plus end_index. Enforce first index to 0 for portability
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

// state events lookup table used by run_state to launch the appropriate state from an input state_t
state_func_t * const state_table[NUM_STATES] = {
  do_state_init,
  do_state_eval_status,
  do_state_running,
  do_state_idle,
  do_state_stop,
  do_state_flush_cache,
  do_state_cache,
  do_state_publish
};

// FSM entry function, to be called in loop
static state_t run_state(state_t current_state, state_data_t *state_data) {
  state_t new_state = state_table[current_state](state_data); //TODO expression preceding parentheses of apparent call must have (pointer-to-) function type
  return new_state;
}

// state function template signature
typedef state_t state_func_t(state_data_t *);

// declaration of state functions necessary to switch state 
static state_t do_state_init(state_data_t *state_data);
static state_t do_state_eval_status(state_data_t *state_data);
static state_t do_state_running(state_data_t *state_data);
static state_t do_state_idle(state_data_t *state_data);
static state_t do_state_stop(state_data_t *state_data);
static state_t do_state_flush_cache(state_data_t *state_data);
static state_t do_state_cache(state_data_t *state_data);
static state_t do_state_publish(state_data_t *state_data);

//                .__        
//   _____ _____  |__| ____  
//  /     \\__  \ |  |/    \ 
// |  Y Y  \/ __ \|  |   |  \
// |__|_|  (____  /__|___|  /
//       \/     \/        \/ 

int main(int argc, char const *argv[]) {
  void *dlhandle = NULL;
  
  //init state_data
  state_data_t state_data = {
    .running = false,
    .carIsConnected = true,
    .carIsMoving = true,
    .can_data = {0},
    .ud = {NULL, false}
  };

  //print current git commint id
  printf("%s Version %s\n", argv[0], GIT_COMMIT_HASH);

  // Connect to data provider plugin
  if (argc != 2) {
    fprintf(stderr, "Exactly one argument needed (config file path)\n");
    exit(EXIT_FAILURE);
  }
  if ((state_data.ud.cfg = new_config(argv[1], CFG_PUB)) == NULL) {
    exit(EXIT_FAILURE);
  }
  dlhandle = dlopen(state_data.ud.cfg->plugin_path, RTLD_LOCAL | RTLD_LAZY);
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

  //launch init FSM state
  mosquitto_lib_init();
  state_data.m = mosquitto_new(argv[0], false, &state_data.ud);
  state_t cur_state = run_state(INIT, &state_data.data);

  //loop for the FSM
  do {
    mosquitto_loop(state_data.m, 1, 1);
    cur_state = run_state(cur_state, &state_data.data);
  } while (state_data.running);

  return 0;
}

// ____________________   _____   
// \_   _____/   _____/  /     \  
//  |    __) \_____  \  /  \ /  \ 
//  |     \  /        \/    Y    \
//  \___  / /_______  /\____|__  /
//      \/          \/         \/ 

// .__       .__  __   
// |__| ____ |__|/  |_ 
// |  |/    \|  \   __\
// |  |   |  \  ||  |  
// |__|___|  /__||__|  
//         \/          

state_t do_state_init(state_data_t *state_data) {
  state_data->running = true;

  //Mosquitto initialize
  mosquitto_connect_callback_set(state_data->m, mq_connect);
  mosquitto_disconnect_callback_set(state_data->m, mq_disconnect);

  state_data->status = mosquitto_connect(state_data->m, state_data->ud.cfg->broker_host, state_data->ud.cfg->broker_port, 10);
  if (state_data->status == MOSQ_ERR_INVAL) {
    fprintf(stderr, "Error connecting\n");
    exit(EXIT_FAILURE);
  } else if (state_data->status == MOSQ_ERR_ERRNO) {
    perror("MQTT");
    exit(EXIT_FAILURE);
  }

  // cache file initialize
  //reserve first 4 bytes in file for a persistent seek address
  // state_data->cache = fopen(ud.cfg->cache_path, "a+");
  // if (ftell(state_data->cache) == 0) {
  //   uint32_t zero = sizeof(uint32_t) + 1;
  //   fwrite(&zero, sizeof(uint32_t), 1, state_data->cache);
  //   fflush(state_data->cache);
  // }
  // then, when flushing the cache, the seek address has to be updated
  // after every read: read seek address from first 4 bytes; move to that
  // address; chek if the next char is "$"; do n=atoi() of the next 5 chars as
  // string; read the buffer of the next n bytes, and call bson_new_from_data();
  // update the seek address with ftell() and loop again.

  return EVAL_STATUS;
}

//                     .__              __          __                
//   _______  _______  |  |     _______/  |______ _/  |_ __ __  ______
// _/ __ \  \/ /\__  \ |  |    /  ___/\   __\__  \\   __\  |  \/  ___/
// \  ___/\   /  / __ \|  |__  \___ \  |  |  / __ \|  | |  |  /\___ \ 
//  \___  >\_/  (____  /____/ /____  > |__| (____  /__| |____//____  >
//      \/           \/            \/            \/                \/ 

state_t do_state_eval_status(state_data_t *state_data) {

  if(state_data->carIsMoving){
    return RUNNING;
  }else{
    return IDLE;
  }
}

//                            .__                
// _______ __ __  ____   ____ |__| ____    ____  
// \_  __ \  |  \/    \ /    \|  |/    \  / ___\ 
//  |  | \/  |  /   |  \   |  \  |   |  \/ /_/  >
//  |__|  |____/|___|  /___|  /__|___|  /\___  / 
//                   \/     \/        \//_____/  

state_t do_state_running(state_data_t *state_data) {

  if(state_data->carIsConnected){
    return PUBLISH;
  }else{
    return CACHE;
  }
}

// .__    .___.__          
// |__| __| _/|  |   ____  
// |  |/ __ | |  | _/ __ \ 
// |  / /_/ | |  |_\  ___/ 
// |__\____ | |____/\___  >
//         \/           \/ 

state_t do_state_idle(state_data_t *state_data) {

  if(state_data->carIsConnected){
    return FLUSH_CACHE;
  }else{
    return CACHE;
  }
}

//           __                 
//   _______/  |_  ____ ______  
//  /  ___/\   __\/  _ \\____ \ 
//  \___ \  |  | (  <_> )  |_> >
// /____  > |__|  \____/|   __/ 
//      \/              |__|    

state_t do_state_stop(state_data_t *state_data) {

  printf("> Clean exit\n");

  fclose(state_data->cache);
  mosquitto_destroy(state_data->m);
  mosquitto_lib_cleanup();

  state_data->running = false;//nello struct metto false/true per poter uscire dall loop

  return STOP; 
}

//   _____.__               .__                           .__            
// _/ ____\  |  __ __  _____|  |__     ____ _____    ____ |  |__   ____  
// \   __\|  | |  |  \/  ___/  |  \  _/ ___\\__  \ _/ ___\|  |  \_/ __ \ 
//  |  |  |  |_|  |  /\___ \|   Y  \ \  \___ / __ \\  \___|   Y  \  ___/ 
//  |__|  |____/____//____  >___|  /  \___  >____  /\___  >___|  /\___  >
//                        \/     \/       \/     \/     \/     \/     \/ 

state_t do_state_flush_cache(state_data_t *state_data) {
  //sendData with cache
  return PUBLISH;
}

//                      .__            
//   ____ _____    ____ |  |__   ____  
// _/ ___\\__  \ _/ ___\|  |  \_/ __ \ 
// \  \___ / __ \\  \___|   Y  \  ___/ 
//  \___  >____  /\___  >___|  /\___  >
//      \/     \/     \/     \/     \/ 

state_t do_state_eval_cache(state_data_t *state_data) {
  //store cache
  return EVAL_STATUS;
}

//             ___.  ___.   .__  .__       .__     
// ______  __ _\_ |__\_ |__ |  | |__| _____|  |__  
// \____ \|  |  \ __ \| __ \|  | |  |/  ___/  |  \ 
// |  |_> >  |  / \_\ \ \_\ \  |_|  |\___ \|   Y  \
// |   __/|____/|___  /___  /____/__/____  >___|  /
// |__|             \/    \/             \/     \/ 

state_t do_state_publish(state_data_t *state_data) {

  get_data(&state_data->can_data);
  can_data_to_bson(&state_data->can_data, &state_data->bdoc, state_data->ud.cfg->plugin_path);
  state_data->blen = state_data->bdoc->len;
  state_data->data = bson_get_data(state_data->bdoc); 

  //this step will not be necessary (testing only)
  char *json = bson_as_json(state_data->bdoc, &state_data->jlen);
  printf("> Original doc as JSON (%zu bytes):\n%s\n", state_data->jlen, json);
  free(json);
  // dump it to a data buffer

  printf("> Raw data buffer:\n");
  print_buffer(stdout, state_data->data, state_data->blen);
  // Send BSON data as a buffer via MQTT
  mosquitto_publish(state_data->m, NULL, state_data->ud.cfg->mqtt_topic, state_data->blen, state_data->data, 0, false); //NOTE: retain flag could be usefull for data log on DB as its similar to TCP 
  printf("> Sent %zu bytes.\n\n", state_data->blen);
  bson_destroy(state_data->bdoc);

  return EVAL_STATUS;
}
