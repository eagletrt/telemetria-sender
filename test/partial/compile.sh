#!/bin/bash

# mongo_start_session
gcc ./source/souites/mongo_services/mongo_start_session.test.c -o ./dist/souites/mongo_services/mongo_start_session.test.out \
    ../../utils/misc_utils/misc_utils.c \
    ../../services/mongo_service/mongo_service.c \
    -I/usr/include/libbson-1.0 \
    -I/usr/include/libmongoc-1.0 \
    -I/usr/include/libbson-1.0 \
    -I/usr/include/libmongoc-1.0 \
    -lmongoc-1.0 \
    -lbson-1.0 \
    -lmosquitto \
    -lpthread \
    -lm
if [ $? -eq 0 ]; then
    echo "mongo_services compiled"
else
    echo "Error in mongo_services"
    exit 1
fi


# mongo_insert
gcc ./source/souites/mongo_services/mongo_insert.test.c -o ./dist/souites/mongo_services/mongo_insert.test.out \
    ../../utils/misc_utils/misc_utils.c \
    ../../services/mongo_service/mongo_service.c \
    -I/usr/include/libbson-1.0 \
    -I/usr/include/libmongoc-1.0 \
    -I/usr/include/libbson-1.0 \
    -I/usr/include/libmongoc-1.0 \
    -lmongoc-1.0 \
    -lbson-1.0 \
    -lmosquitto \
    -lpthread \
    -lm
if [ $? -eq 0 ]; then
    echo "mongo_insert compiled"
else
    echo "Error in mongo_insert"
    exit 1
fi

# mongo_quit
gcc ./source/souites/mongo_services/mongo_quit.test.c -o ./dist/souites/mongo_services/mongo_quit.test.out \
    ../../utils/misc_utils/misc_utils.c \
    ../../services/mongo_service/mongo_service.c \
    -I/usr/include/libbson-1.0 \
    -I/usr/include/libmongoc-1.0 \
    -I/usr/include/libbson-1.0 \
    -I/usr/include/libmongoc-1.0 \
    -lmongoc-1.0 \
    -lbson-1.0 \
    -lmosquitto \
    -lpthread \
    -lm
if [ $? -eq 0 ]; then
    echo "mongo_quit compiled"
else
    echo "Error in mongo_quit"
    exit 1
fi

# mosquitto_setup
gcc ./source/souites/mosquitto_service/mosquitto_setup.test.c -o ./dist/souites/mosquitto_service/mosquitto_setup.test.out \
    ../../utils/misc_utils/misc_utils.c \
    ../../utils/log_utils/log_utils.c \
    ../../services/mosquitto_service/mosquitto_service.c \
    -I/usr/include/libbson-1.0 \
    -I/usr/include/libmongoc-1.0 \
    -I/usr/include/libbson-1.0 \
    -I/usr/include/libmongoc-1.0 \
    -lmongoc-1.0 \
    -lbson-1.0 \
    -lmosquitto \
    -lpthread \
    -lm
if [ $? -eq 0 ]; then
    echo "mongo_quit compiled"
else
    echo "Error in mongo_quit"
    exit 1
fi

echo "Everything compiled :)"
exit 0