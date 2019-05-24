//
// Created by taariq on 5/23/19.
//

#include "execute.h"
#include "defs.h"
#include "utils.h"
#include <stdlib.h>

void execute_dpi(current_state *state){
    uint8_t opcode = state->decoded_instruction.opcode;
    uint8_t i = state->decoded_instruction.i;
    uint16_t operand2 = state->decoded_instruction.operand2;
    if (state->decoded_instruction.i){
        //Immediate constant
        uint8_t rotate = mask_4_bit(operand2, 8);
        uint8_t imm = operand2 & 0xF;
        //Zero extend to 32 bits
        uint32_t imm_rotated = rotate_right((uint32_t) imm);
    }
    else{
        //Shifted Register
    }

}

void execute_sdt(current_state *state){
    //TODO
}

void execute_mul(current_state *state){
    //TODO
}

void execute_branch(current_state *state){
    //TODO
}