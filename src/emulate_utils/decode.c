//
// Created by taariq on 5/22/19.
//

#include <stdint.h>
#include "defs.h"
#include "utils.h"

void decode_dpi(current_state *state) {
    uint32_t value = state->fetched_instruction.binary_value;
    //NOTE: bits 32 - 28 corresponds to 0 to 4 bits; thus mask ending at the 4th bit
    state->decoded_instruction.cond = mask_4_bit(value, 28);
    state->decoded_instruction.i = mask_1_bit(value, 25);
    state->decoded_instruction.opcode = mask_4_bit(value, 21);
    state->decoded_instruction.s = mask_1_bit(value, 20);
    state->decoded_instruction.rn = mask_4_bit(value, 16);
    state->decoded_instruction.rd = mask_4_bit(value, 12);
    //Get the last 12 bits
        state->decoded_instruction.operand2 = value & 0xFFF;
    }

void decode_sdt(current_state *state) {

}

void decode_mul(current_state *state){

}



void decode_branch(current_state *state){
    uint32_t value = state->fetched_instruction.binary_value;
    state->decoded_instruction.cond = mask_4_bit(value, 28);
    state->decoded_instruction.offset = value & 0xFFFFFF;
}

void decode_branch(current_state *state){
    uint32_t value = state->fetched_instruction.binary_value;
    state->decoded_instruction.cond = mask_4_bit(value, 28);
    state->decoded_instruction.offset = value & 0xFFFFFF;
}
