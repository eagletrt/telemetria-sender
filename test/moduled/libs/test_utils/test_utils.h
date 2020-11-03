#ifndef TELEMETRY_TEST_UTILS
#define TELEMETRY_TEST_UTILS

/* IMPORTS */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define TEST_UTILS_BUFFER_SIZE 100

/* EXPORTED TYPES */

typedef struct {
    char* buffer[5 * TEST_UTILS_BUFFER_SIZE + 1];
    int out_pipe[2];
    int saved_stdout;
} redirect_config_t;

/* EXPORTED FUNCTIONS */

redirect_config_t init_redirect_config();
void redirect_stdout(redirect_config_t* config);
void recover_stdout(redirect_config_t* config);

#endif
