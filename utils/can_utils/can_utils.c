#include "can_utils.h"

int canOpenSocket(const char* can_device, struct sockaddr_can* address) {
	int can_socket;
	struct ifreq ifr;

	can_socket = socket(PF_CAN, SOCK_RAW, CAN_RAW);
	if (can_socket < 0) {
		return -1;
	}

	strcpy(ifr.ifr_name, can_device);
	ioctl(can_socket, SIOCGIFINDEX, &ifr);

	(*address).can_family = AF_CAN;
	(*address).can_ifindex = ifr.ifr_ifindex;

	if (bind(can_socket, (struct sockaddr *) address, sizeof(*address)) < 0) {
		return -2;
	}

	return can_socket;
}

int canSendSimple(int socket, const char* data) {
	int length = strlen(data);

	if (length > 8) {
		printf("Error in canSendSimple, data too long");
		return -1;
	}

	struct can_frame frame;
	frame.can_id = 0x7FF;
	frame.can_dlc = length;

	for (int i = 0; i < length; ++i) {
		frame.data[i] = data[i];
	}

	return write(socket, &frame, sizeof(frame));
}

int canSend(int socket, int id, int length, const char* data) {
	if (length > 8) {
		printf("Error in canSendSimple, data too long");
		return -1;
	}

	struct can_frame frame;
	frame.can_id = id;
	frame.can_dlc = length;
	
	for (int i = 0; i < length; ++i) {
		frame.data[i] = data[i];
	}

	return write(socket, &frame, sizeof(frame));
}

int canReceiveSimple(int socket, char** data) {
	struct can_frame frame_read;
	int result = read(socket, &frame_read, 16);

	*data = (char*) malloc(frame_read.can_dlc * sizeof(char));

	for (int i = 0; i < frame_read.can_dlc; ++i) {
		(*data)[i] = frame_read.data[i];
	}

	return result;
}

int canReceive(int socket, int* id, char** data) {
	struct can_frame frame_read;
	int result = read(socket, &frame_read, 16);

	*data = (char*) malloc(frame_read.can_dlc * sizeof(char));
	*id = frame_read.can_id;

	for (int i = 0; i < frame_read.can_dlc; ++i) {
		(*data)[i] = frame_read.data[i];
	}

	return result;
}

int canReceiveCompact(int socket, int* id, int* dataLeft, int* dataRight) {
	struct can_frame frame_read;
	int result = read(socket, &frame_read, 16);

	*id = frame_read.can_id;
	*dataLeft = 0;
	*dataRight = 0;

	for (int i = 0; i < 4; ++i) {
		*dataLeft += (i < frame_read.can_dlc ? frame_read.data[i] << (8 * (3 - i)) : 0);
		*dataRight += ((i + 4) < frame_read.can_dlc ? frame_read.data[i + 4] << (8 * (3 - i)) : 0);
	}

	return result;
}
