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

uint32_t set_n_bits(uint32_t, int, int);

uint8_t mask_1_bit(int, int);

int rol(uint32_t);

void toBinaryString(uint32_t, char *);

uintptr_t get_address(char *, symbol_table)

bool is8bit(int);

bool isArgument(char);

void get_argument(char *, int, char *);

char *second_pass(char **code, tokenised_line tokenised_line, symbol_table symbol_table);

char *two_pass_assembly(char **code, int line_num);

int tokenizer(char *);


#endif //ARM11_11_UTILS_H