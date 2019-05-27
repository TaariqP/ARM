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
#include "utils.c"

//binary must start as all 0s
void assemble_branch(char *string, uint32_t *binary){
    char cond[3];
    extract_branch_cond(string, cond);
    int binary = 0;
    int cond_end_bit = 28;

    //setting cond bits
    switch (cond) {
        case "eq" :
            //BEQ
            binary = set_4_bits(binary, cond_end_bit, 0);
            break;

        case "ne":
            //BNE
            binary = set_4_bits(binary, cond_end_bit, 1);
            break;

        case "ge":
            //BGE
            binary = set_4_bits(binary, cond_end_bit, 10);
            break;

        case "lt":
            //BLT
            binary = set_4_bits(binary, cond_end_bit, 11);
            break;

        case "gt":
            //BGT
            binary = set_4_bits(binary, cond_end_bit, 12);
            break;

        case "le":
            //BLE
            binary = set_4_bits(binary, cond_end_bit, 13);
            break;

        case "al":
            //UNCONDITIONAL BRANCH
            binary = set_4_bits(binary, cond_end_bit, 14);
            break;

        case "  ":
            //B (NO SUFFIX)
            binary = set_4_bits(binary, cond_end_bit, 14);
            break;

        default:
            //NOT A VALID INSTRUCTION
            fprintf(stderr, "not a valid branch instruction");
    }

    //set bits 27-24 to be 1010
    binary = set_4_bits(binary, 24, 10);


}