#include "can_custom.h"

int open_can_socket(char* can_device, struct sockaddr_can* addr) {
	int s;
	struct ifreq ifr;

	if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0 ) {
		//Error while opening socket
		printf("Error while opening socket");
		return -1;
	}


	strcpy(ifr.ifr_name, can_device);
	ioctl(s, SIOCGIFINDEX, &ifr);

	(*addr).can_family = AF_CAN;
	(*addr).can_ifindex = ifr.ifr_ifindex;

	if (bind(s, (struct sockaddr *) addr, sizeof(*addr)) < 0) {
		//Error in socket bind
		printf("Error in socket bind\n");
		return -2;
	}

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

int receive_can_compact(int socket, int* id, int* data1, int*data2) {
	struct can_frame frame_read;
	int ret = read(socket, &frame_read,16);

	__u8* data = (__u8*) malloc(8 * sizeof(__u8));
	*id = frame_read.can_id;

	*data1 = 0;
	*data2 = 0;

	*data1 += (0 < frame_read.can_dlc) ? (frame_read.data[0]<<24) : (0);
	*data1 += (1 < frame_read.can_dlc) ? (frame_read.data[1]<<16) : (0);
	*data1 += (2 < frame_read.can_dlc) ? (frame_read.data[2]<<8) : (0);
	*data1 += (3 < frame_read.can_dlc) ? (frame_read.data[3]<<0) : (0);
	
	*data2 += (4 < frame_read.can_dlc) ? (frame_read.data[4]<<24) : (0);
	*data2 += (5 < frame_read.can_dlc) ? (frame_read.data[5]<<16) : (0);
	*data2 += (6 < frame_read.can_dlc) ? (frame_read.data[6]<<8) : (0);
	*data2 += (7 < frame_read.can_dlc) ? (frame_read.data[7]<<0) : (0);

	return ret;
}