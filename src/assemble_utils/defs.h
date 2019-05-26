//
// Created by taariq on 5/25/19.
//

#ifndef ARM11_11_DEFS_H
#define ARM11_11_DEFS_H


#include <stdint-gcc.h>

typedef struct {
    char *label;
    uintptr_t memory_address;
} mapping;

typedef struct {
    mapping mapping;
} symbol_table;


#endif //ARM11_11_DEFS_H
