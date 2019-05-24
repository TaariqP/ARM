//
// Created by taariq on 5/23/19.
//

#include "defs.h"
#include "utils.c"

void execute_dpi(current_state *state) {
    uint8_t opcode = state->decoded_instruction.opcode;
    uint8_t i = state->decoded_instruction.i;
    uint16_t operand2 = state->decoded_instruction.operand2;

    int finalOp2 = 0;
    int shiftCarry = 0;
    int carry = 0;

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
                //Bit 28 carry out
                shiftCarry = mask_1_bit(finalOp2, 28);
                break;
            case 1:
                finalOp2 = lsr(rm_value, shiftAmount);
                //Bit 3 carry out
                shiftCarry = mask_1_bit(finalOp2, 3);
                break;
            case 2:
                finalOp2 = asr(rm_value, shiftAmount);
                //Bit 3 carry out
                shiftCarry = mask_1_bit(finalOp2, 3);
                break;
            case 3:
                finalOp2 = ror(rm_value, (unsigned int) shiftAmount);
                //Bit 3 carry out
                shiftCarry = mask_1_bit(finalOp2, 3);
                break;
            default:
                printf("Invalid shift type");
        }
    }


    int returnValue;
    int rn = state->decoded_instruction.rn;

    switch (opcode) {
        case 0:
            //AND
            returnValue = rn & finalOp2;
            carry = shiftCarry;
            break;
        case 1:
            //EOR/XOR
            returnValue = rn ^ finalOp2;
            carry = shiftCarry;
            break;
        case 2:
            //SUB
            returnValue = rn - finalOp2;
            carry = finalOp2 <= rn;
            break;
        case 3:
            //RSB
            returnValue = rn - finalOp2;
            carry = rn <= finalOp2;
            break;
        case 4:
            //ADD
            returnValue = rn + finalOp2;
            break;
        case 8:
            //TST
            returnValue = rn & finalOp2;
            carry = shiftCarry;
            break;
        case 9:
            //TEQ
            returnValue = rn ^ finalOp2;
            carry = shiftCarry;
            break;
        case 10:
            //CMP
            returnValue = rn - finalOp2;
            carry = finalOp2 <= rn;
            break;
        case 12:
            //OR
            returnValue = rn | finalOp2;
            carry = shiftCarry;
            break;
        case 13:
            //MOV
            returnValue = finalOp2;
            carry = shiftCarry;
            break;
    }

    //tst, teq, cmp do not write to rd
    if (opcode != 8 & opcode != 9 & opcode != 10) {
        set_register(state, state->decoded_instruction.rd, returnValue);
    }


    uint32_t cpsr = state->registers[CPSR];
    //Setting the CPSR
    if (state->decoded_instruction.s){
        //C (30) bit is carry out
        set_CPSR_bit(state, C, carry);
        //Z (31) bit if result is zero
        if (returnValue == 0){
            set_CPSR_bit(state, Z, 1);
        }
        //N (32) bit is bit 31 of result
        set_CPSR_bit(state, N, returnValue >> 31);
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