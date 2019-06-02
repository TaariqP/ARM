//
// Created by taariq on 5/28/19.
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "utils.h"


// the variables below will be common to all functions, so have been initialised here. maybe we should pass them in?

char condition[3];


//TODO

//CHANGE ALL ASSEMBLE FUNCTIONS TO TAKE TOKENISED LINE rather than the whole string
//ALSO ADD assemble special instructions
void assemble_dpi_to(tokenised_line *tokenised_line, int line, char *binary_string) {
    uint32_t binary = 0;

    //set cond to 1110
    set_n_bits(&binary, COND_END_BIT, 14);

    //bits 27-26 always 0

    char *command = tokenised_line->opcode[line];
    DPI_TYPE type;


    //set opcode and type
    //needs if else ladder because of the way C compares strings :(
    if (!strcmp(command, "eor")) {
        //EOR
        set_n_bits(&binary, DPI_OPCODE_END_BIT, 1);
        type = compute_result;
    } else if (!(strcmp(command, "sub"))) {
        //SUB
        set_n_bits(&binary, DPI_OPCODE_END_BIT, 2);
        type = compute_result;
    } else if (!(strcmp(command, "rsb"))) {
        //RSB
        set_n_bits(&binary, DPI_OPCODE_END_BIT, 3);
        type = compute_result;
    } else if (!(strcmp(command, "add"))) {
        //ADD
        set_n_bits(&binary, DPI_OPCODE_END_BIT, 4);
        type = compute_result;
    } else if (!(strcmp(command, "orr"))) {
        //ORR
        set_n_bits(&binary, DPI_OPCODE_END_BIT, 12);
        type = compute_result;
    } else if (!(strcmp(command, "mov"))) {
        //MOV
        set_n_bits(&binary, DPI_OPCODE_END_BIT, 13);
        type = single_operand;
    } else if (!(strcmp(command, "tst"))) {
        //TST
        set_n_bits(&binary, DPI_OPCODE_END_BIT, 8);
        type = set_CPSR;
    } else if (!(strcmp(command, "teq"))) {
        //TEQ
        set_n_bits(&binary, DPI_OPCODE_END_BIT, 9);
        type = set_CPSR;
    } else if (!(strcmp(command, "cmp"))) {
        //CMP
        set_n_bits(&binary, DPI_OPCODE_END_BIT, 10);
        type = set_CPSR;
    } else {
        //AND (no bits set, they remain 0)
        type = compute_result;
    }


//    int no_of_operands = 4;
//    //Optional sub command
//    if (!(strcmp(command, "sub")){
//        for (int i = 0; i < no_of_operands; ++i) {
//            char* operand = tokenised_line->operands[line][i];
//            if (strstr(operand, "lsl")){
//                char* r3 = tokenised_line->operands[line][i-1];
//
//                //Store the state of tokenised line
//                //Set tokenised line to lsl r3,(last byte of value in r4)
//                assemble_special_to(tokenised_line, line)
//            }
//        }
//    }

    //set S bit if type is set_CPSR
    if (type == set_CPSR) {
        set_n_bits(&binary, 20, 1);
    }

    //set I (can be done after because it is a single bit)
    int argument;
    //decide which operand to look at based on type (mov looks at op 2, others at op 3)
    if (type == single_operand | type == set_CPSR) {
        argument = 1;
    } else {
        argument = 2;
    }
    //identifying and checking operand2
    char *operand2 = tokenised_line->operands[line][argument];
    bool isImmediate = (operand2[0] == '#');
    if (isImmediate) {
        set_n_bits(&binary, 25, 1);
    }



    //set Rn (except for mov)
    if (type != single_operand & type != set_CPSR) {
        char *rn = tokenised_line->operands[line][1];
        //move pointer to only consider reg number (remove r from rxx)
        rn += sizeof(char);
        int reg_num = (int) strtol(rn, (char **) NULL, 10);
        set_n_bits(&binary, 16, reg_num);
    }

    if (type == set_CPSR) {
        char *rn = tokenised_line->operands[line][0];
        rn += sizeof(char);
        int reg_num = (int) strtol(rn, (char **) NULL, 10);
        set_n_bits(&binary, 16, reg_num);
    }

    //set Rd
    if (type != set_CPSR) {
        char *rd = tokenised_line->operands[line][0];
        rd += sizeof(char);
        int reg_num = (int) strtol(rd, (char **) NULL, 10);
        set_n_bits(&binary, 12, reg_num);
    }

    //takes off the # or the r, regardless of register or immediate value
    operand2 += sizeof(char);


    if (type == set_CPSR) {
        int op2 = (int) strtol(operand2, (char **) NULL, 10);
        set_n_bits(&binary, 0, op2);
    }
    //Base can be either BASE 10 OR BASE 16

    int base = 0;
    set_base(operand2, &base);

    //calculate offset
    if (isImmediate) {
        int immediate_value = (int) strtol(operand2, NULL, base);
        if (immediate_value <= 256) {
            //can be stored directly in last 8 bits without need for rotate
            set_n_bits(&binary, 0, immediate_value);
        } else {
            //rotate left till is 8 bits, and rotates are even, then store appropriately
            int num_of_rotates = 0;

            while (!(is8bit(immediate_value) & (num_of_rotates % 2 == 0))) {
                immediate_value = rol((uint32_t) immediate_value);
                num_of_rotates++;
            }


            set_n_bits(&binary, 8, (num_of_rotates / 2));
            set_n_bits(&binary, 0, immediate_value);
        }
    } else {
        //is shift register
        //calculate and set shift

        //set Rm
        int rm = (int) strtol(operand2, (char **) NULL, 10);
        set_n_bits(&binary, 0, rm);

    }

    toBinaryString(binary, binary_string);
}


void assemble_sdt_to(tokenised_line *tokenised_line, int line, char *binary_string) {

    uint32_t binary = 0;
    char *opcode = tokenised_line->opcode[line];
    char *address = tokenised_line->operands[line][1];
    char *address_offset = tokenised_line->operands[line][2];
    //really need the following int to make this work fully
    int num_of_operands;


    //initialise all to their respective defaults
    int set_I = 0;
    int set_P = 0;
    int set_U = 1;
    int set_L = 0;
    int set_rn = 0;
    int set_rd = 0;
    int set_offset = 0;

    //extract rd
    set_rd = (int) strtol((tokenised_line->operands[line][0] += sizeof(char)), NULL, 10);

    if (!(strcmp(opcode, "ldr"))){
        //LDR instruction
        set_L = 1;
        if (address[0] == '='){
            //numeric constant

            //potential just expression ++
            address += sizeof(char);
            //asssume always hex
            int expression_value = (int) strtol(address, NULL, 16);
            if (expression_value <= 0xFF){
                //convert to mov instruction
                opcode[0] = 'm';
                opcode[1] = 'o';
                opcode[2] = 'v';

                address -= sizeof(char);
                address[0] = '#';
                tokenised_line->operands[line][1] = address;
                assemble_dpi_to(tokenised_line, line, binary_string);
                return;
            }

            //in general case we need to output the value of expression, store it in 4 bytes at end of program
            //TODO: output expression, stick it onto end of binary
            //TODO: get address of new thing added, calculate offset, set last bits to offset
            
        }


    } else {
        //STR instruction
        set_L = 0;
    }

    //need to somehow differentiate between pre and post indexed
    if (true){

        //pre indexed
        set_P = 1;
        address += (2 * sizeof(char));

        set_rn = (int) strtol(address, NULL, 10);

        //either type [rn,<#exp>] or type [rn], TODO: identify type, set offset based on it
        if (address_offset[0] == '#') {
            //type is [rn,<#expression>]
            address_offset += sizeof(char);
            bool isNegative = false;
            if (address_offset[0] == '-'){
                isNegative = true;
                address_offset += sizeof(char);
            }
            //get base of number and set offset
            int base;
            set_base(address_offset, &base);
            set_offset = (int) strtol(address_offset, NULL, base);

            //set U based on +ve or -ve
            if (isNegative){
                set_U = 0;
            }

        }
        //otherwise is of type [rn], offset need not be set
    }

    //assume always executed, set cond
    set_n_bits(&binary, 28, 14);

    set_n_bits(&binary, 26, 1);

    //setI
    set_n_bits(&binary, 25, set_I);

    //setP
    set_n_bits(&binary, 24, set_P);

    //setU
    set_n_bits(&binary, 23, set_U);

    //setL
    set_n_bits(&binary, 20, set_L);

    //setrn
    set_n_bits(&binary, 16, set_rn);

    //setrd
    set_n_bits(&binary, 12, set_rd);

    //setoffset
    set_n_bits(&binary, 0, set_offset);

    toBinaryString(binary, binary_string);
    printf("%d\n", binary);
    printf("%x\n", binary);
}


void assemble_mul_to(tokenised_line *tokenised_line, int line, char *binary_string) {
    //condition is the last 2 letters of the opcode
    char *condition = tokenised_line->opcode[line] + sizeof(char);
    uint32_t binary = 0;
    //set cond to 1110
    set_n_bits(&binary, COND_END_BIT, 14);

    //bits 27 to 22 are already 0

    //set A only if mla
    if (strcmp(condition, "la") == 0) {
        set_n_bits(&binary, 21, 1);
    }

    // S bit already set to 0

    //TODO: set Rd
    set_operand(&binary, line, 0, 16, tokenised_line);

    //TODO: set Rn
    if (strcmp(condition, "la") == 0) {
        set_operand(&binary, line, 3, 12, tokenised_line);
    }
    //TODO: set Rs
    set_operand(&binary, line, 2, 8, tokenised_line);

    //set bits 7-4 to be 1001
    set_n_bits(&binary, 4, 9);

    //TODO: set Rm
    set_operand(&binary, line, 1, 0, tokenised_line);

    toBinaryString(binary, binary_string);

}


void assemble_branch_to(tokenised_line *tokenised_line, char **code, int line, symbol_table *symbol_table,
                        char *binary_string, int num_of_labels) {
    //condition is the last two letter of the command
    uint32_t binary = 0;
    char *condition = tokenised_line->opcode[line] + sizeof(char);




    //setting cond bits
    //TODO when all test cases pass, refactor because perhaps all test cases are not actually commands
    if (strcmp(condition, "eq") == 0) {
        //BEQ
        set_n_bits(&binary, COND_END_BIT, 0);
    } else if (strcmp("ne", condition) == 0) {
        //BNE
        set_n_bits(&binary, COND_END_BIT, 1);
    } else if (strcmp("ge", condition) == 0) {
        //BGE
        set_n_bits(&binary, COND_END_BIT, 10);
    } else if (strcmp("lt", condition) == 0) {
        //BLT
        set_n_bits(&binary, COND_END_BIT, 11);
    } else if (strcmp("gt", condition) == 0) {
        //BGT
        set_n_bits(&binary, COND_END_BIT, 12);
    } else if (strcmp("le", condition) == 0) {
        //BLE
        set_n_bits(&binary, COND_END_BIT, 13);
    } else if (strcmp("al", condition) == 0) {
        //UNCONDITIONAL BRANCH
        set_n_bits(&binary, COND_END_BIT, 14);
    } else if (condition[0] == '\0') {
        //B (NO SUFFIX)
        set_n_bits(&binary, COND_END_BIT, 14);
    } else {
        //NOT A VALID INSTRUCTION
        fprintf(stderr, "not a valid branch instruction");
    }

    //set bits 27-24 to be 1010
    set_n_bits(&binary, 24, 10);

    //4 bytes
    //int pc = current_address + 8;

    //Get operand which will be an address
    int target_address = (int) strtol(tokenised_line->operands[line][0], NULL, 10);
    int current_address = 4 * (line - num_of_labels);
    //Calculate offset = label - current - 8 byte pipeline
    int offset = (target_address - current_address) - 8;
    //get first 26 bits
    offset &= 0x3FFFFFF;
    //Check 26 bits
    if (!is26bit(offset)) {
        fprintf(stderr, "invalid offset");
    } else {
        //Right shift
        offset >>= 2;
        //Get first 24 bits
        offset &= 0xFFFFFF;
        //check offset valid and set offset
        printf("offset value: %d\n", offset);
        char *res;
        toBinaryString(offset, res);
        printf("Offset: %s\n", res);
        set_n_bits(&binary, 0, offset);
        toBinaryString(binary, binary_string);
    }
}


void assemble_special_to(tokenised_line *tokenised_line, int line, char *binary_string) {
    uint32_t binary = 0;
    char *opcode = tokenised_line->opcode[line];

    if (!(strcmp("andeq", opcode))) {
        //ALL 0 HALT INSTRUCTION
        toBinaryString(binary,binary_string);
        return;
    }
    //LSL INSTRUCTION
    /*LSL Rn <#exp> = mov Rn, Rn, lsl <#exp>
     -apply a logical shift to Rn by the amount specified
        by exp to compute a new value
     -then we just return mov Rn val*/

    //set binary to basically mov

    //cond
    set_n_bits(&binary, 28, 14);

    //2 zeroes, I is zero,

    //opcode
    set_n_bits(&binary, 21, 13);

    //S is zero

    //Rn is zero

    //Rd is
    set_operand(&binary, line, 0, 12, tokenised_line);


    //Setting Operand2
    //get shift amount
    char *expression = tokenised_line->operands[line][1] + sizeof(char);
    int base;
    set_base(expression, &base);
    int shiftAmt = (int) strtol(expression, NULL, base);

    //can only take 5 bits of shiftAmt, or else its a register shift, but register unspecified
    //shiftAmt &= 0x1F;


    //Shift
    set_n_bits(&binary, 7, shiftAmt);

    //Rm set to rn
    set_operand(&binary, line, 0, 0, tokenised_line);
    //set_n_bits(&binary, 0, shiftAmt);
    //set_operand(&binary, line, 1, 0, tokenised_line);


    toBinaryString(binary, binary_string);


}
