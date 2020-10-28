#!/bin/bash
echo "compiling..."
gcc main.test.c -o test.out \
    ./munit/munit.c \
    ../../utils/misc_utils/misc_utils.c \
    ./utils/misc_utils/misc_utils.test.c \
    ./utils/misc_utils/clone_strings_array/clone_strings_array.test.c \
    ./utils/misc_utils/free_strings_array/free_strings_array.test.c \
    -lm
echo "compiled :)"