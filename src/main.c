#include "./state_machine/state_machine_condition.h"
#include "./state_machine/state_machine.h"

const state_codes ENTRY_STATE = INIT;
const state_codes EXIT_STATE = EXIT;

condition_t condition;

int main(int argc, char *argv[]) {
    state_codes current_state = ENTRY_STATE;
    result_codes result_code;
    result_codes (*state_function)();

    condition.config_path = argv[1];

    while(current_state != EXIT_STATE) {
        state_function = state[current_state];
        result_code = state_function();
        if (current_state != EXIT_STATE) {
            current_state = lookup_transitions(current_state, result_code);
        }
    }
    state[EXIT_STATE]();

    return 0;
}