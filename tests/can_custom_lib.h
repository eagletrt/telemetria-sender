/*
 * can_custom_lib.h
 *
 * Enhancement of the can utils lib for linux
 *
 * Authors: Ivan Martini <ivanmartini96@gmail.com>
 *          
 * This is a lib done exclusively for the FSAE Eagle Racing Team, from the University of Trento.
 * It provides some custom functionality to the existing can bus developed by volkswagen, in its linux version.
 * Everything is built on top of the raw canbus.
 *
 * See the examples to understand its full usage
 * 
 * == Still developing ==
 */

#ifndef CAN_CUSTOM_LIB_H
#define CAN_CUSTOM_LIB_H

//basic function
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

//sockets import
#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

//can utils import
#include <linux/can.h>
#include <linux/can/raw.h>

int open_can_socket(char* can_device, struct sockaddr_can* addr);

int simple_send_can(int socket, char* data);
int send_can(int socket, int id, char* data);

int simple_receive_can(int socket, char** data);
int receive_can(int socket, int* id,char** data);
int receive_can_compact(int socket, int* id,int* data1, int* data2);
#endif
