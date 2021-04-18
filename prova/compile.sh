#!/bin/bash
echo "transpiling..."
echo "compiling..."
gcc main.c -o send.out \
    ../utils/can_utils/can_utils.c \
    ../utils/can-cicd/naked-generator/Primary/c/Primary.c \
    ../utils/can-cicd/naked-generator/Secondary/c/Secondary.c \
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