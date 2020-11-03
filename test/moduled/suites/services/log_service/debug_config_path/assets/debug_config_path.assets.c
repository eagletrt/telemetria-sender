#include "debug_config_path.assets.h"

condition_t log_service_debug_config_path_assets_conditions[3] = {
    {
        .verbose = 0,
        .config_path = "config_path"
    },
    {
        .verbose = 1,
        .config_path = "config_path"
    },
    {
        .verbose = 2,
        .config_path = "second_config_path"
    }
};

char* log_service_debug_config_path_assets_results[3] = {
    "",
    "\e[1;30m[DEBUG]\x1b[0m\t\tConfig file path is config_path\n",
    "\e[1;30m[DEBUG]\x1b[0m\t\tConfig file path is second_config_path\n"
};