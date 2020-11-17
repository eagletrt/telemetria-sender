#include "../../../../../services/config_service/config_service.h"

condition_t condition;

/*
    1 path
*/
int main(int argc, char *argv[]) {
	condition.config_path = (char *)malloc((strlen(argv[1]) + 1) * sizeof(char));
	strcpy(condition.config_path, argv[1]);

	fflush(stdout);
	int saved_stdout = dup(STDOUT_FILENO);
	dup2(open("/dev/null", O_WRONLY|O_CREAT, 0666), STDOUT_FILENO);
	config_code c = handleConfig();
	fflush(stdout);
	dup2(saved_stdout, STDOUT_FILENO);

	printf("mqtt.host:\"%s\"\n", condition.mqtt.host);
	printf("mqtt.port:%d\n", condition.mqtt.port);
	printf("mqtt.data_topic:\"%s\"\n", condition.mqtt.data_topic);
	printf("mqtt.log_topic:\"%s\"\n", condition.mqtt.log_topic);
	
	printf("mongodb.host:\"%s\"\n", condition.mongodb.host);
	printf("mongodb.port:%d\n", condition.mongodb.port);
	printf("mongodb.db:\"%s\"\n", condition.mongodb.db);
	printf("mongodb.collection:\"%s\"\n", condition.mongodb.collection);

	printf("gps.plugged:%d\n", condition.gps.plugged);
	printf("gps.simulated:%d\n", condition.gps.simulated);
	printf("gps.interface:\"%s\"\n", condition.gps.interface);

	printf("verbose:%d\n", condition.verbose);
	printf("sending_rate:%d\n", condition.structure.sending_rate);
	printf("can_interface:\"%s\"\n", condition.can.can_interface);

	printf("pilots:[");
	for (int i = 0; i < condition.session.pilots_count; i++) {
		printf(i != condition.session.pilots_count - 1 ? "\"%s\"," : "\"%s\"", condition.session.pilots[i]);
	}
	printf("]\n");

	printf("races:[");
	for (int i = 0; i < condition.session.races_count; i++) {
		printf(i != condition.session.races_count - 1 ? "\"%s\"," : "\"%s\"", condition.session.races[i]);
	}
	printf("]\n");

	return 0;
}