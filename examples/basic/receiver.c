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
		- si può inserire tutto in un semplice while. 
		- il codice contenuto in token di ha un receiver sempre aperto. 
		  quanto il processo riceve il token cambia e entra in modalità scrittura.
	
	2) Sender: controllare che supporti integer o nel caso scrivere una funzione d'appogio
		- Viene inviato semplicement un byte. Per ovvi motivi, stampando è più comodo usare l'esadecimale (sempre due cifre).
		  ma sta a noi come interpretarlo. il formato perfetto sarebbe unsigned char. (un byte in plain base2)

	3) Receiver: salvare tutti i frame in un file .csv
	4) Creare una libreria (05/11/2018)
	5) Integrare il codice al nostro progetto con il plugin (07/11/2018) 
*/

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
