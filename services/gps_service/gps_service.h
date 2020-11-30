#ifndef TELEMETRY_GPS_SERVICE
#define TELEMETRY_GPS_SERVICE

/* IMPORTS */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include "../../state_machine/state_machine_condition.h"
#include "../../utils/log_utils/log_utils.h"

/* EXTERN */

extern condition_t condition;

/* TYPES */

/**
 * The struct for the $GGA gps messages
*/ 
typedef struct {
	double latitude;
	double longitude;
	double altitude;
	char* ns_indicator;
	char* ew_indicator;
	char* utc_time;
	int status;
} gps_gga_struct;

/**
 * The struct for the $GLL gps messages
*/ 
typedef struct {
	double latitude;
	double longitude;
	char* ns_indicator;
	char* ew_indicator;
	char* utc_time;
	int status;
} gps_gll_struct;

/**
 * The struct for the $VTG gps messages
*/ 
typedef struct {
	double ground_speed_knots;
	double ground_speed_human;
} gps_vtg_struct;

/**
 * The struct for the $RMC gps messages
*/ 
typedef struct {
	double latitude;
	double longitude;
	char* ns_indicator;
	char* ew_indicator;
	char* utc_time;
	char* date;
	int status;
	double ground_speed_knots;
} gps_rmc_struct;

/**
 * The struct that contains all the gps messages that will be stored in a single document
*/ 
typedef struct {
	gps_gga_struct **gga;
	int gga_count;
	gps_gll_struct **gll;
	int gll_count;
	gps_vtg_struct **vtg;
	int vtg_count;
	gps_rmc_struct **rmc;
	int rmc_count;
} gps_struct;

/* FUNCTIONS */

/**
 * Opens and returns the gps port
 * @return The serial port
*/ 
int openGPSPort();

/**
 * Preparing port to be simulated
*/ 
int prepareSimulatedPort();

/**
 * Reads data from the gps serial port and returns it as a structured object
 * @return The parsed data as a gps_struct object pointer
*/ 
gps_struct* readGPS();

/**
 * Deallocates the given gps_struct object
 * @param The gps_struct object that is to be deallocated
*/ 
void gpsFree(gps_struct* gps_data);

#endif


