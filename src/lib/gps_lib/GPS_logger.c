//GPS_logger.c
//E-AGLE
//Filippo Gastaldello
//2019-12-12

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include<fcntl.h>
#include<errno.h>
#include<termios.h>
#include<unistd.h>

#include "GPS.h"

//signatures
char* format_coords(float coord);


int main(int argc, char *argv[]){

	int serial_port = openPort("/dev/ttyACM0");

	GGA *GGA_struct;
	while(1){

		GGA_struct = getGGAstruct(serial_port);
		//printf("main\nlat:%s\nlon:%s\nalt:%s\ntime:%s\nstatus:%s\n\n",GGA_struct.latitude,GGA_struct.longitude,GGA_struct.altitude,GGA_struct.status);

	}

	close(serial_port);

	return 0;

}


