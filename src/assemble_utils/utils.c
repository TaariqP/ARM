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

/*the below contain all possible instructions, and their types for use in the second pass*/
const char *DPI[] = {"add", "sub", "rsb", "and", "eor", "orr", "mov", "tst", "teq", "cmp"};
const char *MUL[] = {"mul", "mla"};
const char *SDT[] = {"ldr", "str"};
const char *BRANCH[] = {"beq", "bne", "bge", "blt", "bgt", "ble", "b"};
const char *SPECIAL[] = {"lsl", "andeq"};

//returns the value of the exact bit being requested (0 indexed)
uint8_t mask_1_bit_assemble(int value, int bit) {
  return (uint8_t) ((value >> bit) & 0x1);
}

//converts a 32bit integer to a binary string
void toBinaryString(int binary, char *result) {
  for (int i = 0; i < 32; i++) {
    result[i] = (char) ((mask_1_bit_assemble(binary, 31 - i) + '0'));
  }
  result[32] = '\0';
}

//identifies and sets base of a number represented as a string
void set_base(char *expression, int *base) {
  if (strstr(expression, "0x")) {
    //Base 16
    *base = 16;
  } else {
    //Base 10
    *base = 10;
  }
}

//writes binary string to an output file
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
    }

    fwrite(&bytes, sizeof(bytes), 1, binary_file);
    fclose(binary_file);
  } else {
    perror("could not write to binary file");
  }
}

char *trim_whitespace(char *str) {
  while (isspace((unsigned char) *str)) str++;
  if (*str == 0)
    return str;
  return str;
}

//rotate left once
int rol(uint32_t val) {
  int msb = mask_1_bit_assemble(val, 31);
  int temp = val << 1;
  if (msb) {
    temp |= msb;
  } else {
    temp &= 0xFFFFFFFE;
  }
  return temp;
}

//returns true if the integer can be represented in 8 bits
bool is8bit(int val) {
  return (val == (val & 0xFF));
}

//returns true if the integer can be represented in 26 bits
bool is26bit(int val) {
  return (val == (val & 0x3FFFFFF));
}

//helper to indetify whether a certain charatcer makes up part of an argument
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

/* Second Pass */
void set_n_bits(uint32_t *binary_num, int end_bit, int value) {
  *binary_num |= (value << end_bit);
}

/*the following functions aims to reduce duplication in assemble_mul and assemble_dpi
 * it sets the binary number at the specified bit to hold a specific register value*/
void set_operand(uint32_t *binary, int line, int arg_num, int end_bit, tokenised_line *tokenised_line) {
  char *reg = tokenised_line->operands[line][arg_num];
  reg += sizeof(char);
  int reg_num = (int) strtol(reg, (char **) NULL, 10);
  set_n_bits(binary, end_bit, reg_num);
}

void
set_sdt_bits(uint32_t *binary, int set_I, int set_P, int set_U, int set_L, int set_rn, int set_rd, int set_offset) {
  //assume always executed, set cond
  set_n_bits(binary, COND_END_BIT, 14);

  set_n_bits(binary, 26, 1);

  //setI
  set_n_bits(binary, 25, set_I);

  //setP
  set_n_bits(binary, 24, set_P);

  //setU
  set_n_bits(binary, 23, set_U);

  //setL
  set_n_bits(binary, 20, set_L);

  //setrn
  set_n_bits(binary, 16, set_rn);

  //setrd
  set_n_bits(binary, 12, set_rd);

  //setoffset
  set_n_bits(binary, 0, set_offset);
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
  for (int i = 0; i < symbol_table->num_elements; ++i) {
    if (label == NULL) {
      continue;
    }
    if (strcmp(symbol_table->mappings[i].label, label) == 0) {
      return symbol_table->mappings[i].memory_address;
    }
  }
  return -1;
}

/*used to fill in the symbol table*/
void add_to_mappings(symbol_table *symbol_table, mapping mapping) {
  //Increment number of elements in symbol table
  int num_elements = symbol_table->num_elements;
  symbol_table->num_elements = symbol_table->num_elements + 1;
  //Add mapping to symbol table
  symbol_table->mappings[num_elements] = mapping;
}

/*checks if a string contains any non whitespace character*/
int is_empty(const char *string) {
  while (*string != '\0') {
    if (!isspace(*string))
      return 0;
    string++;
  }
  return 1;
}

/*fills in the tokenized line struct provided */
int tokenizer(char *line, int line_num, tokenised_line *tokenised_line) {
  char *line_t = (char *) malloc(sizeof(char) * LINE_LENGTH);
  strcpy(line_t, line);

  char *line_t_alt = line_t;

  //Get label (if its just a label) (e.g. wait: this gets wait)
  if (strchr(line_t, ':') != NULL) {
    char *label_from_line_t = strtok_r(line_t, ":", &line_t);
    char label[strlen(label_from_line_t) + 1];
    strcpy(label, label_from_line_t);
    strcpy(tokenised_line->label[line_num], label);
    free(line_t_alt);
    return 0;
  }

  //Get Opcode (e.g mov r1, r2 - this gets mov"
  char *opcode_from_line_t;
  opcode_from_line_t = strtok_r(line_t, " ", &line_t);
  char opcode[strlen(opcode_from_line_t) +1];
  strcpy(opcode, opcode_from_line_t);
  strcpy(tokenised_line->opcode[line_num], opcode);

  //Split into operands (e.g. mov r1,r2 - this gets r1 etc
  int num_of_operands = 0;
  char *operand_from_line_t;
  if (strchr(line_t, ',') != NULL) {
    operand_from_line_t = strtok_r(line_t, ",", &line_t);
    while (operand_from_line_t) {
      char operand[strlen(opcode_from_line_t) +1];
      strcpy(operand, operand_from_line_t);
      strcpy(tokenised_line->operands[line_num][num_of_operands], operand);

      num_of_operands++;
      operand_from_line_t = strtok_r(line_t, ",", &line_t);
    }
  } else {
    char line_t_string[strlen(line_t) +1];
    strcpy(line_t_string, line_t);
    strcpy(tokenised_line->operands[line_num][num_of_operands], line_t_string);
    num_of_operands++;
  }
  tokenised_line->num_of_operands[line_num] = num_of_operands;
  for (int i = 0; i < num_of_operands; ++i) {
    char operand_to_add[strlen(trim_whitespace((tokenised_line->operands)[line_num][i])) + 1];
    strcpy(operand_to_add, (trim_whitespace((tokenised_line->operands)[line_num][i])));
    strcpy(tokenised_line->operands[line_num][i], operand_to_add);
  }

  free(line_t_alt);
  return num_of_operands;

}

/*check if given string contains the char we are searching for*/
bool containsChar(char c, char *string) {
  for (int i = 0; i < strlen(string); i++) {
    if (string[i] == c) {
      return true;
    }
  }
  return false;
}


int first_pass(char **code, tokenised_line *tokenised_line, symbol_table *symbol_table) {
  char *line;
  int total_labels = 0;
  //Go through each line of code and get labels and add to symbol table.
  for (int line_num = 0; line_num < tokenised_line->num_of_lines; line_num++) {
    line = code[line_num];
    int operand_num = tokenizer(line, line_num, tokenised_line);
    if (operand_num == 0) {
      mapping *mapping = malloc(sizeof(mapping) * 500);
      mapping->label = tokenised_line->label[line_num];
      mapping->memory_address = 4 * line_num;
      add_to_mappings(symbol_table, *mapping);
      total_labels++;
      free(mapping);
    }
  }
  return total_labels;
}

char *second_pass(char **code, tokenised_line *tokenised_line, symbol_table *symbol_table, int total_labels) {

  //TODO: VALGRIND ERROR
  int offset = 0;
  char *byte_to_add = calloc(sizeof(char), 4 * LINES);

  char *binary = (char *) malloc(INSTRUCTION_SIZE * LINES);
  binary[0] = '\0';
  int num_of_labels = 0;
  //Read opcode mnemonics + operands for each instruction
  for (int line_num = 0; line_num < tokenised_line->num_of_lines; ++line_num) {
    int num_of_operands = tokenizer(code[line_num], line_num, tokenised_line);
    //Instructions
    if (num_of_operands != 0) {
      //Get operands that are labels and use symbol table to get address
      for (int j = 0; j < num_of_operands; ++j) {
        if (is_in_symbol_table(tokenised_line->operands[line_num][j], symbol_table)) {
          int address = get_address(tokenised_line->operands[line_num][j], symbol_table);
          sprintf(tokenised_line->operands[line_num][j], "%d", address);
          if ((line_num * 4) > address) {
            num_of_labels++;
          }
        }
      }

      //Calls to Instruction_assemble

      for (int k = 0; k < NUMBER_OF_DPI; ++k) {
        if (strcmp(tokenised_line->opcode[line_num], DPI[k]) == 0) {
          char binaryToAdd[33];
          assemble_dpi_to(tokenised_line, line_num, binaryToAdd);
          strcat(binary, binaryToAdd);
          break;
        }
      }

      for (int k = 0; k < NUMBER_OF_SDT; ++k) {
        int value = 0;
        if (strcmp(tokenised_line->opcode[line_num], SDT[k]) == 0) {
          char binaryToAdd[33];
          assemble_sdt_to(tokenised_line, line_num, binaryToAdd, line_num - num_of_labels,
                          tokenised_line->num_of_lines - total_labels, &value, offset, &byte_to_add);
          strcat(binary, binaryToAdd);
          if (value) {
            offset += 4;
            value = 0;
          }
          break;
        }
      }
      for (int k = 0; k < NUMBER_OF_MUL; ++k) {
        if (strcmp(tokenised_line->opcode[line_num], MUL[k]) == 0) {
          char binaryToAdd[33];
          assemble_mul_to(tokenised_line, line_num, binaryToAdd);
          strcat(binary, binaryToAdd);
          break;
        }
      }
      for (int k = 0; k < NUMBER_OF_BRANCH; ++k) {
        if (strcmp(tokenised_line->opcode[line_num], BRANCH[k]) == 0) {
          char binaryToAdd[33];
          assemble_branch_to(tokenised_line, code, line_num, symbol_table, binaryToAdd, num_of_labels);
          strcat(binary, binaryToAdd);
          break;
          //strcat(binary, assemble_branch(tokenised_line, line_num, symbol_table));
        }
      }
      for (int k = 0; k < NUMBER_OF_SPECIAL; ++k) {
        if (strcmp(tokenised_line->opcode[line_num], SPECIAL[k]) == 0) {
          char binaryToAdd[33];
          assemble_special_to(tokenised_line, line_num, binaryToAdd);
          strcat(binary, binaryToAdd);
          break;
        }
      }
    }
  }
  strcat(binary, byte_to_add);
  free(byte_to_add);
  return binary;
}


char *two_pass_assembly(char **code, int num_of_lines) {

  symbol_table *symbol_table = malloc(sizeof(*symbol_table));
  symbol_table->num_elements = 0;
  symbol_table->mappings = (mapping *) malloc(sizeof(mapping) * num_of_lines);

  tokenised_line *tokenised_line = malloc(sizeof(*tokenised_line) * num_of_lines);
  tokenised_line->num_of_lines = num_of_lines;
  tokenised_line->label = (char **) malloc(sizeof(char *) * num_of_lines);
  for (int k = 0; k < num_of_lines; ++k) {
    tokenised_line->label[k] = (char *) malloc(sizeof(char) * LINE_LENGTH);
  }

  tokenised_line->opcode = (char **) malloc(sizeof(char *) * OPCODE_LENGTH * (num_of_lines));
  for (int k = 0; k < num_of_lines; ++k) {
    tokenised_line->opcode[k] = (char *) malloc(sizeof(char) * (OPCODE_LENGTH + 1));
  }

  // MAX_operands = number of lines * (LINELENGTH / OPERAND_LENGTH)
  tokenised_line->operands = (char ***) malloc(sizeof(char **) * num_of_lines);
  tokenised_line->num_of_operands = (int *) malloc(sizeof(int) * num_of_lines);
  for (int i = 0; i < num_of_lines; ++i) {
    // allocate each row (the row cells)
    tokenised_line->operands[i] = (char **) calloc( MAX_OPERANDS, sizeof(char *));
    for (int j = 0; j < MAX_OPERANDS; ++j) {
      // initialize the row cell by allocating the string
      tokenised_line->operands[i][j] = (char *) calloc((OPERAND_LENGTH + 1), sizeof(char));/* allocate string */
    }
  }

//First pass assoociates labels with memory addresses.
  int total_labels = first_pass(code, tokenised_line, symbol_table);

/* Second Pass */

  char *binary = second_pass(code, tokenised_line, symbol_table, total_labels);

    for (int i = 0; i < num_of_lines; ++i) {
        for (int j = 0; j < MAX_OPERANDS; ++j) {
            free(tokenised_line->operands[i][j]);  // free the string
        }
        free(tokenised_line->operands[i]);         // free the row
    }

  for (int l = 0; l < num_of_lines; ++l) {
    free(tokenised_line->opcode[l]);
  }
  free(tokenised_line->opcode);
  free(tokenised_line->num_of_operands);

  for (int l = 0; l < num_of_lines; ++l) {
    free(tokenised_line->label[l]);
  }
  free(tokenised_line->label);
  free(tokenised_line->operands);
  free(tokenised_line);
  free(symbol_table->mappings);
  free(symbol_table);

  return binary;
}
