#include <stdlib.h>
#include <stdio.h>
#include <printf.h>
#include <stdint.h>
#include "emulate_utils/utils.h"
#include "emulate_utils/execute.h"
#include "emulate_utils/decode.h"
#include "emulate_utils/defs.h"
//#include "emulate_utils/utils.c"
//#include "emulate_utils/execute.c"
//#include "emulate_utils/decode.c"


void execute(current_state *state) {
    instruction_type type = state->decoded_instruction.type;
    if (check_condition(state)) {
        switch (type) {
            case DPI:
                //printf ("executing DPI \n");
                execute_dpi(state);
                break;
            case SDT:
                //printf ("executing SDT \n");
                execute_sdt(state);
                break;
            case MUL:
                //printf ("executing mul \n");

                execute_mul(state);
                break;
            case BRANCH:
                //printf("executing branch \n");
                execute_branch(state);
                break;
            default:
                printf("Error in execute type");
        }
    } else {
        //printf ("\n not executing the above instruction \n");
    }
}

void decode(current_state *state) {
    uint32_t fetched_instruction = state->fetched_instruction.binary_value;

    if (fetched_instruction == 0) {
        state->decoded_instruction.type = ALL_ZERO;
    } else if (mask_1_bit(fetched_instruction, 27)) {
        //BRANCH
        //printf("decoding branch \n");
        state->decoded_instruction.type = BRANCH;
        decode_branch(state);
    } else if (mask_1_bit(fetched_instruction, 26)) {
        //SDT
        state->decoded_instruction.type = SDT;
        decode_sdt(state);
    } else if (mask_4_bit(fetched_instruction, 4) == 9) {
        //MULTIPLY
        state->decoded_instruction.type = MUL;
        decode_mul(state);
    } else if (!mask_1_bit(fetched_instruction, 27) & !mask_1_bit(fetched_instruction, 26) &
               mask_1_bit(fetched_instruction, 25)) {
        //DPI
        state->decoded_instruction.type = DPI;
        decode_dpi(state);
    } else {
        state->decoded_instruction.type = DPI;
        decode_dpi(state);
    }
}

//set fetched_instruction
void fetch(current_state *state) {
    state->fetched_instruction.binary_value =
            get_instruct(state, state->registers[PC]);
    state->address = state->registers[PC];
}


void pipeline_cycle(current_state *state, int size) {


    //infinite loop till halt encountered
    while (state->decoded_instruction.type != ALL_ZERO & state->registers[PC] < (size + 8)) {

        if (state->decoded_instruction.type != NONE) {
            execute(state);
        }
        if (state->fetched_instruction.binary_value != 0) {
            decode(state);
        } else if (state->decoded_instruction.type != NONE) {
            decode(state);
        }
        fetch(state);

        pc_increment(state);
    }


}


/*int main(int argc, char **argv) {

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
    current_state INITIAL_STATE = {
            .memory = {0},
            .registers = {0},
            .fetched_instruction.binary_value = 0,
            .decoded_instruction.type = NONE
    };
    *state = INITIAL_STATE;


    binary_file_loader(filename, (char *) state->memory);

    int size = get_file_size(filename);

    pipeline_cycle(state, size);

    print_registers(state->registers);
    print_binary(state->memory);
    free(state);

    return EXIT_SUCCESS;
}*/