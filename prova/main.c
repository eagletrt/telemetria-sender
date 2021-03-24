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

#ifndef IDS_H
#define IDS_H

#define NETWORK_VERSION 1.2f

/* TOPIC ACU */
#define TOPIC_ACU_MASK 0b00000011111
#define TOPIC_ACU_FILTER 0b00000000000
#define ID_TS_STATUS 0b00000000000
#define ID_STEER_STATUS 0b00000100000
#define ID_SET_CAR_STATUS 0b00001000000
#define ID_GET_CAR_STATUS 0b00001100000

/* TOPIC ACUnSTEER */
#define TOPIC_ACUnSTEER_MASK 0b00000011111
#define TOPIC_ACUnSTEER_FILTER 0b00000000001
#define ID_HV_VOLTAGE 0b01100000001
#define ID_HV_CURRENT 0b01100100001
#define ID_HV_TEMP 0b01101000001
#define ID_HV_ERROR 0b00100000001

/* TOPIC BMS_HV */
#define TOPIC_BMS_HV_MASK 0b00000011111
#define TOPIC_BMS_HV_FILTER 0b00000000010
#define ID_SET_TS_STATUS 0b00000000010

/* TOPIC STEER */
#define TOPIC_STEER_MASK 0b00000011111
#define TOPIC_STEER_FILTER 0b00000000011
#define ID_TLM_STATUS 0b00100000011
#define ID_CAR_STATUS 0b00000000011

/* TOPIC TLM */
#define TOPIC_TLM_MASK 0b00000011111
#define TOPIC_TLM_FILTER 0b00000000100
#define ID_SET_TLM_STATUS 0b00100000100

#endif

#include "../utils/can-cicd/acunsteer.h"
#include "../utils/can_utils/can_utils.h"

int main() {
	HV_VOLTAGE value_a = {
		3, 3, 3, 3};
	uint8_t* buffer_a = (uint8_t*)malloc(8 * sizeof(uint8_t));
	serialize_HV_VOLTAGE(&value_a, buffer_a, sizeof(value_a));

	HV_VOLTAGE value_b = {
		5, 5, 5, 5};
	uint8_t* buffer_b = (uint8_t*)malloc(8 * sizeof(uint8_t));
	serialize_HV_VOLTAGE(&value_b, buffer_b, sizeof(value_b));

	char* can_interface = "can0";
	struct sockaddr_can addr;
	int socket = canOpenSocket(can_interface, &addr);


	canSend(socket, ID_HV_VOLTAGE, 8, buffer_a);
	canSend(socket, ID_HV_VOLTAGE, 8, buffer_b);

	return 0;
}