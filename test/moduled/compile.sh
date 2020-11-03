#!/bin/bash
echo "compiling..."
gcc main.test.c -o test.out \
    \
    ./libs/munit/munit.c \
    ./libs/test_utils/test_utils.c \
    \
    ../../utils/log_utils/log_utils.c \
    ../../utils/misc_utils/misc_utils.c \
    ../../utils/config_utils/config_utils.c \
    ../../utils/config_utils/jsmn/jsmn.c \
    ../../services/log_service/log_service.c \
    ../../services/config_service/config_service.c \
    ../../services/mongo_service/mongo_service.c \
    \
    \
    ./suites/utils/utils.test.c \
    \
    ./suites/utils/log_utils/log_utils.test.c \
    ./suites/utils/log_utils/log_info/log_info.test.c \
    ./suites/utils/log_utils/log_success/log_success.test.c \
    ./suites/utils/log_utils/log_debug/log_debug.test.c \
    ./suites/utils/log_utils/log_error/log_error.test.c \
    ./suites/utils/log_utils/log_warning/log_warning.test.c \
    \
    ./suites/utils/misc_utils/misc_utils.test.c \
    ./suites/utils/misc_utils/digit_counts/digit_counts.test.c \
    ./suites/utils/misc_utils/print_int_array/print_int_array.test.c \
    ./suites/utils/misc_utils/print_double_array/print_double_array.test.c \
    ./suites/utils/misc_utils/free_strings_array/free_strings_array.test.c \
    ./suites/utils/misc_utils/print_strings_array/print_strings_array.test.c \
    ./suites/utils/misc_utils/clone_strings_array/clone_strings_array.test.c \
    \
    ./suites/utils/config_utils/config_utils.test.c \
    ./suites/utils/config_utils/new_config/new_config.test.c \
    ./suites/utils/config_utils/print_config/print_config.test.c \
    ./suites/utils/config_utils/delete_config/delete_config.test.c \
    ./suites/utils/config_utils/parse_config_file/parse_config_file.test.c \
    \
    \
    ./suites/services/services.test.c \
    \
    ./suites/services/log_service/log_service.test.c \
    ./suites/services/log_service/debug_config_path/debug_config_path.test.c \
    ./suites/services/log_service/debug_config_path/assets/debug_config_path.assets.c \
    ./suites/services/log_service/debug_gps_port/debug_gps_port.test.c \
    ./suites/services/log_service/debug_gps_port/assets/debug_gps_port.assets.c \
    ./suites/services/log_service/debug_generic/debug_generic.test.c \
    \
    ./suites/services/config_service/config_service.test.c \
    ./suites/services/config_service/handle_config/handle_config.test.c \
    \
    ./suites/services/mongo_service/mongo_service.test.c \
    ./suites/services/mongo_service/mongo_setup/mongo_setup.test.c \
    \
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