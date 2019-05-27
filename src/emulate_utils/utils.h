//
// Created by taariq on 5/22/19.
//

#ifndef ARM11_11_UTILS_H
#define ARM11_11_UTILS_H

#include <stdbool.h>
#include "defs.h"

uint8_t mask_1_bit(int, int);

uint8_t mask_4_bit(int, int);

uint8_t mask_8_bit(int, int);

void binary_file_loader(char *, char *);

void print_binary(uint8_t *);

void print_registers(int32_t*);

bool check_condition(current_state*);

int get_file_size(char*);

uint32_t get_instruct(current_state *, int);

void pc_increment(current_state *);

void set_register(current_state *, int, int);

void set_CPSR_bit(current_state *, int, int);

int32_t sign_extend_26_to_32(int32_t);

int ror(int32_t, uint32_t);

int lsr(int, int);

int asr(int, int);

int lsl(int, int);


#endif //ARM11_11_UTILS_H
