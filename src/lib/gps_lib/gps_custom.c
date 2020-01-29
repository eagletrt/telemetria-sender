/*GPS.c*/
#include<string.h>
#include<stdlib.h>
#include<stdio.h>

#include<fcntl.h>
#include<errno.h>
#include<termios.h>
#include<unistd.h>

#include "gps_custom.h"



int openPort(char *port){

	//open serial port
	int serial_port = open(port,O_RDWR);

	//check for errors
	if(serial_port < 0){

		printf("Error %i from open: %s\n",errno,strerror(errno));
		return -1;

	}

	//create new termios struct to configure port
	struct termios tty;
	memset(&tty, 0, sizeof tty);

	//read in existing settings, handle errors
	if(tcgetattr(serial_port, &tty) != 0){

		printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));

	}

	//the c_cflags member of the termios contains control fields

	tty.c_cflag &= ~PARENB; 	//disable parity bit
	tty.c_cflag &= ~CSTOPB;		//clear stop field
	tty.c_cflag |= CS8;		//8 data bits per byte
	tty.c_cflag &= ~CRTSCTS;	//disable TRS/CTS hardware flow control
	tty.c_cflag |= CREAD | CLOCAL;	//turn on READ and ignore control lines, setting CLOCAL allows us to read data

	//local modes
	tty.c_lflag &= ~ICANON;		//disable canonical mode, in canonical mode input data is received line by line, usually undesired when dealing with serial ports
	tty.c_lflag &= ~ECHO;		//if this bit (ECHO) is set, sent characters will be echoed back.
	tty.c_lflag &= ~ECHOE;
	tty.c_lflag &= ~ECHONL;
	tty.c_lflag &= ~ISIG;		//when the ISIG bit is set, INTR,QUIT and SUSP characters are interpreted. we don't want this with a serial port

	//input modes (c_iflag)
	//the c_iflag member of the termios struct contains low-level settings for input processing. the c_iflag member is an int
	tty.c_iflag &= ~(IXON | IXOFF | IXANY);						//clearing IXON,IXOFF,IXANY disable software flow control
	tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);	//clearing all of this bits disable any special handling of received bytes, i want raw data

	//output modes (c_oflag). the c_oflag member of the termios struct contain low level settings for output processing, we want to disable any special handling of output chars/bytes
	tty.c_oflag &= ~OPOST;		//prevent special interpretation of output bytes
	tty.c_oflag &= ~ONLCR;		//prevent conversion of newline to carriage return/line feed

	//setting VTIME VMIN
	tty.c_cc[VTIME] = 10;		//read() will block until either any amount of data is received or the timeout ocurs
	tty.c_cc[VMIN] = 0;

	//setting baud rate
	cfsetispeed(&tty, B460800);
	cfsetospeed(&tty, B460800);

	//after changing settings we need to save the tty termios struct, also error checking
	if(tcsetattr(serial_port, TCSANOW, &tty) != 0){

		printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));

	}

	return serial_port;

}

GLL GLLstring2GLLstruct(char *message){

	char *token;
	GLL message_struct;

	token = strtok(message,",");
	token = strtok(NULL,",");
	//float latitude = atof(token);
	//message_struct.latitude = format_coords(latitude);
	message_struct.latitude = token;
	token = strtok(NULL,",");
	message_struct.ns_indicator = token;
	token = strtok(NULL,",");
	message_struct.longitude = token;
	token = strtok(NULL,",");
	message_struct.ew_indicator = token;
	token = strtok(NULL,",");
	message_struct.UTCtime = token;
	token = strtok(NULL,",");
	message_struct.status = token;

	return message_struct;
}

GGA *GGAstring2GGAstruct(char *message){

	char *token;
	GGA *message_struct = (GGA*) malloc(sizeof(GGA));

	token = strtok(message,",");
	token = strtok(NULL,",");
	message_struct->UTCtime = token;
	token = strtok(NULL,",");
	message_struct->latitude = token;
	token = strtok(NULL,",");
	message_struct->ns_indicator = token;
	token = strtok(NULL,",");
	message_struct->longitude = token;
	token = strtok(NULL,",");
	message_struct->ew_indicator = token;
	token = strtok(NULL,",");
	message_struct->status = token;
	token = strtok(NULL,",");
	token = strtok(NULL,",");
	token = strtok(NULL,",");
	message_struct->altitude = token;

	return message_struct;
}
//starts reading untill one GGA message is catched, then turn it into a struct and terurns it
GGA* getGGAstruct(int serial_port){

	//reading is done through the read() function. a buffer is needed for linux to write the data into

	char read_buf[1024];
	memset(&read_buf,'\0',sizeof(read_buf));
	char *token;

	int num_bytes = read(serial_port,&read_buf,sizeof(read_buf));
	//printf("\nRead %i bytes. Received message:\n",num_bytes);
	if(num_bytes < 0){

		printf("Error reading: %s\n", strerror(errno));

	}
	//prendo una riga (fino al \n)
	token = strtok(read_buf,"\n");
	GGA* GGAstruct;
	while(token != NULL){
		//if cascade, controllo che tipo di messaggio arriva, e decido cosa farne
		if(strstr(token,"GGA")){
			GGAstruct = GGAstring2GGAstruct(token);
			printf("lat:%s\nlon:%s\nalt:%s\ntime:%s\nstatus:%s\n\n",GGAstruct->latitude,GGAstruct->longitude,GGAstruct->altitude,GGAstruct->UTCtime,GGAstruct->status);
			return GGAstruct;
		}
		token = strtok(NULL,"\n");
	}

}
