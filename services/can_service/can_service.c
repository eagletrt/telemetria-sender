#include "can_service.h"

can_code canSetupPrimary() {
    can_code result;

    // Reads the can_interface from the conditions and opens the can
    char* can_interface = condition.can_primary.can_interface;
    struct sockaddr_can addr;
    int socket = canOpenSocket(can_interface, &addr);
    
    // Sets the right result depending on the outcome
    if (socket == -1) {
        result = CAN_SERVICE_CONNECTION_ERROR;
    }
    else if (socket == -2) {
        result = CAN_SERVICE_BINDING_ERROR;
    }
    else {
        condition.can_primary.socket = socket;
        result = CAN_SERVICE_OK;
    }

    return result;
}

can_code canSetupSecondary() {
    can_code result;

    // Reads the can_interface from the conditions and opens the can
    char* can_interface = condition.can_secondary.can_interface;
    struct sockaddr_can addr;
    int socket = canOpenSocket(can_interface, &addr);
    
    // Sets the right result depending on the outcome
    if (socket == -1) {
        result = CAN_SERVICE_CONNECTION_ERROR;
    }
    else if (socket == -2) {
        result = CAN_SERVICE_BINDING_ERROR;
    }
    else {
        condition.can_secondary.socket = socket;
        result = CAN_SERVICE_OK;
    }

    return result;
}

int canReadPrimary(int *id, char** data) {
    return canReceive(condition.can_primary.socket, id, data);
}

int canReadSecondary(int *id, char** data) {
    return canReceive(condition.can_secondary.socket, id, data);
}

can_code canAnswerWheel(int enabled) {
    can_code result = CAN_SERVICE_OK;

    // Create response
    uint8_t* buffer_message = (uint8_t*)malloc(8 * sizeof(uint8_t));
    Primary_Tlm_Status payload = enabled ? Primary_Tlm_Status_ON : Primary_Tlm_Status_OFF;
	serialize_Primary_TLM_STATUS(buffer_message, payload);

    // Send the message
	int outcome = canSend(condition.can_primary.socket, ID_TLM_STATUS, 8, buffer_message);
    // Set the result depending on the outcome
    if (outcome < 0) {
        result = CAN_SERVICE_SENDING_ERROR;
    }
    
    // Free the message
	free(buffer_message);
    
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