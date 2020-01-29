#include <stdio.h>
#include <string.h>
#include "mosquitto_custom.h"

mosq_t* mosquitto_setup(int port, char* host, char* topic, char* log_topic){
  mosq_t* toRtn;
  toRtn = (mosq_t*) malloc(sizeof(mosq_t));

  if (verbose) printf("Mosquitto setup procedure started\n");

  mosquitto_lib_init();

  toRtn->handler = mosquitto_new("Eagle racing team - Telemetria", false, NULL);
  if (!toRtn->handler) {
    free(toRtn);
    if (verbose) printf("Something went wrong during the mosq client startup.\n");
    return NULL;
  }

  toRtn->broker_host = (char*) malloc(sizeof(char)*strlen(host));
  strcpy(toRtn->broker_host,host);
  toRtn->broker_port = port;
  toRtn->mqtt_topic = (char*) malloc(sizeof(char)*strlen(topic));
  strcpy(toRtn->mqtt_topic,topic);
  toRtn->mqtt_log_topic = (char*) malloc(sizeof(char)*strlen(log_topic));
  strcpy(toRtn->mqtt_log_topic,log_topic);

  mosquitto_connect(toRtn->handler, toRtn->broker_host, toRtn->broker_port, 60);
  if (verbose) printf("Connection Established\n");
  
  return toRtn;}

int mosquitto_send(mosq_t* handler, bson_t* message) {
  mosquitto_publish(handler->handler, NULL, handler->mqtt_topic, message->len, bson_get_data(message), 0, false);
	if (verbose) printf("Sent %d bytes of data", message->len);
  return 0;
}

int mosquitto_log(mosq_t* handler, char* message) {
  int len = 0;

  while (message != NULL && message[len] != '\0') {
    len++;
  }

  mosquitto_publish(handler->handler, NULL, handler->mqtt_log_topic, len, message, 0, false);
  if (verbose) printf("Logged %d bytes of data", len);
  return 0;
}

int mosquitto_quit(mosq_t* handler) {
  mosquitto_destroy(handler->handler);
  free(handler->broker_host);
  free(handler->mqtt_topic);
  free(handler->mqtt_log_topic);

  free(handler);
  mosquitto_lib_cleanup();
  return 0;}