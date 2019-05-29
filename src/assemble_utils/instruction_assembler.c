//
// Created by taariq on 5/28/19.
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <stdlib.h>
#include "utils.h"


// the initial 0 binary that we set depending on the instruction
uint32_t binary = 0;

//general purpose code for reduced duplication
char cond[3];

//TODO

//CHANGE ALL ASSEMBLE FUNCTIONS TO TAKE TOKENISED LINE rather than the whole string
//ALSO ADD assemble special instructions
uint32_t assemble_dpi(tokenised_line *tokenised_line, int line) {

    //set cond to 1110
    binary = set_n_bits(binary, COND_END_BIT, 14);

    //bits 27-26 always 0

    char *command = tokenised_line->opcode[line];
    DPI_TYPE type;


    //set opcode and type
    //needs if else ladder because of the way C compares strings :(
    if (!strcmp(command, "eor")){
        //EOR
        set_n_bits(binary, DPI_OPCODE_END_BIT, 1);
        type = compute_result;
    } else if (!(strcmp(command, "sub"))){
        //SUB
        set_n_bits(binary, DPI_OPCODE_END_BIT, 2);
        type = compute_result;
    } else if (!(strcmp(command, "rsb"))){
        //RSB
        set_n_bits(binary, DPI_OPCODE_END_BIT, 3);
        type = compute_result;
    } else if (!(strcmp(command, "add"))){
        //ADD
        set_n_bits(binary, DPI_OPCODE_END_BIT, 4);
        type = compute_result;
    } else if (!(strcmp(command, "orr"))){
        //ORR
        set_n_bits(binary, DPI_OPCODE_END_BIT, 12);
        type = compute_result;
    } else if (!(strcmp(command, "mov"))){
        //MOV
        set_n_bits(binary, DPI_OPCODE_END_BIT, 13);
        type = single_operand;
    } else if (!(strcmp(command, "tst"))){
        //TST
        set_n_bits(binary, DPI_OPCODE_END_BIT, 8);
        type = set_CPSR;
    } else if (!(strcmp(command, "teq"))){
        //TEQ
        set_n_bits(binary, DPI_OPCODE_END_BIT, 9);
        type = set_CPSR;
    } else if (!(strcmp(command, "cmp"))){
        //CMP
        set_n_bits(binary, DPI_OPCODE_END_BIT, 10);
        type = set_CPSR;
    } else{
        //AND (no bits set, they remain 0)
        type = compute_result;
    }

    //set S bit if type is set_CPSR
    if (type == set_CPSR){
        set_n_bits(binary, 20, 1);
    }

    //set I (can be done after because it is a single bit)
    int argument;
    //decide which operand to look at based on type (mov looks at op 2, others at op 3)
    if(type == single_operand){
        argument = 1;
    } else {
        argument = 2;
    }
    //identifying and checking operand2
    char* operand2 = tokenised_line->operands[line][argument];
    if (operand2[0]=='#'){
        set_n_bits(binary,25, 1);
    }

    //set Rn (except for mov)
    if (type != single_operand){
        char* rn = tokenised_line->operands[line][1];
        //move pointer to only consider reg number (remove r from rxx)
        rn += sizeof(char);
        int reg_num = (int) strtol(rn, (char **) NULL, 10);
        set_n_bits(binary, 16, reg_num);
    }

    //set Rd
    char* rd = tokenised_line->operands[line][0];
    rd+= sizeof(char);
    int reg_num = (int) strtol(rd, (char **) NULL, 10);
    set_n_bits(binary, 12, reg_num);

    


    return binary;
}


uint32_t assemble_mul(char *string, char **code, int line, symbol_table symbol_table) {
    extract_2_char_cond(string, cond);

    //set cond to 1110
    binary = set_n_bits(binary, COND_END_BIT, 14);

    //bits 27 to 22 are already 0

    //set A only if mla
    if (strcmp(cond, "la") == 0) {
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

uint32_t assemble_sdt(char *string, char **code, int line, symbol_table symbol_table){
    return binary;
}

uint32_t assemble_branch(char *string, char **code, int line, symbol_table symbol_table) {
    extract_2_char_cond(string, cond);

    //setting cond bits
    if (strcmp(cond, "eq") == 0){
        //BEQ
        binary = set_n_bits(binary, COND_END_BIT, 0);
    } else if (strcmp("ne", cond) == 0){
        //BNE
        binary = set_n_bits(binary, COND_END_BIT, 1);
    } else if (strcmp("ge", cond) == 0){
        //BGE
        binary = set_n_bits(binary, COND_END_BIT, 10);
    } else if (strcmp("lt", cond) == 0) {
        //BLT
        binary = set_n_bits(binary, COND_END_BIT, 11);
    } else if (strcmp("gt", cond) == 0) {
        //BGT
        binary = set_n_bits(binary, COND_END_BIT, 12);
    } else if (strcmp("le", cond) == 0){
        //BLE
        binary = set_n_bits(binary, COND_END_BIT, 13);
    } else if (strcmp("al", cond) == 0){
        //UNCONDITIONAL BRANCH
        binary = set_n_bits(binary, COND_END_BIT, 14);
    } else if (strcmp("  ", cond) == 0){
        //B (NO SUFFIX)
        binary = set_n_bits(binary, COND_END_BIT, 14);
    } else {
        //NOT A VALID INSTRUCTION
        fprintf(stderr, "not a valid branch instruction");
    }

    //set bits 27-24 to be 1010
    binary = set_n_bits(binary, 24, 10);


    //TODO: set offset
    uint32_t current_address = &code[line];
        //TODO 1: get the exact label from the instruction
        char *label = (char*) malloc(0);
        get_argument(string, 1, label);
        
        //TODO 2: extract that exact mapping from the symbol table
        //TODO 3: find the address for that label from mapping
        //TODO 4: calculate the offset (destination - current + 8)?

    //uint32_t destination_address =

     free(label);
    return binary;
}
