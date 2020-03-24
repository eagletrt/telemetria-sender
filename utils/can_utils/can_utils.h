#ifndef TELEMETRY_CAN_UTILS
#define TELEMETRY_CAN_UTILS

/* IMPORTS */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/can.h>
#include <linux/can/raw.h>

/* FUNCTIONS */

/**
 * This function opens the can socket
 * 
 * @param can_device The can device (es. "can0")
 * @param address The address that will be filled
 * @return The opened can socket. -1 if there was an error in opening the socket, -2 if there
 * was an error binding the socket
*/
int canOpenSocket(const char* can_device, struct sockaddr_can* address);

/**
 * This function sends data into the can
 * The id will be 0x7FFS
 * 
 * @param socket The can socket
 * @param data The message to send, as a char array
 * @return -1 if there was an error
*/
int canSendSimple(int socket, const char* data);

/**
 * This function sends data into the can
 * 
 * @param socket The can socket
 * @param id The id of the message
 * @param length The length of the message
 * @param data The message to send, as a char array
 * @return -1 if there was an error
*/
int canSend(int socket, int id, int length, const char* data);

/**
 * This function reads data from the can
 * 
 * @param socket The can socket
 * @param data The char array where the read data is saved
 * @return -1 if there was an error
*/
int canReceiveSimple(int socket, char** data);

/**
 * This function reads data from the can, retrieving also the id of the message
 * 
 * @param socket The can socket
 * @param id The integer wher the id of the message is saved
 * @param data The char array where the read data is saved
 * @return -1 if there was an error
*/
int canReceive(int socket, int* id, char** data);

/**
 * This function reads data from the can, specifying also an id and 
 * storing the data into two integers containing the numbers representing 
 * respectively the first four bytes and the last four bytes of the message
 * 
 * @param socket The can socket
 * @param id The integer wher the id of the message is saved
 * @param dataLeft The number contained in the first four bytes of the can message
 * @param dataRight The number contained in the last four bytes of the can message
 * @return -1 if there was an error
*/
int canReceiveCompact(int socket, int* id, int* dataLeft, int* dataRight);

#endif
