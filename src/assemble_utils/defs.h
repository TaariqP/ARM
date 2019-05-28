//
// Created by taariq on 5/25/19.
//

#ifndef ARM11_11_DEFS_H
#define ARM11_11_DEFS_H


#include <stdint-gcc.h>

#define LINES 50
#define LINE_LENGTH 511
#define OPCODE_LENGTH 3
#define OPERAND_LENGTH 20
#define INSTRUCTION_SIZE 32
#define NUMBER_OF_DPI 10
#define NUMBER_OF_MUL 2
#define NUMBER_OF_SDT 2
#define NUMBER_OF_BRANCH 7
#define NUMBER_OF_SPECIAL 2
// the initial 0 binary that we set depending on the instruction
#define binary 0

//general purpose code for reduced duplication
#define cond_end_bit 28
#define cond[3]


char* DPI[] = {"add","sub","rsb","and","eor","orr","mov","tst","teq","cmp"};
char* MUL[] = {"mul","mla"};
char* SDT[] = {"ldr","str"};
char* BRANCH[] = {"beq","bne","bge","blt","bgt","ble","b"};
char* SPECIAL[] = {"lsl","andeq"};

typedef struct {
    char *label;
    uintptr_t memory_address;
} mapping;

typedef struct {
    int num_elements;
   mapping* mappings;
} symbol_table;

typedef struct {
    //Array of strings
    char** label;
    char** opcode;
    //Array of array of strings
    char*** operands;
} tokenised_line;


#endif //ARM11_11_DEFS_H
