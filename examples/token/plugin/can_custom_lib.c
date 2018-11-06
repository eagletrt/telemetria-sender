#include "can_custom_lib.h"

int open_can_socket(char* can_device, struct sockaddr_can* addr) {
	int s;
	struct ifreq ifr;

	if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0 ) {
		//Error while opening socket
		printf("Error while opening socket\n");
		return -1;
	}


	strcpy(ifr.ifr_name, can_device);
	ioctl(s, SIOCGIFINDEX, &ifr);


	//addr = (struct (sockaddr_can*)) malloc(sizeof(sockaddr_can));
	(*addr).can_family = AF_CAN;
	(*addr).can_ifindex = ifr.ifr_ifindex;

	if (bind(s, (struct sockaddr *) addr, sizeof(*addr)) < 0) {
		//Error in socket bind
		printf("Error in socket bind\n");
		return -2;
	}

	printf("Socket opened on %s\n", can_device);

	return s;
}

int simple_send_can(int socket, char* data) {
	int len = strlen(data);

	if (len>8) {
		//Data are too long. Not sending.
		return -1;
	}

	//setting the frame to send
	struct can_frame frame;

	frame.can_id = 0x7FF;
	frame.can_dlc = len;
	
	for (int i = 0; i < len; ++i) {
		frame.data[i] = data[i];
	}

	//invio del frame + salvataggio dei byte inviati
	return write(socket, &frame, sizeof(struct can_frame));
}

int send_can(int socket, int id, char* data) {
	int len = strlen(data);

	if (len>8) {
		//Data are too long. Not sending.
		return -1;
	}

	//setting the frame to send
	struct can_frame frame;

	frame.can_id = id;
	frame.can_dlc = len;
	
	for (int i = 0; i < len; ++i) {
		frame.data[i] = data[i];
	}

	//invio del frame + salvataggio dei byte inviati
	return write(socket, &frame, sizeof(struct can_frame));
}

int simple_receive_can(int socket, char** data) {
	struct can_frame frame_read;
	int ret = read(socket, &frame_read,16);

	*data = (char*) malloc(frame_read.can_dlc * sizeof(char));

	for (int i = 0; i < frame_read.can_dlc; ++i) {
		(*data)[i] = frame_read.data[i];
	}

	return ret;
}

int receive_can(int socket, int* id,char** data) {
	struct can_frame frame_read;
	int ret = read(socket, &frame_read,16);

	*data = (char*) malloc(frame_read.can_dlc * sizeof(char));
	*id = frame_read.can_id;

	for (int i = 0; i < frame_read.can_dlc; ++i) {
		(*data)[i] = frame_read.data[i];
	}

	return ret;
}