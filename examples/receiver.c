//basic io function
#include <stdio.h>
#include "plugin/can_custom_lib.h"

void printData(int id, char* data);

int main(int argc, char *argv[]) {
	//socket
	int s;
	//bytes ricevuti
	int nbytes;
	//info per legare il socket al can
	struct sockaddr_can addr;

	//nome del can su quale mandare i dati
	char* name = (char*) malloc(sizeof(char)*5);
	strcpy(name,"vcan0");

	s = open_can_socket(name,&addr);

	//ricezione e stampa
	int id;
	char* data;
	nbytes = receive_can(s,&id,&data);

	printData(id,data);	
	return 0;
}




void printData(int id, char* data) {
	// MAX string lenght	
	// 3 spaces for id
	// 1 white, 1 for "|", 1 white
	// 16 spaces for data (2 each byte) + 7 for the points.	
	// 1 for the terminator	
	
	printf("%03X \t[%li]  ",id, strlen(data));
	for (int i = 0;i<strlen(data); i++) {
		printf("%02X ", (data[i] & 0xFF));
	}
	printf("\n");
}
