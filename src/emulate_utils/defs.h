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
#define N 31
#define Z 30
#define C 29
#define V 28


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
    uint8_t p;
    uint8_t u;
    uint8_t opcode;
    uint8_t s;
    int32_t rn;
    uint8_t rd;
    uint8_t rs;
    uint8_t rm;
    uint8_t a;
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
    //needed for sdt
    int address;
} current_state;




#endif //ARM11_11_DEFS_H
