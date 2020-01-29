#!/bin/bash
node ./javascript/
gcc main.c -o pub ./lib/config_lib/jsmn/jsmn.c ./lib/config_lib/config.c ./lib/structure_lib/structure_custom.c ./lib/mongo_lib/mongo_custom.c ./lib/can_lib/can_custom.c ./lib/mosquitto_lib/mosquitto_custom.c ./lib/gps_lib/gps_custom.c  -I/usr/include/libbson-1.0 -I/usr/include/libmongoc-1.0 -lmongoc-1.0 -lbson-1.0 -lmosquitto #-lm -lc
echo "compiled :)"