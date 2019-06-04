//
// Created by taariq on 5/28/19.
//

#ifndef ARM11_11_INSTRUCTION_ASSEMBLE_H
#define ARM11_11_INSTRUCTION_ASSEMBLE_H


#include <stdint.h>
#include "defs.h"

void assemble_dpi_to(tokenised_line *, int, char *);

void assemble_sdt_to(tokenised_line *, int, char *, int, int, int *,
                     int, char **);

void assemble_mul_to(tokenised_line *, int, char *);

void assemble_branch_to(tokenised_line *, char **, int, symbol_table *, char *, int);

void assemble_special_to(tokenised_line *, int, char *);

#endif //ARM11_11_INSTRUCTION_ASSEMBLE_H
