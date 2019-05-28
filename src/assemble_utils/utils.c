//
// Created by Satadru on 5/25/19.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defs.h"

void binary_file_writer(char *filename, const char *binary_string) {
    FILE *binary_file = fopen(filename, "wb");
    if (binary_file == NULL) {
        printf("Error opening file");
    }
    fwrite(filename, 1, sizeof(binary_string), binary_file);
    fclose(binary_file);
}

void extract_2_char_cond(char *string, char result[3]) {
    if (string[1] != ' ') {
        result[0] = string[1];
        result[1] = string[2];
        result[2] = '/0';
    } else {
        result[0] = ' ';
        result[1] = ' ';
        result[2] = '/0';
    }
}

//gets the specified argument from the instruction
//e.g: get_argument("mul r1, r2, r3", 2) = "r2";
char* get_argument(char *instruction, int argument_number){
    int i = 0;
    int startpos = 0;
    int endpos =0;
    while (i < argument_number) {
        if (instruction[startpos] == ' '){
            i++;
        }
        startpos++;
    }
    if (instruction[startpos] == ','){
        startpos++;
    }
    return "";
}

/*sets as many bits as necessary to encompass value
 e.g (if value is <2, only 1 bit is set, if 3 < value < 8 then 3 bits are set

 assumption that bits being set start as 0, as do all bits less significant
 than end bit. */
uint32_t set_n_bits(uint32_t binary, int end_bit, int value) {
    binary |= (value << end_bit);
    return binary;
}

void add_to_mappings(symbol_table* symbol_table, mapping mapping){
    int num_elements = symbol_table->num_elements;
    symbol_table->num_elements = symbol_table->num_elements + 1;
    symbol_table->mappings[num_elements] = mapping;
}



    //First pass assoociates labels with memory addresses.
    first_pass(code, tokenised_line, symbol_table);

    /* Second Pass */
    char *binary = second_pass(code, tokenised_line, symbol_table);

    // REMEMBER TO free variables

    return binary;

    //Get Opcode (e.g mov r1, r2 - this gets mov"
    *tokenised_line.opcode = strtok_r(line_t, " ", &line_t);

void add_to_mappings(symbol_table *symbol_table, mapping mapping) {
    int num_elements = symbol_table->num_elements;
    symbol_table->num_elements = symbol_table->num_elements + 1;
    symbol_table->mappings[num_elements] = mapping;
}

void first_pass(char **code, tokenised_line tokenised_line, symbol_table symbol_table) {
    char *line;
    for (int line_num = 0; line_num < LINES; line_num++) {
        line = code[line_num];
        int operand_num = tokenizer(line, tokenised_line);
        if (operand_num == 0) {
            printf("label: %s\n", tokenised_line.label);
            mapping mapping = {
                    .label = tokenised_line.label,
                    .memory_address = &code[line_num + 1]
            };
            add_to_mappings(&symbol_table, mapping);
        }
    }
}

char *second_pass(char **code, tokenised_line tokenised_line, symbol_table symbol_table) {

    char *binary = (char *) malloc(INSTRUCTION_SIZE * LINES);
    binary[0] = '\0';
    //Read opcode mnemonics + operands for each instruction
    for (int i = 0; i < LINES; ++i) {
        int num_of_operands = tokenizer(code[i], tokenised_line);

        //exclude labels
        if (num_of_operands != 0) {
            for (int j = 0; j < num_of_operands; ++j) {
                uintptr_t address;

            }
        }
    }

    return binary;

}

char *two_pass_assembly(char **code, int line_num) {

    symbol_table symbol_table = {
            .num_elements = 0,
            .mappings = malloc(sizeof(mapping) * LINES)
    };

    tokenised_line tokenised_line = {
            .label = malloc(sizeof(char) * LINE_LENGTH),
            .opcode = malloc(sizeof(char) * OPCODE_LENGTH),
            .operands = malloc(sizeof(char *) * LINE_LENGTH / OPERAND_LENGTH)
    };

    //First pass assoociates labels with memory addresses.
    first_pass(code, line_num, tokenised_line, symbol_table);

    /* Second Pass */

}