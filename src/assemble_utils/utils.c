//
// Created by taariq on 5/25/19.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "defs.h"


void two_pass_assembly() {

    //First pass
    symbol_table *symbol_table = malloc(sizeof(mapping) * LINES);

    /* Second Pass */

}

int tokenizer(char *line, tokenised_line tokenised_line) {

    char *line_t = malloc(sizeof(char) * LINE_LENGTH);
    strcpy(line_t, line);


    //Get label (if its just a label) (e.g. wait: this gets wait)
    if (strchr(line, ':') != NULL) {
        *tokenised_line.label = strtok_r(line_t, ":", &line_t);
        return 0;
    }

    //Get Opcode (e.g mov r1, r2 - this gets mov"
    *tokenised_line.opcode = strtok_r(line_t, " ", &line_t);

    //Split into operands (e.g. mov r1,r2 - this gets r1 etc
    int num_of_operands = 0;
    char *operand;
    while (operand = strtok_r(line_t, ",", &line_t)) {
        (*tokenised_line.operands)[num_of_operands] = operand;
        num_of_operands++;
    }

    return num_of_operands;

}
