//basic io function
#include <stdio.h>
#include "plugin/can_custom_lib.h"

int main(int argc, char *argv[]) {
	//socket
	int s;
	//bytes inviati
	int nbytes;
	//info per legare il socket al can
	struct sockaddr_can addr;

	//nome del can su quale mandare i dati
	char* name = (char*) malloc(sizeof(char)*5);
	strcpy(name,"vcan0");

	s = open_can_socket(name,&addr);

	int id = 0x123;
	char* data = (char*) malloc(8*sizeof(char));

	data[0] = 0x01;
	data[1] = 0x23;

	data[2] = 0x45;
	data[3] = 0x67;

	data[4] = 0x89;
	data[5] = 0xAB;

	data[6] = 0xCD;
	data[7] = 0xEF;

	nbytes = send_can(s,id,data);

	printf("Wrote %d bytes\n", nbytes);
	return 0;
}



