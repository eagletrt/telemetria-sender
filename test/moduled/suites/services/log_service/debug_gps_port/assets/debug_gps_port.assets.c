#include "debug_gps_port.assets.h"

condition_t log_service_debug_gps_port_assets_conditions[3] = {
    {
        .verbose = 0,
        .gps = {
            .port = 23
        }
    },
    {
        .verbose = 1,
        .gps = {
            .port = 1
        }
    },
    {
        .verbose = 1,
        .gps = {
            .port = 23
        }
    }
};

char* log_service_debug_gps_port_assets_results[3] = {
    "",
    "\e[1;30m[DEBUG]\x1b[0m\t\tGps port is 1\n",
    "\e[1;30m[DEBUG]\x1b[0m\t\tGps port is 23\n"
};