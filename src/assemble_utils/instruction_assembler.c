//
// Created by ss13718 on 27/05/19.
//

#include <bits/types/FILE.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "defs.h"
#include "utils.h"
#include "instruction_assembler.h"


// the initial 0 binary that we set depending on the instruction
uint32_t binary = 0;

//general purpose code for reduced duplication
int cond_end_bit = 28;
char cond[3];



uint32_t assemble_dpi(char *string, char **code, int line, symbol_table *symbol_table){
    extract_2_char_cond(string, cond);

    //set cond to 1110
    binary = set_n_bits(binary, 28, 14);
}


uint32_t assemble_mul(char *string, char **code, int line, symbol_table *symbol_table){
    extract_2_char_cond(string, cond);

    //set cond to 1110
    binary = set_n_bits(binary, cond_end_bit, 14);

    //bits 27 to 22 are already 0

    //set A only if mla
    if (cond == "la") {
        set_n_bits(binary, 21, 1);
    }

    //TODO: set Rd

    //TODO: set Rn

    //TODO: set Rs

    //set bits 7-4 to be 1001
    binary = set_n_bits(binary, 4, 9);

    //TODO: set Rm


    return binary;

}

uint32_t assemble_sdt(char *string, char **code, int line, symbol_table *symbol_table){

}

 uint32_t assemble_branch(char *string, char **code, int line, symbol_table *symbol_table){
     extract_2_char_cond(string, cond);

    //setting cond bits
    switch (cond) {
        case "eq" :
            //BEQ
            binary = set_n_bits(binary, cond_end_bit, 0);
            break;

        case "ne":
            //BNE
            binary = set_n_bits(binary, cond_end_bit, 1);
            break;

        case "ge":
            //BGE
            binary = set_n_bits(binary, cond_end_bit, 10);
            break;

        case "lt":
            //BLT
            binary = set_n_bits(binary, cond_end_bit, 11);
            break;

        case "gt":
            //BGT
            binary = set_n_bits(binary, cond_end_bit, 12);
            break;

        case "le":
            //BLE
            binary = set_n_bits(binary, cond_end_bit, 13);
            break;

        case "al":
            //UNCONDITIONAL BRANCH
            binary = set_n_bits(binary, cond_end_bit, 14);
            break;

        case "  ":
            //B (NO SUFFIX)
            binary = set_n_bits(binary, cond_end_bit, 14);
            break;

        default:
            //NOT A VALID INSTRUCTION
            fprintf(stderr, "not a valid branch instruction");
    }

    //set bits 27-24 to be 1010
    binary = set_n_bits(binary, 24, 10);


    //TODO: set offset
    uint32_t current_address = &code[line];
        //TODO 1: get the exact label from the instruction
        //TODO 2: extract that exact mapping from the symbol table
        //TODO 3: find the address for that label from mapping
        //TODO 4: calculate the offset (destination - current + 8)?

    //label =
    //uint32_t destination_address =



    return binary;
}