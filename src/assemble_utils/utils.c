//
// Created by Satadru on 5/25/19.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "defs.h"
#include "instruction_assembler.h"


char *DPI[] = {"add", "sub", "rsb", "and", "eor", "orr", "mov", "tst", "teq", "cmp"};
char *MUL[] = {"mul", "mla"};
char *SDT[] = {"ldr", "str"};
char *BRANCH[] = {"beq", "bne", "bge", "blt", "bgt", "ble", "b"};
char *SPECIAL[] = {"lsl", "andeq"};

void binary_file_writer(char *filename, const char *binary_string) {
    FILE *binary_file = fopen(filename, "wb+");
    if (binary_file) {
        //number of bytes (32 = 4 bytes)
        int no_of_instructions = (int) strlen(binary_string) / 32;
        int bytes[no_of_instructions];

        //32 bits + null terminator
        char final_instruction[33];

        for (int i = 0; i < no_of_instructions; ++i) {
            final_instruction[32] = '\0';
            strncpy(final_instruction, &binary_string[i * 32], 32);
            bytes[i] = (int) strtol(final_instruction, NULL, 2);
        }

        for (int j = 0; j < no_of_instructions; ++j) {
            char bin[100];
            toBinaryString((uint32_t) bytes[j], bin);
            printf("%s", bin);
        }


        fwrite(&bytes, sizeof(bytes), 1, binary_file);
        fclose(binary_file);
    } else {
        printf("could not write to binary file");
    }

}

//void extract_2_char_cond(char *string, char result[3]) {
//    if (string[1] != ' ') {
//        result[0] = string[1];
//        result[1] = string[2];
//        result[2] = '\0';
//    } else {
//        result[0] = ' ';
//        result[1] = ' ';
//        result[2] = '\0';
//    }
//}

char *trim_whitespace(char *str) {
    while (isspace((unsigned char) *str)) str++;
    if (*str == 0)
        return str;
    return str;
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
void set_n_bits(uint32_t *binary_num, int end_bit, int value) {
    *binary_num |= (value << end_bit);
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
int get_address(char *label, symbol_table *symbol_table) {
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

int test_tokenizer(tokenised_line *tokenised_line) {
    printf("Tokenised_lines\n");
    printf("Number of lines: %d", tokenised_line->num_of_lines);
    printf("Opcodes: ");
    for (int i = 0; i < tokenised_line->num_of_lines; ++i) {
        printf("%s", tokenised_line->opcode[i]);
    }

    printf("operands");
//    for (int j = 0; j < tokenised_line->num_of_lines; ++j) {
//        for (int i = 0; i < tokenised_line->; ++i) {
//
//        }
//        printf("%s", tokenised_line->operands[])
//    }
}

int tokenizer(char *line, int line_num, tokenised_line *tokenised_line) {

    char *line_t = (char *) malloc(sizeof(char) * LINE_LENGTH);
    strcpy(line_t, line);


    //Get label (if its just a label) (e.g. wait: this gets wait)
    if (strchr(line_t, ':') != NULL) {
        //tokenised_line->label = strtok_r(line_t, ":", &line_t);
        //printf("label: %s\n", *tokenised_line->label);
        return 0;
    }

    //Get Opcode (e.g mov r1, r2 - this gets mov"
    char *opcode;
    //opcode = strtok(line_t, " ");
    opcode = strtok_r(line_t, " ", &line_t);
    (tokenised_line->opcode[line_num]) = opcode;
    printf("opcode: %s\n", tokenised_line->opcode[line_num]);

    //NEW TOKENISER



    //Split into operands (e.g. mov r1,r2 - this gets r1 etc
    int num_of_operands = 0;
    char *operand;
    if (strchr(line_t, ',') != NULL) {
        operand = strtok_r(line_t, ",", &line_t);
        while (operand) {
            (tokenised_line->operands)[line_num][num_of_operands] = operand;
            printf("%s\n", tokenised_line->operands[line_num][num_of_operands]);
            num_of_operands++;
            operand = strtok_r(line_t, ",", &line_t);
        }
    } else {
        (tokenised_line->operands)[line_num][num_of_operands] = line_t;
        num_of_operands++;
    }
    for (int i = 0; i < num_of_operands; ++i) {
        (tokenised_line->operands)[line_num][i] = trim_whitespace((tokenised_line->operands)[line_num][i]);
        printf("%s\n", tokenised_line->operands[line_num][i]);
    }

    //free(line_t);
    return num_of_operands;

}


void first_pass(char **code, tokenised_line *tokenised_line, symbol_table *symbol_table) {
    char *line;
    //Go through each line of code and get labels and add to symbol table.
    for (int line_num = 0; line_num < tokenised_line->num_of_lines; line_num++) {
        line = code[line_num];
        int operand_num = tokenizer(line, line_num, tokenised_line);
        if (operand_num == 0) {
//            printf("label: %s\n", tokenised_line->label);
//            mapping* mapping = malloc(sizeof(mapping));
//            mapping->label = tokenised_line->label;
//            mapping->memory_address = &line[line_num + 1];
//            add_to_mappings(symbol_table, mapping);
        }
    }
}

char *second_pass(char **code, tokenised_line *tokenised_line, symbol_table *symbol_table) {

    //TODO: VALGRIND ERROR
    char *binary = (char *) malloc(INSTRUCTION_SIZE * LINES);
    binary[0] = '\0';
    //Read opcode mnemonics + operands for each instruction
    for (int line_num = 0; line_num < tokenised_line->num_of_lines; ++line_num) {
        int num_of_operands = tokenizer(code[line_num], line_num, tokenised_line);

        //Labels
        if (num_of_operands == 0) {
            //TODO
        }

        //Instructions
        if (num_of_operands != 0) {
            //Get operands that are labels and use symbol table to get address
            for (int j = 0; j < num_of_operands; ++j) {
                int address;
//                if (is_in_symbol_table(tokenised_line->operands[j], symbol_table)) {
//                    address = get_address(tokenised_line->operands[j], symbol_table);
//                    sprintf(tokenised_line->operands[j], "%d", address);
//                }
            }

            //Calls to Instruction_assemble

            for (int k = 0; k < NUMBER_OF_DPI; ++k) {
                if (strcmp(tokenised_line->opcode[line_num], DPI[k]) == 0) {
                    char binaryToAdd[33];
                    assemble_dpi_to(tokenised_line, line_num, binaryToAdd);
                    strcat(binary, binaryToAdd);
                    printf("%s\n", binaryToAdd);
                    break;
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

    symbol_table *symbol_table = malloc(sizeof(*symbol_table));
    symbol_table->num_elements = 0;
//    symbol_table->mappings;

    tokenised_line *tokenised_line = malloc(sizeof(*tokenised_line) * num_of_lines);
    tokenised_line->num_of_lines = num_of_lines;
//    tokenised_line->label;


    //char** array of strings
    //Pointers are 8 bytes (64-bits) long
    // SIZE OF (char pointer) = 8 bytes
    //size of char = 1 byte
    //OPCODE_LENGTH = 3;
    //char** opcode should be (number of lines * opcode length)

    //TODO: VALGRIND ERROR
    tokenised_line->opcode = (char **) malloc(sizeof(char *) * OPCODE_LENGTH * (num_of_lines));
    for (int k = 0; k < num_of_lines; ++k) {
        //TODO: VALGRIND ERROR
        tokenised_line->opcode[k] = (char *) malloc(sizeof(char) * (OPCODE_LENGTH + 1));
    }

    //Char*** = array of array of strings
    //char** = array of strings
    //char* = string
    // MAX_operands = number of lines * (LINELENGTH / OPERAND_LENGTH)

    tokenised_line->operands = (char ***) malloc(sizeof(char **) * num_of_lines);
    for (int i = 0; i < num_of_lines; ++i) {
        // allocate each row (the row cells)
        tokenised_line->operands[i] = (char **) malloc(sizeof(char *) * MAX_OPERANDS);
        for (int j = 0; j < MAX_OPERANDS; ++j) {
            // initialize the row cell by allocating the string
            tokenised_line->operands[i][j] = (char *) malloc(sizeof(char) * (OPERAND_LENGTH + 1));/* allocate string */
        }
    }

//First pass assoociates labels with memory addresses.
    first_pass(code, tokenised_line, symbol_table);

/* Second Pass */

    char *binary = second_pass(code, tokenised_line, symbol_table);

// REMEMBER TO free


//    for (int i = 0; i < num_of_lines; ++i) {
//        for (int j = 0; j < MAX_OPERANDS; ++j) {
//            //This free is colliding with the malloc in tokeniser
//            free(tokenised_line->operands[i][j]);  // free the string
//        }
//        free(tokenised_line->operands[i]);         // free the row
//    }

    for (int l = 0; l < num_of_lines; ++l) {
        free(tokenised_line->opcode[l]);
    }
    free(tokenised_line->opcode);

    free(tokenised_line->operands);
    free(tokenised_line);
    free(symbol_table);

    return binary;
}
