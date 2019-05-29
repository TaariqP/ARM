//
// Created by taariq on 5/28/19.
//

#ifndef ARM11_11_INSTRUCTION_ASSEMBLE_H
#define ARM11_11_INSTRUCTION_ASSEMBLE_H


#include <stdint.h>
#include "defs.h"

char *assemble_dpi(tokenised_line*, int);

uint32_t assemble_sdt(tokenised_line *, char **, int, symbol_table);

uint32_t assemble_mul(tokenised_line*, int);

char *assemble_branch(tokenised_line, char **, int, symbol_table);
#endif //ARM11_11_INSTRUCTION_ASSEMBLE_H
