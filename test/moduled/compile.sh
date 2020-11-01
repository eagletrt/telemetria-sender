#!/bin/bash
echo "compiling..."
gcc main.test.c -o test.out \
    ./libs/munit/munit.c \
    ./libs/test_utils/test_utils.c \
    ../../utils/misc_utils/misc_utils.c \
    ./suites/utils/misc_utils/misc_utils.test.c \
    ./suites/utils/misc_utils/clone_strings_array/clone_strings_array.test.c \
    ./suites/utils/misc_utils/free_strings_array/free_strings_array.test.c \
    ../../utils/log_utils/log_utils.c \
    ./suites/utils/log_utils/log_utils.test.c \
    ../../utils/config_utils/config_utils.c \
    ../../utils/config_utils/jsmn/jsmn.c \
    ./suites/utils/config_utils/config_utils.test.c \
    ./suites/utils/config_utils/new_config_and_delete_config/new_config_and_delete_config.test.c \
    ./suites/utils/config_utils/parse_config_file/parse_config_file.test.c \
    -lm
echo "compiled :)"