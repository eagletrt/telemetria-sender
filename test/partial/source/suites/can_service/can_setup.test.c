#include <bson.h>
#include <mongoc.h>
#include "../../../../../services/can_service/can_service.h"

condition_t condition;

/*
    1 can_interface
*/
int main(int argc, char *argv[]) {
    condition.can.can_interface = (char *)malloc((strlen(argv[1]) + 1) * sizeof(char));
    strcpy(condition.can.can_interface, argv[1]);

    can_code res = canSetup();
    free(condition.can.can_interface);
    return 0;
}