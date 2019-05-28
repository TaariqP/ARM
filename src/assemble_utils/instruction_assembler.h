//
// Created by ss13718 on 27/05/19.
//

#ifndef ARM11_11_INSTRUCTION_ASSEMBLER_H
#define ARM11_11_INSTRUCTION_ASSEMBLER_H

#include "defs.h"

// the initial 0 binary that we set depending on the instruction

//general purpose code for reduced duplication


uint32_t assemble_dpi(char*, char **, int, symbol_table *);

uint32_t assemble_sdt(char*, char **, int, symbol_table *);

uint32_t assemble_mul(char*, char **, int, symbol_table *);

uint32_t assemble_branch(char*, char **, int, symbol_table *);

#endif //ARM11_11_INSTRUCTION_ASSEMBLER_H
