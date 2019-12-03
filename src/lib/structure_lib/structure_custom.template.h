#ifndef STRUCT_H
#define STRUCT_H

#include <bson.h>

extern int verbose;
extern int receive_can_compact(int socket, int* id, int* data1, int*data2);

// {{GENERATE_STRUCTURE_CODE}}
data_t* data_setup();
int data_elaborate(data_t* data, bson_t** sending);
int data_quit(data_t* data);
int data_gather(data_t* data, int timing, int socket);

#endif