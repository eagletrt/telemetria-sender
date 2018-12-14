/**********************************************************************
 * pub.c - collect data from a CAN plugin and send them via MQTT      *
 **********************************************************************/
#include "../plugin.h"
#include "../utils.h"
#include "../version.h"
#include "../can_custom_lib.h"
#include <bson.h>
#include <dlfcn.h>
#include <mosquitto.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

//TEST MODE
//the 3rd arg contains a sequence of ints from 0 to testModeLastCommand indicating the states to execute
int testModeIndex = 0;
int testModeLength = 0;
int *testModeCommands; //array that indicate the fsm state to play/test during execution (0 = publish, 1 = cache, 2 = flush)
bool testMode = false; //indicate weather the pub is in test mode or not
int testModeLastCommand = 2;

int flushCacheCounter = 1; //we use this counter to regulate the number of flush_cache with the live data from CAN when idle

int (*get_data)(int* data_gathered, int data_lenght,can_data_t *data);
void dataGathering(int* to_send, int* index, int socket);

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
  int socket;
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
  state_t new_state = state_table[current_state](state_data);
  return new_state;
}

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

  //check args
  if (argc != 2 && argc != 3) {
    fprintf(stderr, "Exactly one argument needed (config file path) to launch normal pub. (for test mode add a sequence of ints from 0 to %i indicating the states to execute in test mode)\n", testModeLastCommand);
    exit(EXIT_FAILURE);
  }

  //test mode input
  if (argc == 3) {
    testMode = true;
    testModeCommands = (int*)malloc(strlen(argv[2]));
    if (testModeCommands != NULL) {        
        for(int i = 0; i < strlen(argv[2]); i++){ 
          int command = argv[2][i] - '0'; //convert char to int remove the offset '0' to realign to count from 0-9
          //check if command is valid
          if(command > testModeLastCommand || command < 0){
            fprintf(stderr, "At least one command in test mode was not recognized (for test mode add a sequence of ints from 0 to %i indicating the states to execute in test mode)\n", testModeLastCommand);
            free(testModeCommands); //free test string 
            exit(EXIT_FAILURE);
          } else {
            testModeCommands[i] = command;
            testModeLength++;
          }
        }
    } else {
      fprintf(stderr, "No commands (for test mode add a sequence of ints from 0 to %i indicating the states to execute in test mode)\n", testModeLastCommand);
      free(testModeCommands); //free test string 
      exit(EXIT_FAILURE);
    }
  }

  // Connect to data provider plugin
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
  state_t cur_state = run_state(INIT, &state_data);

  //loop for the FSM
  do {
    mosquitto_loop(state_data.m, 1, 1);
    cur_state = run_state(cur_state, &state_data);
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

  // create the file if not exist
  state_data->cache = fopen(state_data->ud.cfg->cache_path, "a+");
  fclose(state_data->cache);
  // open the file in update mode
  state_data->cache = fopen(state_data->ud.cfg->cache_path, "rb+");
  fseek(state_data->cache, 0, SEEK_END);//set the file position of the stream to tail
  // if the file is empty reserve first 4 bytes in file for a persistent seek address
  if (ftell(state_data->cache) == 0) {
    uint32_t zero = sizeof(uint32_t);
    printf("first seek address should be: %zu\n", zero); 
    fwrite(&zero, sizeof(uint32_t), 1, state_data->cache);
    fflush(state_data->cache);
  }

  //open the socket
  struct sockaddr_can addr1;

  char* name = (char*) malloc(sizeof(char)*5);
  strcpy(name,"can0");

  state_data->socket = open_can_socket(name,&addr1);
  //socket opened

  return EVAL_STATUS;
}

//                     .__              __          __                
//   _______  _______  |  |     _______/  |______ _/  |_ __ __  ______
// _/ __ \  \/ /\__  \ |  |    /  ___/\   __\__  \\   __\  |  \/  ___/
// \  ___/\   /  / __ \|  |__  \___ \  |  |  / __ \|  | |  |  /\___ \ 
//  \___  >\_/  (____  /____/ /____  > |__| (____  /__| |____//____  >
//      \/           \/            \/            \/                \/ 

state_t do_state_eval_status(state_data_t *state_data) {

  if(testMode){
    if(testModeIndex == testModeLength){ //stop when there are no more commands
      return STOP;
    }
    else{
      printf("test command: %i\n", testModeCommands[testModeIndex]); 
      if(testModeCommands[testModeIndex] == 0 || testModeCommands[testModeIndex] == 1)
        return RUNNING;
      if(testModeCommands[testModeIndex] == 2)
        return IDLE;
    }
  }  
  else{
    if(state_data->carIsMoving){
      flushCacheCounter = 1; //reset the counter for number of flush_cache done
      return RUNNING;
    }
    else
      return IDLE;
  }
}

//                            .__                
// _______ __ __  ____   ____ |__| ____    ____  
// \_  __ \  |  \/    \ /    \|  |/    \  / ___\ 
//  |  | \/  |  /   |  \   |  \  |   |  \/ /_/  >
//  |__|  |____/|___|  /___|  /__|___|  /\___  / 
//                   \/     \/        \//_____/  

state_t do_state_running(state_data_t *state_data){

  //in either case i need the data from can
  int* to_send;
  int index = 0;
  dataGathering(to_send, &index, state_data->socket);
  get_data(to_send, index ,&state_data->can_data);
  can_data_to_bson(&state_data->can_data, &state_data->bdoc, state_data->ud.cfg->plugin_path);
  if(testMode){
    if(testModeCommands[testModeIndex] == 0){
      testModeIndex++; //we are done with this command
      return PUBLISH;
    }
    if(testModeCommands[testModeIndex] == 1){
      testModeIndex++; //we are done with this command
      return CACHE;
    }
  }  
  else{
    if(state_data->carIsConnected)
      return PUBLISH;
    else
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

  if(testMode){
    if(testModeCommands[testModeIndex] == 2){
      testModeIndex++; //we are done with this command
      if (flushCacheCounter % 10 == 0) { //every 9 flush we send the current data coming from CAN
        printf("send idle data\n");
        //get data from CAN
        int* to_send;
        int index = 0;
        dataGathering(to_send, &index, state_data->socket);
        get_data(to_send, index ,&state_data->can_data);    
        can_data_to_bson(&state_data->can_data, &state_data->bdoc, state_data->ud.cfg->plugin_path);
        return PUBLISH;
      }
      else{
        flushCacheCounter++;
        return FLUSH_CACHE;
      }
    }
  }  
  else{
    if(state_data->carIsConnected){
      if (flushCacheCounter % 10 == 0) { //every 9 flush we send the current data coming from CAN
        printf("send idle data\n");
        //get data from CAN
        int* to_send;
        int index = 0;
        dataGathering(to_send, &index, state_data->socket);
        get_data(to_send, index ,&state_data->can_data);    
        can_data_to_bson(&state_data->can_data, &state_data->bdoc, state_data->ud.cfg->plugin_path);
        return PUBLISH;
      }
      else{
        flushCacheCounter++;
        return FLUSH_CACHE;
      }
    }
    else{
      if (flushCacheCounter % 10 == 0) { //we send the current data coming from CAN less often as we are in idle
        //get data from CAN
        int* to_send;
        int index = 0;
        dataGathering(to_send, &index, state_data->socket);
        get_data(to_send, index ,&state_data->can_data);    
      get_data(to_send, index ,&state_data->can_data);    
        get_data(to_send, index ,&state_data->can_data);    
        can_data_to_bson(&state_data->can_data, &state_data->bdoc, state_data->ud.cfg->plugin_path);
        return CACHE;
      }
      else
        flushCacheCounter++;
    }
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

  state_data->running = false;//we set false so that we exit the loop

  if(testMode)
    free(testModeCommands); //free test string 

  return STOP; 
}

//   _____.__               .__                           .__            
// _/ ____\  |  __ __  _____|  |__     ____ _____    ____ |  |__   ____  
// \   __\|  | |  |  \/  ___/  |  \  _/ ___\\__  \ _/ ___\|  |  \_/ __ \ 
//  |  |  |  |_|  |  /\___ \|   Y  \ \  \___ / __ \\  \___|   Y  \  ___/ 
//  |__|  |____/____//____  >___|  /  \___  >____  /\___  >___|  /\___  >
//                        \/     \/       \/     \/     \/     \/     \/

state_t do_state_flush_cache(state_data_t *state_data) {
  // retrieve data from cache
  // seek address has to be updated after every read: read seek address from first 4 bytes; 
  // move to that address; check if the next char is "$"; do n=atoi() of the next 5 chars as
  // string; read the buffer of the next n bytes, and call bson_new_from_data();
  // update the seek address with ftell() and loop again.
  fseek(state_data->cache, 0, SEEK_SET); //set the file position of the stream to head
  uint32_t seekAddress;
  fread(&seekAddress, sizeof(uint32_t), 1, state_data->cache); //read seek address
  fseek(state_data->cache, seekAddress, SEEK_SET); //sets the file position of the stream to seekAddress
  char c[5] = {0};
  fread(&c, sizeof(char), 1, state_data->cache);
  if (c[0] == '$') { //if the first char is not $ then there is a problem
    printf("flushing cache in position: %i\n\n", seekAddress);
    fread(&c, sizeof(char), 5, state_data->cache);
    state_data->blen = atoi(c); //size of the buffer to read
    uint8_t *buffer = (uint8_t *)malloc(state_data->blen); //allocate temporary buffer to contain the read data  
    fread(buffer, state_data->blen, 1, state_data->cache); //read data
    state_data->bdoc = bson_new_from_data(buffer, state_data->blen); //store data into bson
    free(buffer); //deallocate the buffer as we are done with it
    seekAddress = ftell(state_data->cache); 
    fseek(state_data->cache, 0, SEEK_SET); //sets the file position of the stream to start of file
    fwrite(&seekAddress, sizeof(uint32_t), 1, state_data->cache); //write the new seek address
    return PUBLISH;
  }
  else{
    printf("ERROR flushing cache in position: %i\n\n", seekAddress);
    return EVAL_STATUS;
  } 
}

//                      .__            
//   ____ _____    ____ |  |__   ____  
// _/ ___\\__  \ _/ ___\|  |  \_/ __ \ 
// \  \___ / __ \\  \___|   Y  \  ___/ 
//  \___  >____  /\___  >___|  /\___  >
//      \/     \/     \/     \/     \/ 

state_t do_state_cache(state_data_t *state_data) {

  state_data->blen = state_data->bdoc->len;
  state_data->data = bson_get_data(state_data->bdoc); 

  //cache data locally, since the MQTT link is not available
  fseek(state_data->cache, 0, SEEK_END);//set the file position of the stream to tail
  fprintf(state_data->cache, "$%05zu", state_data->blen); //write length of data
  fwrite(state_data->data, state_data->blen, 1, state_data->cache); //write data
  fflush(state_data->cache);
  printf("cached %i bytes of data\n\n", state_data->blen);
  fflush(stdout);

	bson_destroy(state_data->bdoc);

  if(!testMode){
    //since i'm not connected try to reconnect
    mosquitto_reconnect(state_data->m);
  }

  return EVAL_STATUS;
}

//             ___.   .__  .__       .__     
// ______  __ _\_ |__ |  | |__| _____|  |__  
// \____ \|  |  \ __ \|  | |  |/  ___/  |  \ 
// |  |_> >  |  / \_\ \  |_|  |\___ \|   Y  \
// |   __/|____/|___  /____/__/____  >___|  /
// |__|             \/            \/     \/ 
// TODO: separare i dati in arrivo e pubblicare su topic differenti

state_t do_state_publish(state_data_t *state_data) {

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
  mosquitto_publish(state_data->m, NULL, state_data->ud.cfg->mqtt_topic, state_data->blen, state_data->data, 0, false);
  printf("> Sent %zu bytes.\n\n", state_data->blen);
  bson_destroy(state_data->bdoc);

  return EVAL_STATUS;
}

void dataGathering(int* to_send, int* index, int socket) {
  // Data Gathering
  // Timer setting

  // trigger must be setted in ms, data are sent every end of timer

  double msec = 0, trigger = 100, end = 0;
  struct timespec tstart={0,0}, tend={0,0};
  clock_gettime(CLOCK_MONOTONIC, &tstart);
  end = ((double)tstart.tv_sec*1000 + 1.0e-6*tstart.tv_nsec);

  int size = 90;
  *index = 0;
  to_send = (int *) malloc(size*sizeof(int));

  do {
    int id = 0; 
    int data1 = 0;
    int data2 = 0;

    receive_can_compact(socket,&id,&data1,&data2);

    //realloc
    if (size - 5 < *index) {
      //realloc
      size *= 2;
      to_send = (int *) realloc(to_send, size*sizeof(int));
    } 
  
    to_send[(*index)++] = id;
    to_send[(*index)++] = data1;
    to_send[(*index)++] = data2; 

    clock_gettime(CLOCK_MONOTONIC, &tend);
    msec = (((double)tend.tv_sec*1000 + 1.0e-6*tend.tv_nsec) - end);
  } while ( msec < trigger );
}