#include "gather_utils.h"

data_t* gatherCreateData() {
	data_t* data = (data_t*) malloc(sizeof(data_t));
	data->steering_wheel.marker = 0;
	// {{GENERATE_STRUCTURE_ALLOCATOR}}
	return data;
}

void gatherDeleteData(data_t *data) {
	//{{GENERATE_STRUCTURE_DEALLOCATOR}}
}

void gatherDataToBson(data_t *data, bson_t** bson_document) {
	//{{GENERATE_BSON}}
}