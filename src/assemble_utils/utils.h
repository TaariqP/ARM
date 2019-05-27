//
// Created by Satadru on 5/25/19.
//


#ifndef ARM11_11_UTILS_H
#define ARM11_11_UTILS_H

#include <stdint.h>

void binary_file_writer(char*, const char*);

void extract_2_char_cond(char*, char*);

uint32_t set_n_bits(uint32_t, int , int);

void two_pass_assembly();

void tokenizer(char *);


#endif //ARM11_11_UTILS_H