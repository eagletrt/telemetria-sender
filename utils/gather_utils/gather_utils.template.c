#include "gather_utils.h"

long long int gatherGetCurrentTimestamp() {
	struct timeb timer_msec;
	if(!ftime(&timer_msec)) {
		return ((long long int) timer_msec.time) * 1000ll + ((long long int) timer_msec.millitm);
	}
	else {
		return -1;
	}
}

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