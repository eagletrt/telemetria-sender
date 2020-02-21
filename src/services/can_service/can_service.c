#include "can_service.h"

can_code canSetup() {
    can_code result;

    char* can_interface = condition.can.can_interface;
    struct sockaddr_can addr;
    int socket = canOpenSocket(can_interface, &addr);
    
    if (socket == -1) {
        result = CAN_SERVICE_CONNECTION_ERROR;
    }
    else if (socket == -2) {
        result = CAN_SERVICE_BINDING_ERROR;
    }
    else {
        condition.can.socket = socket;
        result = CAN_SERVICE_OK;
    }
    return result;
}

int canRead(int *id, int *data_left, int *data_right) {
    return canReceiveCompact(condition.can.socket, id, data_left, data_right);
}

can_code canAnswerWheel(int enabled) {
    can_code result = CAN_SERVICE_OK;
    int timestamp = time(NULL);

    char *data = (char*) malloc(sizeof(char) * 8);
	data[0] = 101;
	data[1] = (enabled ? 1 : 0);
	data[2] = condition.session.selected_pilot;
	data[3] = condition.session.selected_race;
	data[4] = (timestamp >> 24) & 0xFF;
	data[5] = (timestamp >> 16) & 0xFF;
	data[6] = (timestamp >> 8) & 0xFF;
	data[7] = (timestamp) & 0xFF;

	int outcome = canSend(condition.can.socket, 0xAB, 8, data);
    if (outcome < 0) {
        result = CAN_SERVICE_SENDING_ERROR;
    }
    
	free(data);
    return result;
}

char* canErrorMessage(can_code code) {
    switch (code)
    {
        case CAN_SERVICE_OK:
            return strdup("Can ok");
        
        case CAN_SERVICE_CONNECTION_ERROR:
            return strdup("Error in connecting to can");

        case CAN_SERVICE_BINDING_ERROR:
            return strdup("Error in binding to can");
        
        default:
            return strdup("Unknown can error code");
    }
}