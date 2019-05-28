//
// Created by ss13718 on 27/05/19.
//

#ifndef ARM11_11_INSTRUCTION_ASSEMBLER_H
#define ARM11_11_INSTRUCTION_ASSEMBLER_H

uint32_t assemble_dpi(char*, uint32_t*, char **, int, symbol_table *);

uint32_t assemble_sdt(char*, uint32_t*, char **, int, symbol_table *);

uint32_t assemble_mul(char*, uint32_t*, char **, int, symbol_table *);

uint32_t assemble_branch(char*, uint32_t*, char **, int, symbol_table *);

#endif //ARM11_11_INSTRUCTION_ASSEMBLER_H
