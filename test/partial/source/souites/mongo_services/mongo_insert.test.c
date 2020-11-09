#include <bson.h>
#include <mongoc.h>
#include "../../../../../services/mongo_service/mongo_service.h"
#include "../../../../../state_machine/state_machine_condition.h"


condition_t condition;

/*
    1 host
    2 port
    3 db
    4 collection
    5 json
*/
int main(int argc, char *argv[]) {
    condition.mongodb.host = (char *)malloc((strlen(argv[1]) + 1) * sizeof(char));
    condition.mongodb.db = (char *)malloc((strlen(argv[3]) + 1) * sizeof(char));
    condition.mongodb.collection = (char *)malloc((strlen(argv[4]) + 1) * sizeof(char));
    strcpy(condition.mongodb.host, argv[1]);
    condition.mongodb.port = atoi(argv[2]);
    strcpy(condition.mongodb.db, argv[3]);
    strcpy(condition.mongodb.collection, argv[4]);
    
    bson_t *b;
    bson_error_t error;
    b = bson_new_from_json (argv[5], -1, &error);

    mongoSetup();
    int res = mongoInsert(b);
    free(b);
    return res;
}