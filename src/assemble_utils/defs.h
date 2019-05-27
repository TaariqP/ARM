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

typedef struct {
    char *label;
    uintptr_t memory_address;
} mapping;

typedef struct {
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
