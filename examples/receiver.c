	
//basic io function
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

//sockets import
#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

//can utils import
#include <linux/can.h>
#include <linux/can/raw.h>

void printFrame(struct can_frame* frame);

int main(int argc, char *argv[]) {
	//socket
	int s;
	//bytes inviati
	int nbytes;
	
	struct sockaddr_can addr;
	struct ifreq ifr;

	//nome del can su quale mandare i dati
	const char* ifname = "vcan0";

	//tentativo di apertura socket can
	if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0 ) {
		perror("Error while opening socket");
		return -1;
	}

	strcpy(ifr.ifr_name, ifname);
	ioctl(s, SIOCGIFINDEX, &ifr);

	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	printf("%s at index %d\n", ifname, ifr.ifr_ifindex);

	//binding del socket can sul socket
	if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("Error in socket bind");
		return -2;
	}

	//ricezione e stampa
	struct can_frame frame_read;
	read(s, &frame_read,16);
	printFrame(&frame_read);
	
	return 0;
}

/*
	Molto bene, l'idea mi è piaciuta.
	Ti ho modificato can_manual.sh perchè alcuni comandi non erano essenziali.
	Come prossime modifiche penso sia il caso di lasciare il receiver aperto
	in modo tale da poter continuare ad inviare sia con sendere che con cansend vcan0 
	e chiudere il programma con un Ctrl+C.
	Per quanto riguarda invece il formato  servirebbe una funzione d'appogio per
	poter utilizzare integer al posto di valori esadecimanli.
	Non penso sia un problema per frame.data passargli integere ma va provato

	Per riassumere:
	1) Receiver sempre aperto
	2) Sender: controllare che supporti integer o nel caso scrivere una funzione d'appogio
	3) Receiver: salvare tutti i frame in un file .csv
	4) Creare una libreria (05/11/2018)
	5) Integrare il codice al nostro progetto con il plugin (07/11/2018) 
*/

void printFrame(struct can_frame* frame) {
	// MAX string lenght	
	// 3 spaces for id
	// 1 white, 1 for "|", 1 white
	// 16 spaces for data (2 each byte) + 7 for the points.	
	// 1 for the terminator	
	
	printf("%03X \t[%u]  ",(*frame).can_id, (*frame).can_dlc);
	for (int i = 0; i<(*frame).can_dlc; i++) {
		printf("%02X ",(*frame).data[i]);
	}
	printf("\n");
}
