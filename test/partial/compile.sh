#!/bin/bash

# mongo_services
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

echo "Everything compiled :)"
exit 0