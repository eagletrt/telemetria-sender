#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
//GPS
#include<fcntl.h>
#include<errno.h>
#include<termios.h>
#include<unistd.h>

#include "./lib/config_lib/config.h"
#include "./lib/config_lib/jsmn/jsmn.h"

#include "./lib/mongo_lib/mongo_custom.h"
#include "./lib/mosquitto_lib/mosquitto_custom.h"
#include "./lib/can_lib/can_custom.h"

#include "./lib/structure_lib/structure_custom.h"
#include "./lib/gps_lib/gps_custom.h"
#include <mosquitto.h>

//TRACING DEBUG GLOBAL VARIABLE
int verbose = 1;
int gps_plugged = 1; // WARN: change to 1 during tests

//TELEMETRY STATE
typedef enum {
	INIT = 0,
	SAVE = 1,
	IDLE = 2,
	EXIT = 3,
	ERROR = 4
} state_t;

//GLOBAL VARIABLES
state_t telemetria_state = INIT;
int quitting_signal = 0;

//HANDLERS - BETTER REMOVE THEM FROM HERE
config_t* config_file;
dbhandler_t* mongo_handler;
int can_socket;
int serial_port;
mosq_t* mosquitto_handler;

//SIGNATURES
void handle_signal(int s);
state_t switch_to(state_t *handler, state_t new_state);
int telemetry_handler(int id, int data1, int data2);
int fill_gps(data_t* data);

int main(int argc, char const *argv[]) {
	if (argc != 2) {
		if (verbose) printf("No argument passed. Usage: ./pub <cfg.json>\n");
		return 1;
	}

  	signal(SIGINT, handle_signal);
  	signal(SIGTERM, handle_signal);
	
	bson_t* message;
  	data_t* data_structure;
  	int message_sent = 0; //status_checker;

  	while (telemetria_state != EXIT) {
  		switch(telemetria_state) {
  			case INIT:
	  			config_file = config_setup(argv[1]);
					if (config_file == NULL) {
						telemetria_state = EXIT;
					} else {
						//config file is correct
						//opening mongo handler
						mongo_handler = mongo_setup(config_file->mongo_port,config_file->mongo_host,config_file->mongo_db);

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
				              	mosquitto_handler = mosquitto_setup(config_file->broker_port,config_file->broker_host,config_file->mqtt_topic,config_file->mqtt_log_topic);
							
								if (gps_plugged) {
									serial_port = openPort("/dev/ttyACM0");
								} else {
									serial_port = -1;
								}

								char *startup = (char*) malloc(sizeof(char)*23);
								strcpy(startup, "Telemetria started up\n");
								startup[22] = 0;
								mosquitto_log(mosquitto_handler,startup);
							}
						}
					}
				switch_to(&telemetria_state, SAVE); // WARN: change to IDLE during test
  			break;

  			case SAVE: case IDLE:
  				data_structure = data_setup();
  				data_gather(data_structure,config_file->sending_time, can_socket);
  				//GPS
  				if (gps_plugged)
  					fill_gps(data_structure);
  				data_elaborate(data_structure, &message);

		        if (telemetria_state == SAVE) { mongo_insert(message,mongo_handler); }
				mosquitto_send(mosquitto_handler,message);

				if (++message_sent > config_file->status_checker) {
					message_sent = 0;
				}

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
  		}
  	}
  	close(serial_port);
	mongo_quit(mongo_handler);
	config_quit(config_file);
	mosquitto_quit(mosquitto_handler);

	return 0;
}

void handle_signal(int s) {
	if (++quitting_signal == 2) {
		exit(0);
	} else {
		telemetria_state = EXIT;
	}
}

state_t switch_to(state_t *handler, state_t new_state) {
	if (new_state != INIT && new_state != ERROR && new_state != *handler) {
		*handler = new_state; 

		char *startup = (char*) malloc(sizeof(char)*36);
		strcpy(startup, "Telemetria state switched to: ");

		switch(new_state) {
			case SAVE:
				strcat(startup, "SAVE\n");
			break;
			case EXIT:
				strcat(startup, "EXIT\n");
			break;
			case IDLE:
				strcat(startup, "IDLE\n");
			break;
		}

		startup[35] = 0;
		mosquitto_log(mosquitto_handler,startup);
		free(startup);

		return new_state;
	}
	return ERROR;
}

	/* 
 	TELEMETRY HANDLER MSG EXPECTED
	ID = 0xAB
	[ 	
	telemetry config = 0
	status 			 = on=1 off=0
	pilot			 = 1-x
	race 			 = 1-y
	]
	*/

int telemetry_handler(int id, int data1, int data2) {
	int status = (data1 >> 16) & 0xFF;
	int pilot = (data1 >> 8) & 0xFF;
	int race = data1 & 0xFF;
	state_t result;

	if (status == 0) {
		result = switch_to(&telemetria_state, IDLE);
	} else if (status == 1) {
		result = switch_to(&telemetria_state, SAVE);
	}

	int change_coll = 0;
	if (config_file != NULL) {
		if (pilot < config_file->pilots_size && pilot != config_file->chosen_pilot) {
			change_coll = 1;
			config_file->chosen_pilot = pilot;
		}

		if (race < config_file->races_size && race != config_file->chosen_race) {
			change_coll = 1;
			config_file->chosen_race = race;
		}
	}

		int ctime = time(0);
	if (change_coll == 1) {
		
		mongo_set_collection(mongo_handler, config_file->pilots[config_file->chosen_pilot] , config_file->races[config_file->chosen_race] , ctime);

		char* message = (char*) malloc(sizeof(char)* (60 + strlen(config_file->pilots[config_file->chosen_pilot]) + strlen(config_file->races[config_file->chosen_race])));
		sprintf(message, "Collection name has been updated with \nPilot :\t%s\nRace :\t%s\n",config_file->pilots[config_file->chosen_pilot], config_file->races[config_file->chosen_race]);
		mosquitto_log(mosquitto_handler, message);
	}
		char *data = (char*) malloc (sizeof(char) * 8);
		data[0] = 101;
		data[1] = (result == SAVE) ? 1 : ((result == IDLE) ? 0 : -1);
		data[2] = pilot;
		data[3] = race;

		data[4] = (ctime >> 24) & 0xFF;
		data[5] = (ctime >> 16) & 0xFF;
		data[6] = (ctime >> 8) & 0xFF;
		data[7] = (ctime) & 0xFF;

		send_can(can_socket, 0xAB, 8, data);
		free(data);
	
} 

int fill_gps(data_t* data){
	GGA *GGA_struct = getGGAstruct(serial_port);

	if (GGA_struct != NULL) {

		data->gps.timestamp = atof(GGA_struct->UTCtime);

		data->gps.latitude = (char*) malloc(sizeof(char) * strlen(GGA_struct->latitude));
		strcpy(data->gps.latitude, GGA_struct->latitude);
		
		data->gps.longitude = (char*) malloc(sizeof(char) * strlen(GGA_struct->longitude));
		strcpy(data->gps.longitude, GGA_struct->longitude);
		
		//latitude and longitude
		/*
		int latitude_d = atoi(GGA_struct->latitude);	 
		int longitude_d = atoi(GGA_struct->longitude);
		
		double latitude_m = latitude_d % 100;
		double longitude_m = longitude_d % 100;
		
		data->gps.latitude = (latitude_d / 100) + (latitude_m/60);
		data->gps.longitude = (longitude_d / 100) + (longitude_m/60);
		*/

		/*
		data->gps.altitude = GGA_struct->altitude;
		data->gps.ns_indicator = malloc(sizeof(char) * 2);
		data->gps.ns_indicator[0] = GGA_struct->ns_indicator;
		data->gps.ns_indicator[1] = '\0';
		data->gps.ew_indicator = malloc(sizeof(char) * 2);
		data->gps.ew_indicator[0] = GGA_struct->ew_indicator;
		data->gps.ew_indicator[1] = '\0';
		*/

	}

	return 0;
}