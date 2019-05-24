//
// Created by taariq on 5/22/19.
//

#ifndef ARM11_11_DEFS_H
#define ARM11_11_DEFS_H

#include <stdint.h>

#define NUM_ADDRESSES 65536
#define NUM_REGISTERS 17
#define PC 15
#define CPSR 16
#define INSTRUCTION_SIZE 32
#define N 32
#define Z 31
#define C 30
#define V 29


typedef enum {
    DPI,
    SDT,
    MUL,
    BRANCH,
    ALL_ZERO,
    NONE
} instruction_type;

typedef struct {
    instruction_type type;
    uint8_t cond;
    uint8_t i;
    uint8_t opcode;
    uint8_t s;
    uint8_t rn;
    uint8_t rd;
    uint32_t operand2;
    uint32_t offset;
} decoded_instruction;

typedef struct {
    uint32_t binary_value;
    char *binary;
} fetched_instruction;

typedef struct {
    uint8_t memory[NUM_ADDRESSES];
    int32_t registers[NUM_REGISTERS];
    fetched_instruction fetched_instruction;
    decoded_instruction decoded_instruction;
} current_state;

current_state INITIAL_STATE = {
        .memory = {0},
        .registers = {0}
};


#endif //ARM11_11_DEFS_H
