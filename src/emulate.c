#include <stdlib.h>
#include <stdio.h>
#include <printf.h>
#include <stdint.h>
#include "emulate_utils/defs.h"
#include "emulate_utils/utils.h"
#include "emulate_utils/execute.h"
#include "emulate_utils/execute.c"
#include "emulate_utils/decode.c"


void execute(current_state *state) {
    instruction_type type = state->decoded_instruction.type;
    if (check_condition(state)) {
        switch (type) {
            case DPI:
                execute_dpi(state);
                break;
            case SDT:
                execute_sdt(state);
                break;
            case MUL:
                execute_mul(state);
                break;
            case BRANCH:
                execute_branch(state);
                break;
            default:
                printf("Error in execute type");
        }
    }
}

//set decoded instruction
void decode(current_state *state){
    uint32_t fetched_instruction = state->fetched_instruction.binary_value;

    //TODO
}

//set fetched_instruction
void fetch(current_state *state) {
    state->fetched_instruction.binary_value =
            get_instruct(state, state->registers[PC]);
}

//initialisation:
//fetch first instruction, decode it.
//fetch second instruction,store it.
//trigger while
//in the while loop:
//fetch a new instruction
//decode previously fetched instruction, and store fetched instruction as fetched
//execute previously decoded instruction, and store decoded instruction as decoded
//increment PC by 4

void pipeline_cycle(current_state *state){
    if (state->fetched_instruction.binary_value == 0){
        fetch(state);
        pc_increment(state);
    } else if (state->decoded_instruction.type == NONE){
        
    }


}


int main(int argc, char **argv) {

    if (argc != 2) {
        printf("Provide an argument!");
        return EXIT_FAILURE;
    }

    char *filename = argv[1];
    FILE *input = fopen(filename, "rb");

    if (input == NULL) {
        printf("Invalid File");
        return EXIT_FAILURE;
    }

    current_state *state = malloc(sizeof(current_state));
    *state = INITIAL_STATE;



    binary_file_loader(filename, (char *) state->memory);

    decode_dpi(state);
    execute_dpi(state);

    print_registers(state->registers);
    print_binary(state->memory);

    return EXIT_SUCCESS;
}
