//
// Created by taariq on 5/22/19.
//

#ifndef ARM11_11_DEFS_H
#define ARM11_11_DEFS_H

#define NUM_ADDRESSES 65536
#define NUM_REGISTERS 17
#define PC 15
#define CPSR 16
#define INSTRUCTION_SIZE 32


typedef struct {
    uint8_t cond;
    uint32_t operation;
} decoded_instruction;

typedef struct {
    uint32_t binary_value;
    char *binary;
} fetched_instruction;

typedef struct {
    uint8_t memory[NUM_ADDRESSES];
    int32_t registers[NUM_REGISTERS];
    fetched_instruction fetched_instruction;
} current_state;

current_state INITIAL_STATE = {
        .memory = {0},
        .registers = {0}
};

#endif //ARM11_11_DEFS_H
