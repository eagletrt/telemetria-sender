#ifndef MOSQUITTO_CUSTOM_LIB_H
#define MOSQUITTO_CUSTOM_LIB_H

//basic import
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <bson.h>
#include <mosquitto.h>

typedef struct {
	struct mosquitto *handler;

	char* broker_host;
	int broker_port;
	char* mqtt_topic;
} mosq_t;

extern int verbose;

mosq_t* mosquitto_setup(int port, char* host, char* topic);
int mosquitto_send(mosq_t* handler, bson_t* message);
int mosquitto_quit(mosq_t* handler);

#endif