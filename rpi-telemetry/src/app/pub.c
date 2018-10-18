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

// all ego-state vars go in here:
typedef struct {
  bool running;
  bool carIsConnected;
  bool carIsMoving;
  struct mosquitto *m;
  const uint8_t *data;
  size_t jlen, blen;
  bson_t *bdoc;
  int status, i, j;
  can_data_t can_data = {0};
  userdata_t ud = {NULL, false};
  FILE *cache;
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
static state_t do_state_init(state_data_t *state_data);
static state_t do_state_eval_status(state_data_t *state_data);
static state_t do_state_running(state_data_t *state_data);
static state_t do_state_idle(state_data_t *state_data);
static state_t do_state_stop(state_data_t *state_data);
static state_t do_state_flush_cache(state_data_t *state_data);
static state_t do_state_cache(state_data_t *state_data);
static state_t do_state_publish(state_data_t *state_data);

// state table
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

typedef struct {
  config_t *cfg;
  bool mqtt_connected;
} userdata_t;


int (*get_data)(can_data_t *data);

void initCheck(int argc, char const *argv[]){
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
}

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

//                .__        
//   _____ _____  |__| ____  
//  /     \\__  \ |  |/    \ 
// |  Y Y  \/ __ \|  |   |  \
// |__|_|  (____  /__|___|  /
//       \/     \/        \/ 

int main(int argc, char const *argv[]) {


  void *dlhandle = NULL;

  state_data_t state_data = {
    false,
    true,
    true,
  };
  state_t cur_state = INIT;

  printf("%s Version %s\n", argv[0], GIT_COMMIT_HASH);

  initCheck(argc,argv);

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
                                              

// FSM entry function, to be caled in loop
static state_t run_state(state_t current_state, state_data_t *state_data) {
  state_t new_state = state_table[current_state](state_data);
  return new_state;
}

// .__       .__  __   
// |__| ____ |__|/  |_ 
// |  |/    \|  \   __\
// |  |   |  \  ||  |  
// |__|___|  /__||__|  
//         \/          

state_t do_state_init(state_data_t *state_data) {
  state_data.running -> true;

  return EVAL_STATUS;
}

//                     .__              __          __                
//   _______  _______  |  |     _______/  |______ _/  |_ __ __  ______
// _/ __ \  \/ /\__  \ |  |    /  ___/\   __\__  \\   __\  |  \/  ___/
// \  ___/\   /  / __ \|  |__  \___ \  |  |  / __ \|  | |  |  /\___ \ 
//  \___  >\_/  (____  /____/ /____  > |__| (____  /__| |____//____  >
//      \/           \/            \/            \/                \/ 

state_t do_state_eval_status(state_data_t *state_data) {

  if(state_data.carIsMoving){
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

  if(state_data.carIsConnected){
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

  if(state_data.carIsConnected){
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

  fclose(state_data.cache);
  mosquitto_destroy(state_data.m);
  mosquitto_lib_cleanup();

  state_data.running -> false;//nello struct metto false/true per poter uscire dall loop

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

  return EVAL_STATUS;
}

//             ___.  ___.   .__  .__       .__     
// ______  __ _\_ |__\_ |__ |  | |__| _____|  |__  
// \____ \|  |  \ __ \| __ \|  | |  |/  ___/  |  \ 
// |  |_> >  |  / \_\ \ \_\ \  |_|  |\___ \|   Y  \
// |   __/|____/|___  /___  /____/__/____  >___|  /
// |__|             \/    \/             \/     \/ 

state_t do_state_publish(state_data_t *state_data) {

  return EVAL_STATUS;
}
