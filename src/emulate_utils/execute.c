//
// Created by taariq on 5/23/19.
//

#include "defs.h"
#include "utils.c"

void execute_dpi(current_state *state) {
    uint8_t opcode = state->decoded_instruction.opcode;
    uint8_t i = state->decoded_instruction.i;
    uint16_t operand2 = state->decoded_instruction.operand2;
    int finalOp2;
    int shiftCarry;

    if (state->decoded_instruction.i) {
        //Immediate constant
        uint8_t rotate = mask_4_bit(operand2, 8);
        uint8_t imm = operand2 & 0xF;
        //Zero extend to 32 bits and rotate right
        finalOp2 = ror((uint32_t) imm, rotate * 2);
    } else {
        //Shifted Register
        uint8_t rm = operand2 & 0xF;
        uint8_t shift = (operand2 >> 4) & 0xFF;
        uint8_t shiftType = (shift >> 1) & 0x3;
        uint8_t shiftAmount = 0;
        if (!(shift & 0x1)) {
            //Constant amount
            shiftAmount = (shift >> 3) & 0x1F;
        } else {
            //Register amount (Optional)
            uint8_t rs = mask_4_bit(shift, 8);
        }

        uint32_t rm_value = state->registers[rm];

        //SHIFT
        switch (shiftType) {
            case 0:
                finalOp2 = lsl(rm_value, shiftAmount);
                shiftCarry
                break;
            case 1:
                finalOp2 = lsr(rm_value, shiftAmount);
                break;
            case 2:
                finalOp2 = asr(rm_value, shiftAmount);
                break;
            case 3:
                finalOp2 = ror(rm_value, (unsigned int) shiftAmount);
                break;
            default:
                printf("Invalid shift type");
        }
    }


    switch (opcode) {
        case 0:
            //AND
            break;
        case 1:
            //EOR/XOR
            break;
        case 2:
            //SUB
            break;
        case 3:
            //RSB
            break;
        case 4:
            //ADD
            break;
        case 8:
            //TST
            break;
        case 9:
            //TEQ
            break;
        case 10:
            //CMP
            break;
        case 12:
            //OR
            break;
        case 13:
            //MOV
            break;
    }


    //Setting the CPSR
    if (state->decoded_instruction.s) {

    }
}

void execute_sdt(current_state *state) {
    //TODO
}

void execute_mul(current_state *state) {
    //TODO
}

void execute_branch(current_state *state) {
    //TODO
}