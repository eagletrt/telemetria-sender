/**********************************************************************
 * canbus.c - acquire CANBus data and fill a can_data_t structure     *
 **********************************************************************/
#include "../linux/libsocketcan.h"
#include "../plugin.h"
#include "../utils.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

// ID Messages
#define CAN_STATUS              0xDA
#define HV_STATE_ID             0xDB
#define ERRORS_STATUS           0xDD
#define EXEC_MODE_ID            0xDF
#define APPS_BSE_STATUS         0xDE
#define STEER_STATUS            0xE1
#define BMS_STATUS_ID           0x7EB
#define ACTUATORS_RANGE_ACK     0xBC
#define TH_BK_VALUE             0xB0

int get_data(can_data_t *data) {

  struct timespec time;
  static uint32_t id = 0;
  int i = 0;
  uint32_t timestamp;
  if( clock_gettime(CLOCK_MONOTONIC, &time) == -1 ) {
    perror("clock gettime");
    return EXIT_FAILURE;
  }
  timestamp = time.tv_sec * 1000 + time.tv_nsec / 1E6;  
  /*
    id = getId();
    payload = getPayload();

    parseCanMessages(id,payload,data);
  */
  return EXIT_SUCCESS;
}

void parseCanMessage(String id, can_data_t *data){

  /*
    Attraverso il plugin che hai implementato ho pensato di fare 
    un parsing dei messaggi in base al file excel che abbiamo per
    poi fare un porting a Fenice senza troppi problemi

    in base all'id dei messaggi si manda la funziona
    il resto mi sembra abbastanza intuitivo

  */

  switch(id){
    case(TH_BK_VALUE)

      setThrottleBrake();

      break;
  }
}

void setTime(){

   data->id = id++;
   data->timestamp = timestamp;
 
}

int getId(){
  
}

void getPayload(){
  /*
    funzione che restituisce un array di 8 byte con da decodificare in JSON 
    direttamente sul nostro can_data_t
  */
}

void setThrottleBrake(){
  setTime();
}


#if 0

  data->id = id++;
  data->timestamp = timestamp;
  
  data->location.latitude = 10 + rnd(-100, 100);
  data->location.longitude = 10 + rnd(-100, 100);
  data->location.elevation = 193 + rnd(-10, 10);

  data->speed = 0.0 + rnd(0, 50);
  data->odometry = 0.0 + rnd(0, 100);
  data->steering_angle = 0.0 + rnd(-20, 20);
  data->throttle = 0.0 + rnd(0, 1);
  data->brake = 0.0 + rnd(0, 1);
  data->acceleration[1].x = 0.0 + rnd(-0.5, 0.5);
  data->acceleration[1].y = 0.0 + rnd(-0.5, 0.5);
  data->acceleration[1].z = -1.0 + rnd(-0.1, 0.1);
  data->gyro[1].x = 0.0 + rnd(-0.5, 0.5);
  data->gyro[1].y = 0.0 + rnd(-0.5, 0.5);
  data->gyro[1].z = 0.0 + rnd(-0.1, 0.1);
  data->magneto[1].x = 0.0 + rnd(-3, 3);
  data->magneto[1].y = 0.0 + rnd(-3, 3);
  data->magneto[1].z = 0.0 + rnd(-3, 3);

  data->accumulator.voltage = 400 + rnd(-100, 0);
  data->accumulator.current[0] = 1.0 + rnd(-0.1, 0.1);
  data->accumulator.current[1] = 1.1 + rnd(-0.1, 0.1);
  data->accumulator.current[2] = 0.9 + rnd(-0.1, 0.1);
  for (i = 0; i < ACCUMULATOR_MODULES; i++) {
    data->accumulator.temperature[i] = 37 + rnd(0, 10);
  }
  return EXIT_SUCCESS;

#endif