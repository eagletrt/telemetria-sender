#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "../utils/can-cicd/includes-generator/Primary/ids.h"
#include "../utils/can-cicd/includes-generator/Secondary/ids.h"
#include "../utils/can-cicd/naked-generator/Primary/c/Primary.h"
#include "../utils/can-cicd/naked-generator/Secondary/c/Secondary.h"
#include "../utils/can_utils/can_utils.h"

int main() {
	uint8_t* buffer_a = (uint8_t*)malloc(8 * sizeof(uint8_t));
	serialize_Primary_HV_VOLTAGE(3, 3, 3, 3, buffer_a, 8);

	uint8_t* buffer_b = (uint8_t*)malloc(8 * sizeof(uint8_t));
	serialize_Primary_HV_VOLTAGE(5, 5, 5, 5, buffer_b, 8);

	char* can_interface = "can0";
	struct sockaddr_can addr;
	int socket = canOpenSocket(can_interface, &addr);

	canSend(socket, ID_HV_VOLTAGE, 8, buffer_a);
	canSend(socket, ID_HV_VOLTAGE, 8, buffer_b);

	uint8_t* buffer_c = (uint8_t*)malloc(8 * sizeof(uint8_t));
	serialize_Secondary_IMU_ACCELERATION(5, 5, 5, buffer_c, 8);

	char* can_interface_2 = "can1";
	struct sockaddr_can addr_2;
	int socket_2 = canOpenSocket(can_interface_2, &addr_2);

	canSend(socket_2, ID_IMU_ACCELERATION, 8, buffer_c);

	return 0;
}