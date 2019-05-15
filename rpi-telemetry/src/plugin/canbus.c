/**********************************************************************
 * canbus.c - acquire CANBus data and fill a can_data_t structure     *
 **********************************************************************/
#include "../linux/libsocketcan.h"
#include "../plugin.h"
#include "../can_custom_lib.h"
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
//Da dove li hai presi tutti questi? Il file di cui dispongo non contiene nulla di tutto ciò

#define STM_FRONTAL             0xC0
#define STM_CENTRAL             0xD0
#define STM_REAR                0xE0

int get_data(int** data_gathered, int data_lenght,can_data_t *data) {
  //printf("%li\n", data_gathered);

  struct timespec time;
  static uint32_t id = 0;
  int i = 0;
  uint32_t timestamp;

  if (clock_gettime(CLOCK_MONOTONIC, &time) == -1 ) {
    perror("clock gettime");
    return EXIT_FAILURE;
  }

  timestamp = time.tv_sec * 1000 + time.tv_nsec / 1E6;

  data->id = id++;
  data->timestamp = timestamp;

  for (int i = 0; i < data_lenght; i = i+3) {

    int id_gathered = data_gathered[i];
    int data1 = data_gathered[i+1];
    int data2 = data_gathered[i+2];

    int firstByte = ((data1 >> 24) & 255);
    printf("%d\n", firstByte);
    switch(id_gathered){
      case(0xB0):
        //pedals
        
        if (firstByte == 0x01) {
        	//throttle
        	data->throttle = ((data1 >> 16) & 255);
        } else {
          data->throttle = 0;
        }

        if (firstByte == 0x02) {
        	//brake
        	data->brake = ((data1 >> 16) & 255);
        }  else {
          data->brake = 0;
        }

      break;
    }
  }
  return EXIT_SUCCESS;
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