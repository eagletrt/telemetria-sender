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
int get_data(int** data_gathered, int data_lenght,can_data_t *data) {
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
  /*
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
  */
  return EXIT_SUCCESS;
}

new_config(argv[1], CFG_SUB)) == NULL