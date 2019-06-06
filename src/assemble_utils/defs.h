//
// Created by taariq on 5/25/19.
//

#ifndef ARM11_11_DEFS_H
#define ARM11_11_DEFS_H


#include <stdint-gcc.h>

#define LINES 50
#define LINE_LENGTH 100
#define OPCODE_LENGTH 5
#define OPERAND_LENGTH 20
#define MAX_OPERANDS 5
#define INSTRUCTION_SIZE 32
#define NUMBER_OF_DPI 10
#define NUMBER_OF_MUL 2
#define NUMBER_OF_SDT 2
#define NUMBER_OF_BRANCH 7
#define NUMBER_OF_SPECIAL 2
#define COND_END_BIT 28
#define DPI_OPCODE_END_BIT 21
#define COMMAND_LENGTH 128

/*required for DPI assemble*/
typedef enum {
    compute_result,
    single_operand,
    set_CPSR
}DPI_TYPE;

/*contains a string label, and its memory address*/
typedef struct {
    char *label;
    int memory_address;
} mapping;

/* contains all mappings (mapping defined above)*/
typedef struct {
    int num_elements;
    mapping* mappings;
} symbol_table;

/*contains all tokenized lines
 * specific line properties can be accessed by
 * e.g: tokenized_line->opcode[line]*/
typedef struct {
    //Array of strings
    int num_of_lines;
    char** label;
    char** opcode;
    //array of ints
    int *num_of_operands;
    //Array of array of strings
    char*** operands;
} tokenised_line;


#endif //ARM11_11_DEFS_H
