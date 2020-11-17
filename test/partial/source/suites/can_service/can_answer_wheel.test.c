#include "../../../../../services/can_service/can_service.h"
#include "../../../../../utils/can_utils/can_utils.h"

condition_t condition;

/*
    1 can_interface
    2 selected_pilot
    3 selected_race
    4 enabled
*/
int main(int argc, char *argv[]) {
	condition.can.can_interface = (char *)malloc((strlen(argv[1]) + 1) * sizeof(char));
	strcpy(condition.can.can_interface, argv[1]);
	condition.session.selected_pilot = atoi(argv[2]);
	condition.session.selected_race = atoi(argv[3]);
	int enabled = atoi(argv[4]);

	can_code res = canSetup();
	printf("%d\n", res);
	if (res != 0)
		return res;

	//canAnswerWheel(enabled);

	int outcome = canSendSimple(condition.can.socket, "1234567");
	printf("%d\n", outcome);


	free(condition.can.can_interface);
	return 0;
}