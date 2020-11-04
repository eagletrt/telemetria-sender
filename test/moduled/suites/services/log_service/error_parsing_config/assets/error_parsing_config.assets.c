#include "error_parsing_config.assets.h"

condition_t log_service_error_parsing_config_assets_conditions[3] = {
    {
        .config_path = "RUST_IN_PEACE"
    },
    {
        .config_path = "config_path"
    },
    {
        .config_path = "second_config_path"
    }
};

char* log_service_error_parsing_config_assets_results[3] = {
    "\x1b[31m[ERROR]\x1b[0m\t\tError in parsing config file RUST_IN_PEACE\n",
    "\x1b[31m[ERROR]\x1b[0m\t\tError in parsing config file config_path\n",
    "\x1b[31m[ERROR]\x1b[0m\t\tError in parsing config file second_config_path\n"
};