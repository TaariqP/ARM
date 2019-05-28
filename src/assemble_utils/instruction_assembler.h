//
// Created by taariq on 5/28/19.
//

#ifndef ARM11_11_INSTRUCTION_ASSEMBLE_H
#define ARM11_11_INSTRUCTION_ASSEMBLE_H


#include <stdint.h>
#include "defs.h"



uint32_t assemble_dpi(char*, char **, int, symbol_table *);

uint32_t assemble_sdt(char*, char **, int, symbol_table);

uint32_t assemble_mul(char*, char **, int, symbol_table);

uint32_t assemble_branch(char*, char **, int, symbol_table);
#endif //ARM11_11_INSTRUCTION_ASSEMBLE_H
