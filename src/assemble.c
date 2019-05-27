#include <stdlib.h>
#include <stdio.h>

#include "assemble_utils/utils.c"

int main(int argc, char **argv) {

    if (argc != 2) {
        printf("Provide an argument!");
        return EXIT_FAILURE;
    }

    symbol_table symbol_table = {
            .mappings = malloc(sizeof(struct mapping) * LINES)
    };

    tokenised_line tokenised_line = {
            .label = malloc(sizeof(char) * LINE_LENGTH),
            .opcode = malloc(sizeof(char) * OPCODE_LENGTH),
            .operands = malloc(sizeof(char *) * LINE_LENGTH / OPERAND_LENGTH)
    };

    tokenizer("mov r1 r2", tokenised_line);
    for (int i = 0; i < tokenised_line.label; ++i) {

    }

    return EXIT_SUCCESS;
}
