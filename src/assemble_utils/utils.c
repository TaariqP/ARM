//
// Created by Satadru on 5/25/19.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "defs.h"
#include "instruction_assembler.h"



char *DPI[] = {"add", "sub", "rsb", "and", "eor", "orr", "mov", "tst", "teq", "cmp"};
char *MUL[] = {"mul", "mla"};
char *SDT[] = {"ldr", "str"};
char *BRANCH[] = {"beq", "bne", "bge", "blt", "bgt", "ble", "b"};
char *SPECIAL[] = {"lsl", "andeq"};

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
        result[2] = '\0';
    } else {
        result[0] = ' ';
        result[1] = ' ';
        result[2] = '\0';
    }
}

uint8_t mask_1_bit_assemble(int value, int bit) {
    return (value >> bit) & 0x1;
}

//rotate left once
int rol(uint32_t val) {
    int msb = mask_1_bit_assemble(val, 31);
    int temp = val << 1;
    if (msb) {
        temp |= msb;
    } else {
        temp &= msb;
    }
    return temp;
}

bool is8bit(int val) {
    return (val == (val & 0xFF));
}

bool is24bit(int val) {
    return (val == (val & 0xFFFFFF));
}

bool isArgument(char c) {
    if (c == ',') {
        return false;
    }
    if (c == ' ') {
        return false;
    }
    if (c == '\0') {
        return false;
    }
    return true;
}

//gets the specified argument from the instruction
//e.g: get_argument("mul r1, r2, r3", 2) = "r2";
void get_argument(char *instruction, int argument_number, char *result) {
    int i = 0;
    int startpos = 0;
    int endpos = 0;
    while (i < argument_number) {
        if (instruction[startpos] == ' ') {
            i++;
        }
        startpos++;
    }
    endpos = startpos;
    while (isArgument(instruction[endpos])) {
        endpos++;
    }
    int length = endpos - startpos + 1;
    result = (char *) realloc(result, sizeof(char) * length);
    for (i = 0; i < length - 1; i++) {
        result[i] = instruction[i + startpos];
    }
    result[length - 1] = '\0';
}

/*sets as many bits as necessary to encompass value
 e.g (if value is <2, only 1 bit is set, if 3 < value < 8 then 3 bits are set

 assumption that bits being set start as 0, as do all bits less significant
 than end bit. */
uint32_t set_n_bits(uint32_t binary_num, int end_bit, int value) {
    binary_num |= (value << end_bit);
    return binary_num;
}

//converts a 32bit integer to a binary string
void toBinaryString(uint32_t binary, char *result) {
    for (int i = 0; i < 32; i++) {
        result[i] = (char) ((mask_1_bit_assemble(binary, 31 - i) + '0'));
    }
    result[32] = '\0';
}

//Check if label exists in symbol table

int is_in_symbol_table(char *label, symbol_table *symbol_table) {
    for (int i = 0; i < symbol_table->num_elements; ++i) {
        if (strcmp(symbol_table->mappings[i].label, label) == 0) {
            return 1;
        }
    }
    return 0;
}

//gets the address of a label
uintptr_t get_address(char *label, symbol_table *symbol_table) {
    //compare given label to the label of each mapping till found.
    int mapping_number = 0;
    for (; mapping_number < symbol_table->num_elements; mapping_number++) {
        if (!(strcmp(symbol_table->mappings[mapping_number].label, label))) {
            //label found
            break;
        }
    }
    return symbol_table->mappings[mapping_number].memory_address;
}

void add_to_mappings(symbol_table *symbol_table, mapping mapping) {
    //Increment number of elements in symbol table
    int num_elements = symbol_table->num_elements;
    symbol_table->num_elements = symbol_table->num_elements + 1;
    //Add mapping to symbol table
    symbol_table->mappings[num_elements] = mapping;
}

int tokenizer(char *line, tokenised_line* tokenised_line) {

    char *line_t = malloc(sizeof(char) * LINE_LENGTH);
    strcpy(line_t, line);


    //Get label (if its just a label) (e.g. wait: this gets wait)
    if (strchr(line, ':') != NULL) {
        tokenised_line->label = strtok_r(line_t, ":", &line_t);
        printf("label: %s\n", *tokenised_line->label);
        return 0;
    }

    //Get Opcode (e.g mov r1, r2 - this gets mov"
    *tokenised_line->opcode = strtok_r(line_t, " ", &line_t);
    printf("opcode: %s\n", *tokenised_line->opcode);

    //Split into operands (e.g. mov r1,r2 - this gets r1 etc
    int num_of_operands = 0;
    char *operand;
    while (operand = strtok_r(line_t, ",", &line_t)) {
        (tokenised_line->operands)[num_of_operands] = operand;
        printf("%s\n", operand);
        num_of_operands++;
    }

    return num_of_operands;

}


void first_pass(char **code, tokenised_line *tokenised_line, symbol_table *symbol_table) {
    char *line;
    //Go through each line of code and get labels and add to symbol table.
    for (int line_num = 0; line_num < tokenised_line->num_of_lines; line_num++) {
        line = code[line_num];
        int operand_num = tokenizer(line, tokenised_line);
        if (operand_num == 0) {
            printf("label: %s\n", tokenised_line->label);
            mapping mapping = {
                    .label = tokenised_line->label,
                    .memory_address = &line[line_num + 1]
            };
            add_to_mappings(symbol_table, mapping);
        }
    }
}

char *second_pass(char **code, tokenised_line *tokenised_line, symbol_table *symbol_table) {

    char *binary = (char *) malloc(INSTRUCTION_SIZE * LINES);
    binary[0] = '\0';
    //Read opcode mnemonics + operands for each instruction
    for (int line_num = 0; line_num < LINES; ++line_num) {
        int num_of_operands = tokenizer(code[line_num], tokenised_line);

        //Labels
        if (num_of_operands == 0) {
            //TODO
        }

        //Instructions
        if (num_of_operands != 0) {
            //Get operands that are labels and use symbol table to get address
            for (int j = 0; j < num_of_operands; ++j) {
                uintptr_t address;
                if (is_in_symbol_table(tokenised_line->operands[j], symbol_table)) {
                    //not sure how to cast this
                    // tokenised_line->operands[j] = get_address(tokenised_line->operands[j], symbol_table);
                }
            }

            //Calls to Instruction_assemble

            for (int k = 0; k < NUMBER_OF_DPI; ++k) {
                if (strcmp(*tokenised_line->opcode, DPI[k]) == 0) {
                    strcat(binary, assemble_dpi(tokenised_line, line_num));
                    printf("adding binary");
                }
            }

            for (int k = 0; k < NUMBER_OF_SDT; ++k) {
                if (strcmp(*tokenised_line->opcode, SDT[k]) == 0) {
                    //strcat(binary, assemble_sdt(tokenised_line, line_num));
                }
            }
            for (int k = 0; k < NUMBER_OF_MUL; ++k) {
                if (strcmp(*tokenised_line->opcode, MUL[k]) == 0) {
                    //strcat(binary, assemble_mul(tokenised_line, line_num, *symbol_table));
                }
            }
            for (int k = 0; k < NUMBER_OF_BRANCH; ++k) {
                if (strcmp(*tokenised_line->opcode, BRANCH[k]) == 0) {
                    //strcat(binary, assemble_branch(tokenised_line, line_num, symbol_table));
                }
            }
            for (int k = 0; k < NUMBER_OF_SPECIAL; ++k) {
                if (strcmp(*tokenised_line->opcode, SPECIAL[k]) == 0) {
                }
            }
        }
    }
    return binary;
}

char *two_pass_assembly(char **code, int num_of_lines) {

    symbol_table *symbol_table = malloc((sizeof(symbol_table)));
    symbol_table->num_elements = 0;
    symbol_table->mappings = malloc(sizeof(mapping) * LINES);

    tokenised_line *tokenised_line = malloc(sizeof(tokenised_line));
    tokenised_line->num_of_lines = num_of_lines;
    tokenised_line->label = malloc(sizeof(char) * LINE_LENGTH);
    tokenised_line->opcode = malloc(sizeof(char) * OPCODE_LENGTH);
    tokenised_line->operands = malloc(sizeof(char *) * LINE_LENGTH / OPERAND_LENGTH);

//First pass assoociates labels with memory addresses.
    first_pass(code, tokenised_line, symbol_table);

/* Second Pass */
    char *binary = second_pass(code, tokenised_line, symbol_table);

// REMEMBER TO free variables

    return binary;

}
