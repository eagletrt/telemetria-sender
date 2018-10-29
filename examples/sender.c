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

int main(int argc, char *argv[]) {
	//socket
	int s;
	//bytes inviati
	int nbytes;
	
	struct sockaddr_can addr;
	struct can_frame frame;
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

	//setup del frame da mandare
	frame.can_id = 0x123;
	frame.can_dlc = 8;
	frame.data[0] = 0x12;
	frame.data[1] = 0x11;

	frame.data[2] = 0x13;
	frame.data[3] = 0xAF;

	frame.data[4] = 0xD3;
	frame.data[5] = 0x3E;

	frame.data[6] = 0x12;
	frame.data[7] = 0x11;

	//invio del frame + salvataggio dei byte inviati
	nbytes = write(s, &frame, sizeof(struct can_frame));
	printf("Wrote %d bytes\n", nbytes);
	
	return 0;
}



