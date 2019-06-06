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
// 0x2020 0028
#define GPIO_CLEAR 538968104
// 0x2020 001C
#define GPIO_ON 538968092
//0x20200000
#define GPIO_0_9  538968064
//0x2020 0004
#define GPIO_10_19 538968068
//0x2020 0008
#define GPIO_20_29 538968072

/*following enum is used to identify the type of instruction*/
typedef enum {
    DPI,
    SDT,
    MUL,
    BRANCH,
    ALL_ZERO,
    NONE
} instruction_type;

/*contains all the possible partitions of the 32 bit integer
 * not specific to one instruction type. Uses partitions from all 4 types*/
//TODO: rename the properties
typedef struct {
    instruction_type type;
    uint8_t cond;
    uint8_t i;
    uint8_t p;
    uint8_t u;
    uint8_t opcode;
    uint8_t s;
    uint8_t l;
    int32_t rn;
    uint8_t rd;
    uint8_t rs;
    uint8_t rm;
    uint8_t a;
    uint32_t operand2;
    uint32_t offset;
} decoded_instruction;

/*contains the binary value of the instruction that has just been fetched*/
typedef struct {
    uint32_t binary_value;
    char *binary;
} fetched_instruction;

/*contains the state of the memory, registers and the current decoded and fetched instructions*/
typedef struct {
    uint8_t memory[NUM_ADDRESSES];
    int32_t registers[NUM_REGISTERS];
    fetched_instruction fetched_instruction;
    decoded_instruction decoded_instruction;
    //needed for sdt
    int address;
} current_state;




#endif //ARM11_11_DEFS_H
