#include <stdlib.h>
#include <stdio.h>
#include <printf.h>
#include <stdint.h>
#include "emulate_utils/defs.h"
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

    if (fetched_instruction == 0) {
        state->decoded_instruction.type = NONE;
    } else if (!mask_1_bit(fetched_instruction, 27)) {
        //DATA PROCESSING
        state->decoded_instruction.type = DPI;
        decode_dpi(state);
    } else if (mask_1_bit(fetched_instruction, 7) && mask_1_bit(fetched_instruction, 4)) {
        //MULTIPLY
        state->decoded_instruction.type = MUL;
        decode_mul(state);
    } else if (mask_1_bit(fetched_instruction, 26)) {
        //SDT
        state->decoded_instruction.type = SDT;
        decode_sdt(state);
    } else if (mask_1_bit(fetched_instruction, 27)) {
        //BRANCH
        state->decoded_instruction.type = BRANCH;
        decode_branch(state);
    } else {
        state->decoded_instruction.type = DPI;
        decode_dpi(state);
    }
}

//set fetched_instruction
void fetch(current_state *state) {
    state->fetched_instruction.binary_value =
            get_instruct(state, state->registers[PC]);
}


void pipeline_cycle(current_state *state){
    //initialisation
    fetch(state);
    pc_increment(state);
    decode(state);
    fetch(state);
    pc_increment(state);

    //infinite loop till halt encountered
    while(state->decoded_instruction.type != ALL_ZERO){
            execute(state);
            decode(state);
            fetch(state);
            pc_increment(state);
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

    pipeline_cycle(state);

    print_registers(state->registers);
    print_binary(state->memory);

    return EXIT_SUCCESS;
}
