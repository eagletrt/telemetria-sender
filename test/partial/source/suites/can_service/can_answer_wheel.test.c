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
	if (res < 0)
		return res;

	int outcome = canAnswerWheel(enabled);
	return outcome;
}