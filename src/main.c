#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#include "./lib/config_lib/config.h"
#include "./lib/config_lib/jsmn/jsmn.h"

#include "./lib/mongo_lib/mongo_custom.h"
#include "./lib/mosquitto_lib/mosquitto_custom.h"
#include "./lib/can_lib/can_custom.h"

#include "./lib/structure_lib/structure_custom.h"

#include <mosquitto.h>

//TRACING DEBUG GLOBAL VARIABLE
int verbose = 2;

//TELEMETRY STATE
typedef enum {
	INIT = 0,
	SEND = 1,
	PAUSE = 2,
	EXIT = 3,
} state_t;

//GLOBAL VARIABLES
state_t telemetria_state = INIT;
int quitting_signal = 0;

//SIGNATURES
void handle_signal(int s);

int main(int argc, char const *argv[]) {
	if (argc != 2) {
		if (verbose) printf("No argument passed. Usage: ./pub <cfg.json>\n");
		return 1;
	}

  	signal(SIGINT, handle_signal);
  	signal(SIGTERM, handle_signal);

  	config_t* config_file;
  	dbhandler_t* mongo_handler;
  	int can_socket;
  	mosq_t* mosquitto_handler;
	
	bson_t* message;
  	data_t* data_structure;


  	while (telemetria_state != EXIT) {
  		switch(telemetria_state) {
  			case INIT:
	  			config_file = config_setup(argv[1]);
					if (config_file == NULL) {
						telemetria_state = EXIT;
					} else {
						//config file is correct
						//opening mongo handler
						mongo_handler = mongo_setup(config_file->mongo_port,config_file->mongo_host,config_file->mongo_db,config_file->mongo_collection);

						if (mongo_handler == NULL) {
							telemetria_state = EXIT;
						} else { 
							//opening a connection to the canbus
							struct sockaddr_can addr;
							char* name = (char*) malloc(sizeof(char)*5);
							strcpy(name,"can0");
							can_socket = open_can_socket(name,&addr);
							
							if (can_socket <= 0) {
				              telemetria_state = EXIT;
				            } else {
				              	//setting up mosquitto
				              	mosquitto_handler = mosquitto_setup(config_file->broker_port,config_file->broker_host,config_file->mqtt_topic);

								bson_t* insert = BCON_NEW (
									"info", BCON_UTF8 ("Telemetria started up")
									);
								mosquitto_send(mosquitto_handler,insert);
								bson_destroy(insert);
							}
						}
					}
				telemetria_state = SEND;			
  			break;

  			case SEND:
  				data_structure = data_setup();
  				data_gather(data_structure,config_file->sending_time, can_socket);
  				data_elaborate(data_structure, &message);

		        mongo_insert(message,mongo_handler);
				mosquitto_send(mosquitto_handler,message);


		        if (verbose) { 
		        	char* str;
		        	if (verbose == 2) {
		        		str = bson_as_relaxed_extended_json (message, NULL);
		        	} else {
		        		str = (char*) malloc(sizeof(char)*10);
		        		strcpy(str, "JSON sent\0");
		        	}

					printf ("\n%s.\nFor a total of %d bytes\n\n", str,message->len);
					bson_free (str);
		    	}
	    	    bson_destroy(message);
	    	    data_quit(data_structure);	
  			break;

  			case PAUSE:
				telemetria_state = EXIT;
  			break;
  		}
  	}
	mongo_quit(mongo_handler);
	config_quit(config_file);

	return 0;
}

void handle_signal(int s) {
	if (++quitting_signal == 2) {
		exit(0);
	} else {
		telemetria_state = EXIT;
	}
}
