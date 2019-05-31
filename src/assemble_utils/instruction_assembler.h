//
// Created by taariq on 5/28/19.
//

#ifndef ARM11_11_INSTRUCTION_ASSEMBLE_H
#define ARM11_11_INSTRUCTION_ASSEMBLE_H


#include <stdint.h>
#include "defs.h"

void assemble_dpi_to(tokenised_line *, int , char* );

uint32_t assemble_sdt(char *, char **, int);

uint32_t assemble_mul(tokenised_line *, int);

char *assemble_branch(tokenised_line *, char **, int, symbol_table *);

char *assemble_special(tokenised_line *, int);

#endif //ARM11_11_INSTRUCTION_ASSEMBLE_H
