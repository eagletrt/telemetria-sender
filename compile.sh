#!/bin/bash
echo "transpiling..."
npm run transpile
echo "compiling..."
gcc main.c -o sender.out \
    ./utils/misc_utils/misc_utils.c \
    ./utils/can_utils/can_utils.c \
    ./utils/log_utils/log_utils.c \
    ./utils/gather_utils/gather_utils.c \
    ./utils/config_utils/jsmn/jsmn.c \
    ./utils/config_utils/config_utils.c \
    ./utils/can-cicd/naked-generator/Primary/c/Primary.c \
    ./utils/can-cicd/naked-generator/Secondary/c/Secondary.c \
    ./services/config_service/config_service.c \
    ./services/gps_service/gps_service.c \
    ./services/mongo_service/mongo_service.c \
    ./services/log_service/log_service.c \
    ./services/can_service/can_service.c \
    ./services/mosquitto_service/mosquitto_service.c \
    ./services/gather_service/gather_service.c \
    ./services/gather_service/master/gather_master_service.c \
    ./services/gather_service/can/gather_can_primary_service.c \
    ./services/gather_service/can/gather_can_secondary_service.c \
    ./services/gather_service/gps/gather_gps_service.c \
    ./services/gather_service/sender/gather_sender_service.c \
    ./state_machine/state_machine.c \
    -I/usr/include/libbson-1.0 \
    -I/usr/include/libmongoc-1.0 \
    -lmongoc-1.0 \
    -lbson-1.0 \
    -lmosquitto \
    -lpthread \
    -lm
if [ $? -eq 0 ]; then
    echo "compiled :)"
    exit 0
else
    echo "Error in compilation :("
    exit 1
fi