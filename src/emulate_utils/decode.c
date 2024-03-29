//
// Created by taariq on 5/22/19.
//
#include <stdio.h>
#include <stdint.h>
#include "defs.h"
#include "utils.h"

/*all 4 functions in this file fill in the decoded_instruction struct (only fills in the appropriate properties)*/

void decode_dpi(current_state *state) {
  uint32_t instruction = state->fetched_instruction.binary_value;
  int32_t *registers = state->registers;
  //NOTE: bits 32 - 28 corresponds to 0 to 4 bits; thus mask ending at the 4th bit
  state->decoded_instruction.cond = mask_4_bit(instruction, 28);
  state->decoded_instruction.i = mask_1_bit(instruction, 25);
  state->decoded_instruction.opcode = mask_4_bit(instruction, 21);
  state->decoded_instruction.s = mask_1_bit(instruction, 20);
  //printf("NEW DPI INSTRUCTION \n register with operand1 is : $%d \n", mask_4_bit(instruction, 16));
  int reg = mask_4_bit(instruction, 16);
  state->decoded_instruction.rn = registers[reg];
  //printf("value of operand1 is : %d \n", state->decoded_instruction.rn);
  state->decoded_instruction.rd = mask_4_bit(instruction, 12);
  //Get the last 12 bits
  state->decoded_instruction.operand2 = instruction & 0xFFF;
}

void decode_sdt(current_state *state) {
  uint32_t value = state->fetched_instruction.binary_value;

  state->decoded_instruction.cond = mask_4_bit(value, 28);
  state->decoded_instruction.i = mask_1_bit(value, 25);
  state->decoded_instruction.p = mask_1_bit(value, 24);
  state->decoded_instruction.u = mask_1_bit(value, 23);
  state->decoded_instruction.l = mask_1_bit(value, 20);
  state->decoded_instruction.rn = mask_4_bit(value, 16);
  state->decoded_instruction.rd = mask_4_bit(value, 12);
  //Could replace operand2 inside dpi with offset instead - use one instead of 2!
  state->decoded_instruction.offset = value & 0xFFF;
}

void decode_mul(current_state *state) {
  uint32_t value = state->fetched_instruction.binary_value;
  state->decoded_instruction.a = mask_1_bit(value, 21);
  state->decoded_instruction.s = mask_1_bit(value, 20);
  state->decoded_instruction.rd = mask_4_bit(value, 16);
  state->decoded_instruction.rn = mask_4_bit(value, 12);
  state->decoded_instruction.rs = mask_4_bit(value, 8);
  state->decoded_instruction.rm = value & 0xF;

}

void decode_branch(current_state *state) {
  uint32_t value = state->fetched_instruction.binary_value;
  state->decoded_instruction.cond = mask_4_bit(value, 28);
  state->decoded_instruction.offset = value & 0xFFFFFF;
}
