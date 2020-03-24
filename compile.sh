#!/bin/bash
#node ./javascript/
echo "transpiling..."
npm run transpile
echo "compiling..."
gcc main.c -o sender.out \
    ./utils/misc_utils/misc_utils.c \
    ./utils/can_utils/can_utils.c \
    ./utils/log_utils/log_utils.c \
    ./utils/config_utils/jsmn/jsmn.c \
    ./utils/config_utils/config_utils.c \
    ./services/config_service/config_service.c \
    ./services/gps_service/gps_service.c \
    ./services/mongo_service/mongo_service.c \
    ./services/log_service/log_service.c \
    ./services/can_service/can_service.c \
    ./services/mosquitto_service/mosquitto_service.c \
    ./services/structure_service/structure_service.c \
    ./state_machine/state_machine.c \
    -I/usr/include/libbson-1.0 \
    -I/usr/include/libmongoc-1.0 \
    -lmongoc-1.0 \
    -lbson-1.0 \
    -lmosquitto \
    -lpthread \
    -lm
echo "compiled :)"