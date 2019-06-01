//
// Created by Satadru on 5/25/19.
//


#ifndef ARM11_11_UTILS_H
#define ARM11_11_UTILS_H

#include <stdint.h>
#include "defs.h"
#include <stdbool.h>

void binary_file_writer(char *, const char *);

void extract_2_char_cond(char *, char *);

char *trim_whitespace(char *);


uint8_t mask_1_bit_assemble(int, int);

int rol(uint32_t);

bool is8bit(int);

bool is24bit(int);

bool isArgument(char);

void set_operand(uint32_t *, int, int, int, tokenised_line *);

void get_argument(char *, int, char *);

void set_n_bits(uint32_t *, int, int);

int is_empty(const char *);


void toBinaryString(uint32_t binary, char *result);

int is_in_symbol_table(char *label, symbol_table *symbol_table);

int get_address(char *, symbol_table *);

void add_to_mappings(symbol_table *symbol_table, mapping mapping);

int tokenizer(char *line, int, tokenised_line *tokenised_line);

void first_pass(char **code, tokenised_line *tokenised_line, symbol_table *symbol_table);

char *second_pass(char **code, tokenised_line *tokenised_line, symbol_table *symbol_table);

char *two_pass_assembly(char **code, int num_of_lines);


#endif //ARM11_11_UTILS_H