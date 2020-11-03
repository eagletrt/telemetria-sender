#include "./test_utils.h"

/* HELPER CONSTANTS */

static const int BUFFER_SIZE = 100;

/* EXPORTED FUNCTIONS */

redirect_config_t init_redirect_config() {
    redirect_config_t redirect_config = {.buffer = {0}};
    return redirect_config;
}

void redirect_stdout(redirect_config_t* config) {
    config->saved_stdout = dup(STDOUT_FILENO); //salvataggio addr di stdout
    if( pipe(config->out_pipe) != 0 ) exit(1); //apertura pipe
    dup2(config->out_pipe[1], STDOUT_FILENO); //ridirezione stdout nel pipe
    close(config->out_pipe[1]); //chiusura pipe
}

void recover_stdout(redirect_config_t* config) {
    fflush(stdout);
    fcntl(config->out_pipe[0], F_SETFL, O_NONBLOCK);
    read(config->out_pipe[0], config->buffer, 5 * TEST_UTILS_BUFFER_SIZE); //lettura pipe nel buffer
    dup2(config->saved_stdout, STDOUT_FILENO); //ripristino stdout
}