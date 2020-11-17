#!/bin/bash

### --- MONGO ---

# mongo_start_session
gcc ./source/suites/mongo_services/mongo_start_session.test.c -o ./dist/suites/mongo_services/mongo_start_session.test.out \
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
gcc ./source/suites/mongo_services/mongo_insert.test.c -o ./dist/suites/mongo_services/mongo_insert.test.out \
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
gcc ./source/suites/mongo_services/mongo_quit.test.c -o ./dist/suites/mongo_services/mongo_quit.test.out \
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

### --- MOSQUITTO ---

# mosquitto_setup
gcc ./source/suites/mosquitto_service/mosquitto_setup.test.c -o ./dist/suites/mosquitto_service/mosquitto_setup.test.out \
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
    echo "mosquitto_setup compiled"
else
    echo "Error in mosquitto_setup"
    exit 1
fi

# mosquitto_send
gcc ./source/suites/mosquitto_service/mosquitto_send.test.c -o ./dist/suites/mosquitto_service/mosquitto_send.test.out \
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
    echo "mosquitto_send compiled"
else
    echo "Error in mosquitto_send"
    exit 1
fi

# mosquitto_log
gcc ./source/suites/mosquitto_service/mosquitto_log.test.c -o ./dist/suites/mosquitto_service/mosquitto_log.test.out \
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
    echo "mosquitto_log compiled"
else
    echo "Error in mosquitto_log"
    exit 1
fi

### --- CAN ---

# can_setup
gcc ./source/suites/can_service/can_setup.test.c -o ./dist/suites/can_service/can_setup.test.out \
    ../../utils/misc_utils/misc_utils.c \
    ../../utils/log_utils/log_utils.c \
    ../../utils/can_utils/can_utils.c \
    ../../services/can_service/can_service.c \
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
    echo "can_setup compiled"
else
    echo "Error in can_setup"
    exit 1
fi

# can_read
gcc ./source/suites/can_service/can_read.test.c -o ./dist/suites/can_service/can_read.test.out \
    ../../utils/misc_utils/misc_utils.c \
    ../../utils/log_utils/log_utils.c \
    ../../utils/can_utils/can_utils.c \
    ../../services/can_service/can_service.c \
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
    echo "can_read compiled"
else
    echo "Error in can_read"
    exit 1
fi

# can_answer_wheel
gcc ./source/suites/can_service/can_answer_wheel.test.c -o ./dist/suites/can_service/can_answer_wheel.test.out \
    ../../utils/misc_utils/misc_utils.c \
    ../../utils/log_utils/log_utils.c \
    ../../utils/can_utils/can_utils.c \
    ../../services/can_service/can_service.c \
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
    echo "can_answer_wheel compiled"
else
    echo "Error in can_answer_wheel"
    exit 1
fi

echo "Everything compiled :)"
exit 0