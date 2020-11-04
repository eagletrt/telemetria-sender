#include "info_new_session.assets.h"

mongodb_instance_t log_service_info_new_session_assets_mongodb_instances[3] = {
    { .session_name = "first" },
    { .session_name = "second" },
    { .session_name = "third" }
};

condition_t log_service_info_new_session_assets_conditions[3] = {
    {
        .mongodb = {
            .instance = &log_service_info_new_session_assets_mongodb_instances[0]
        }
    },
    {
        .mongodb = {
            .instance = &log_service_info_new_session_assets_mongodb_instances[1]
        }
    },
    {
        .mongodb = {
            .instance = &log_service_info_new_session_assets_mongodb_instances[2]
        }
    }
};

char* log_service_info_new_session_assets_results[3] = {
    "\x1b[34m[INFO]\x1b[0m\t\tCreated new session first\n",
    "\x1b[34m[INFO]\x1b[0m\t\tCreated new session second\n",
    "\x1b[34m[INFO]\x1b[0m\t\tCreated new session third\n"
};