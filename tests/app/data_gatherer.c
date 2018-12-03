// This is a test for the data gathering from the can in the car

//COMPILE WITH:
//  $ gcc app/data_gatherer.c app/canbus.c can_custom_lib.c linux/libsocketcan.c

#include <stdio.h>
#include <time.h>
#include <string.h>
#include "../plugin.h"  
#include "../can_custom_lib.h"

int (*get_data)(int* data_gathered, int data_lenght,can_data_t *data);

int main(int argc, char const *argv[]) {
  /*
   *    SOME MOSQUITO/MONGO THINGS
   */

  //open the socket
  int socket;
  struct sockaddr_can addr1;

  char* name = (char*) malloc(sizeof(char)*5);
  strcpy(name,"vcan0");

  socket = open_can_socket(name,&addr1);
  //socket opened
  
  //actually the code is working with this type of cycle, for testing purposes as well
  for (int i = 0; i < 1000; ++i) {

    /*
     *    OTHER MANY MOSQUITO/MONGO THINGS
     */

    // Data Gathering
    // Timer setting

    // trigger must be setted in ms, data are sent every end of timer
    double msec = 0, trigger = 100, end = 0;
    struct timespec tstart={0,0}, tend={0,0};
    clock_gettime(CLOCK_MONOTONIC, &tstart);
    end = ((double)tstart.tv_sec*1000 + 1.0e-6*tstart.tv_nsec);

    int size = 90;
    int index = 0;
    int* to_send = (int *) malloc(size*sizeof(int));

    do {
      int id = 0; 
      int data1 = 0;
      int data2 = 0;

      receive_can_compact(socket,&id,&data1,&data2); 
      printf("X: %d  %lf\n", index, msec);

      //realloc
      if (size - 10 < index) {
        //realloc
        size *= 2;
        to_send = (int *) realloc(to_send, size*sizeof(int));
      } 
    
      to_send[index++] = id;
      to_send[index++] = data1;
      to_send[index++] = data2; 

      clock_gettime(CLOCK_MONOTONIC, &tend);
      msec = (((double)tend.tv_sec*1000 + 1.0e-6*tend.tv_nsec) - end);
    } while ( msec < trigger );

    //get_data(to_send,index,&can_data);
    
    /*
     *    STILL SOME OTHER MANY MOSQUITO/MONGO THINGS
     */
  }

  return 0;
}
