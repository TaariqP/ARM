//
// Created by taariq on 5/23/19.
//

#include "defs.h"
#include "utils.h"
#include <stdio.h>


void execute_dpi(current_state *state) {
  uint8_t opcode = state->decoded_instruction.opcode;
  uint16_t operand2 = state->decoded_instruction.operand2;
  int32_t rn = state->decoded_instruction.rn;
  int rd = state->decoded_instruction.rd;
  //printf("value in this register is : %d \n", rn);


  int finalOp2 = 0;
  int shiftCarry = 0;
  int carry = 0;
  int32_t returnValue = 0;


  if (state->decoded_instruction.i) {
    //Immediate constant
    uint8_t rotate = mask_4_bit(operand2, 8);
    uint8_t imm = operand2 & 0xFF;
    uint32_t imm_32 = imm;
    //Zero extend to 32 bits and rotate right
    finalOp2 = ror(imm_32, (unsigned int) rotate * 2);
    //printf("finalop2: %d \n", finalOp2);
  } else {
    //Shifted Register
    uint8_t rm = mask_4_bit(operand2, 0);
    uint8_t shift = (operand2 >> 4) & 0xFF;
    uint8_t shiftType = (shift >> 1) & 0x3;
    uint8_t shiftAmount = 0;
    if (!(shift & 0x1)) {
      //Constant amount
      //printf("shift type: %d \n", shiftType);
      shiftAmount = (shift >> 3) & 0x1F;
    } else {
      //Register amount (Optional)
      uint8_t rs = mask_4_bit(shift, 8);
      int regVal = state->registers[rs];
      shiftAmount = (regVal & 0xFF);
    }

    int32_t rm_value = state->registers[rm];

    //SHIFT
    switch (shiftType) {
      case 0:
        finalOp2 = lsl(rm_value, shiftAmount);
        shiftCarry = mask_1_bit(rm_value, 31 - shiftAmount);
        //printf("shiftType = %d and carry = %d \n", shiftType, shiftCarry);

        break;
      case 1:
        finalOp2 = lsr(rm_value, shiftAmount);
        shiftCarry = mask_1_bit(rm_value, shiftAmount - 1);
        //printf("shiftType = %d and carry = %d \n", shiftType, shiftCarry);

        break;
      case 2:
        finalOp2 = asr(rm_value, shiftAmount);
        shiftCarry = mask_1_bit(rm_value, shiftAmount - 1);
        //printf("shiftType = %d and carry = %d \n", shiftType, shiftCarry);

        break;
      case 3:
        finalOp2 = ror(rm_value, (unsigned int) shiftAmount);
        shiftCarry = mask_1_bit(rm_value, shiftAmount - 1);
        //printf("shiftType = %d and carry = %d \n", shiftType, shiftCarry);

        break;
      default:
        printf("Invalid shift type");
    }
  }


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
      //printf("rn - finalop2 = %d - %d = %d \n", rn, finalOp2, returnValue);
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
      carry = (rn + finalOp2 < rn);
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
  //printf("rd should be 2: %d and return value end of thing: %d \n", state->decoded_instruction.rd, returnValue);

  //tst, teq, cmp do not write to rd
  if (opcode != 8 && opcode != 9 && opcode != 10) {
    //printf("Opcode is %d. rn is %d. Finalop2 is %d, Set register %d to %d \n ", opcode,
    // rn, finalOp2, state->decoded_instruction.rd, returnValue);
    state->registers[rd] = returnValue;
    //printf("register %d contains %d \n", rd, returnValue);
  }


  //Setting the CPSR
  if (state->decoded_instruction.s) {
    //C (30) bit is carry out
    set_CPSR_bit(state, C, carry);
    //Z (31) bit if result is zero
    set_CPSR_bit(state, Z, returnValue == 0);
    //N (32) bit is bit 31 of result
    set_CPSR_bit(state, N, mask_1_bit(returnValue, 31));
  }

}

void execute_sdt(current_state *state) {
  int16_t offset = state->decoded_instruction.offset;
  int8_t immediate = state->decoded_instruction.i;
  int16_t finalOffset = 0;
  int shiftCarry = 0;
  if (!immediate) {
    //Immediate constant
    finalOffset = offset;
  } else {
    //Shift Register
    uint8_t rm = mask_4_bit(offset, 0);
    uint8_t shift = (uint8_t) ((offset >> 4) & 0xFF);
    uint8_t shiftType = (uint8_t) ((shift >> 1) & 0x3);
    uint8_t shiftAmount = 0;
    if (!(shift & 0x1)) {
      //Constant amount
      shiftAmount = (uint8_t) ((shift >> 3) & 0x1F);
    } else {
      //Register amount (Optional)
      uint8_t rs = mask_4_bit(shift, 8);
      int regVal = state->registers[rs];
      shiftAmount = (uint8_t) (regVal & 0xFF);
    }
    int32_t rm_value = state->registers[rm];

    //SHIFT
    switch (shiftType) {
      case 0:
        finalOffset = (int16_t) lsl(rm_value, shiftAmount);
        shiftCarry = mask_1_bit(rm_value, 31 - shiftAmount);
        break;
      case 1:
        finalOffset = (int16_t) lsr(rm_value, shiftAmount);
        shiftCarry = mask_1_bit(rm_value, shiftAmount - 1);
        break;
      case 2:
        finalOffset = (int16_t) asr(rm_value, shiftAmount);
        shiftCarry = mask_1_bit(rm_value, shiftAmount - 1);
        break;
      case 3:
        finalOffset = (int16_t) ror(rm_value, shiftAmount);
        shiftCarry = mask_1_bit(rm_value, shiftAmount - 1);
        break;
      default:
        printf("Invalid shift type");
    }
  }
  int8_t rn = state->decoded_instruction.rn;
  int16_t address = finalOffset;
  int16_t value = finalOffset;
  if (state->decoded_instruction.u) {
    value += state->registers[rn];
  } else {
    value -= state->registers[rn];
  }
  if (state->decoded_instruction.p) {
    //pre-indexing - transferred data after offset altered
    address = value;
  } else {
    set_register(state, rn, value);
  }
  if (state->decoded_instruction.s) {
    //loading from memory to rd register
    set_register(state, state->decoded_instruction.rd, get_instruct(state, address));
  } else {
    //storing from register to memory
    uint8_t bytes[4];
    bytes[3] = mask_8_bit(value, 0);
    bytes[2] = mask_8_bit(value, 8);
    bytes[1] = mask_8_bit(value, 16);
    bytes[0] = mask_8_bit(value, 24);
    state->memory[address] = bytes[3];
    state->memory[address + 1] = bytes[2];
    state->memory[address + 2] = bytes[1];
    state->memory[address + 3] = bytes[0];
  }
}


void execute_mul(current_state *state) {
  int8_t acc = state->decoded_instruction.a;
  int8_t rm = state->decoded_instruction.rm;
  int8_t rs = state->decoded_instruction.rs;
  int8_t rd = state->decoded_instruction.rd;
  int8_t rn = state->decoded_instruction.rn;
  int64_t result = 0;
  if (acc) {
    result = state->registers[rm] * state->registers[rs] + state->registers[rn];
  } else {
    result = state->registers[rm] * state->registers[rs];
  }
  set_register(state, rd, (int32_t) result & 0xFFFFFFFF);

  if (state->decoded_instruction.s) {
    if (result == 0) {
      set_CPSR_bit(state, Z, 1);
    }
    //N (32) bit is bit 31 of result
    set_CPSR_bit(state, N, mask_1_bit(result, 31));
    set_CPSR_bit(state, Z, result == 0);
  }
}

void execute_branch(current_state *state) {
  //manipulating offset appropriately for addition to PC
  int32_t offset = state->decoded_instruction.offset;
  offset = offset << 2;
  offset = sign_extend_26_to_32(offset);

  //adding two's complement number
  state->registers[PC] += offset;
  state->fetched_instruction.binary_value = 0;

  /* may or may not need the line below, I can't see any changes to running */
  state->decoded_instruction.type = NONE;

}
