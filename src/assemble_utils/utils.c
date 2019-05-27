//
// Created by Satadru on 5/25/19.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defs.h"

void binary_file_writer(char *filename, const char *binary_string){
    FILE *binary_file = fopen(filename, "wb");
    if (binary_file == NULL) {
        printf("Error opening file");
    }
    fwrite(filename, 1, sizeof(binary_string),binary_file);
    fclose(binary_file);
}

void extract_2_char_cond(char *string, char result[3]){
    if (string[1] != ' ') {
        result[0] = string[1];
        result[1] = string[2];
        result[2] = '/0';
    } else {
        result[0] = ' ';
        result[1] = ' ';
        result[2] = '/0';
    }
}

/*sets as many bits as necessary to encompass value
 e.g (if value is <2, only 1 bit is set, if 3 < value < 8 then 3 bits are set

 assumption that bits being set start as 0, as do all bits less significant
 than end bit. */
uint32_t set_n_bits(uint32_t binary, int end_bit, int value){
    binary |= (value << end_bit);
    return binary;
}

char* two_pass_assembly(){

    symbol_table symbol_table = {
            .mappings = malloc(sizeof(mapping) * LINES)
    };

    tokenised_line tokenised_line = {
            .label = malloc(sizeof(char) * LINE_LENGTH),
            .opcode = malloc(sizeof(char) * OPCODE_LENGTH),
            .operands = malloc(sizeof(char *) * LINE_LENGTH / OPERAND_LENGTH)
    };

    //First pass



    /* Second Pass */


}

void first_pass(){
    for (int i = 0; i < LINE_LENGTH; ++i) {
    }
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
