//
// Created by taariq on 5/23/19.
//

#include "defs.h"
#include "utils.c"

void execute_dpi(current_state *state) {
    uint8_t opcode = state->decoded_instruction.opcode;
    uint16_t operand2 = state->decoded_instruction.operand2;

    int finalOp2 = 0;
    int shiftCarry = 0;
    int carry = 0;

    if (state->decoded_instruction.i) {
        //Immediate constant
        uint8_t rotate = mask_4_bit(operand2, 8);
        uint8_t imm = operand2 & 0xFF;
        //Zero extend to 32 bits and rotate right
        finalOp2 = ror((uint32_t) imm, (unsigned int) rotate * 2);
    } else {
        //Shifted Register
        uint8_t rm = mask_4_bit(operand2, 0);
        uint8_t shift = (operand2 >> 4) & 0xFF;
        uint8_t shiftType = (shift >> 1) & 0x3;
        uint8_t shiftAmount = 0;
        if (!(shift & 0x1)) {
            //Constant amount
            shiftAmount = (shift >> 3) & 0x1F;
        } else {
            //Register amount (Optional)
            uint8_t rs = mask_4_bit(shift, 8);
            int shiftRegister = (operand2 >> (12 - 4)) & 0x1F;
            int regVal = state->registers[shiftRegister];
            shiftAmount = (regVal & 0xFF);
        }

        uint32_t rm_value = state->registers[rm];

        //SHIFT
        switch (shiftType) {
            case 0:
                finalOp2 = lsl(rm_value, shiftAmount);
                //Bit 28 carry out
                shiftCarry = mask_1_bit(rm_value, 28);
                break;
            case 1:
                finalOp2 = lsr(rm_value, shiftAmount);
                //Bit 3 carry out
                shiftCarry = mask_1_bit(rm_value, 3);
                break;
            case 2:
                finalOp2 = asr(rm_value, shiftAmount);
                //Bit 3 carry out
                shiftCarry = mask_1_bit(rm_value, 3);
                break;
            case 3:
                finalOp2 = ror(rm_value, (unsigned int) shiftAmount);
                //Bit 3 carry out
                shiftCarry = mask_1_bit(rm_value, 3);
                break;
            default:
                printf("Invalid shift type");
        }
    }


    int returnValue = 0;
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
            returnValue = finalOp2 - rn;
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
    if (opcode != 8 && opcode != 9 && opcode != 10) {
        //printf("Opcode is %d. rn is %d. Finalop2 is %d, Set register %d to %d \n ", opcode, rn, finalOp2, state->decoded_instruction.rd, returnValue);
        set_register(state, state->decoded_instruction.rd, returnValue);
    }


    //Setting the CPSR
    if (state->decoded_instruction.s) {
        //C (30) bit is carry out
        set_CPSR_bit(state, C, carry);
        //Z (31) bit if result is zero
        if (returnValue == 0) {
            set_CPSR_bit(state, Z, 1);
        }
        //N (32) bit is bit 31 of result
        set_CPSR_bit(state, N, returnValue >> 31);
    }
}

void execute_sdt(current_state *state) {
    uint8_t opcode = state->decoded_instruction.opcode;
    uint16_t offset = state->decoded_instruction.offset;

    int finalOffset = 0;
    int shiftCarry = 0;
    int carry = 0;

    if (!state->decoded_instruction.i) {
        //Immediate constant
        uint8_t rotate = mask_4_bit(offset, 8);
        uint8_t imm = offset & 0xFF;
        //Zero extend to 32 bits and rotate right
        finalOffset = ror((uint32_t) imm, (unsigned int) rotate * 2);
    } else {
        //Shifted Register
        uint8_t rm = mask_4_bit(offset, 0);
        uint8_t shift = (offset >> 4) & 0xFF;
        uint8_t shiftType = (shift >> 1) & 0x3;
        uint8_t shiftAmount = 0;
        if (!(shift & 0x1)) {
            //Constant amount
            shiftAmount = (shift >> 3) & 0x1F;
        } else {
            //Register amount (Optional)
            uint8_t rs = mask_4_bit(shift, 8);
            int shiftRegister = (offset >> (12 - 4)) & 0x1F;
            int regVal = state->registers[shiftRegister];
            shiftAmount = (regVal & 0xFF);
        }

        uint32_t rm_value = state->registers[rm];

        //SHIFT
        switch (shiftType) {
            case 0:
                finalOffset = lsl(rm_value, shiftAmount);
                //Bit 28 carry out
                shiftCarry = mask_1_bit(rm_value, 28);
                break;
            case 1:
                finalOffset = lsr(rm_value, shiftAmount);
                //Bit 3 carry out
                shiftCarry = mask_1_bit(rm_value, 3);
                break;
            case 2:
                finalOffset = asr(rm_value, shiftAmount);
                //Bit 3 carry out
                shiftCarry = mask_1_bit(rm_value, 3);
                break;
            case 3:
                finalOffset = ror(rm_value, (unsigned int) shiftAmount);
                //Bit 3 carry out
                shiftCarry = mask_1_bit(rm_value, 3);
                break;
            default:
                printf("Invalid shift type");
        }
    }


    int rn = state->decoded_instruction.rn;
    int address = 0;
    int returnValue = 0;

    //if u is set, add offset, otherwise subtract
    int temprn = state->decoded_instruction.u ? rn + finalOffset : rn - finalOffset;

    if (state->decoded_instruction.p) {
        //pre-indexing - access memory at temprn and dont modify rn
        address = temprn;

    } else {
        //post-indexing - access memory at rn and rn becomes temprn
        address = rn;
        rn = temprn;

        if (rn == rm) {
            //Rm same as Rn is not allowed in post-indexing
            fprintf(stderr, "SDT, rn == rm not allowed in post-indexing\n");
            return;
        }

    }

    if (rn == PC && state->registers[PC] != state->address + 8) {
        //if PC used as base register, must contain instruction's address plus 8 bytes
        fprintf(stderr, "SDT, PC used as base register but does not contain instruction's address plus 8 bytes\n");
        return;
    }

    //gets memory value at this address;
    returnValue = state->memory[address];

    //sets desination register to memory value returned
    set_register(state, state->decoded_instruction.rd, returnValue);

}

void execute_mul(current_state *state) {
    //TODO
}

void execute_branch(current_state *state) {
    //manipulating offset appropriately for addition to PC
    uint32_t offset = state->decoded_instruction.offset;
    offset = offset << 2;
    offset = sign_extend_26_to_32(offset);

    //adding two's complement number
    state->registers[PC] += offset;
    state->decoded_instruction.type = NONE;
    state->fetched_instruction.binary_value = 0;

}