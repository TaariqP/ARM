//
// Created by taariq on 5/28/19.
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "utils.h"

char condition[3];

/*at this point, we know that the tokenized line number x contains a dpi instruction
 * takes the entire set of tokenized lines, and assembles tokenized line x into binary
 * the result is stored as a binary string to the char* provided as input*/
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


// Optional
    int no_of_operands = 4;
    //Optional sub command e.g. sub r5,r4, r3, lsr r2
    if (!(strcmp(command, "sub") && no_of_operands == 4){
        char* ret;
        if (ret = strstr(tokenised_line->operands[line][3], "lsr"){
            ret += sizeof(char);
            // ret now contains the register number to get the last byte
                    
        }
    }


    //set S bit if type is set_CPSR
    if (type == set_CPSR) {
        set_n_bits(&binary, 20, 1);
    }

    //set I (can be done after because it is a single bit)
    int argument;
    //decide which operand to look at (either 2 or 3) based on type
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
        rn++;
        int reg_num = (int) strtol(rn, (char **) NULL, 10);
        set_n_bits(&binary, 16, reg_num);
    }

    if (type == set_CPSR) {
        char *rn = tokenised_line->operands[line][0];
        rn++;
        int reg_num = (int) strtol(rn, (char **) NULL, 10);
        set_n_bits(&binary, 16, reg_num);
    }

    //set Rd
    if (type != set_CPSR) {
        char *rd = tokenised_line->operands[line][0];
        rd++;
        int reg_num = (int) strtol(rd, (char **) NULL, 10);
        set_n_bits(&binary, 12, reg_num);
    }

    //takes off the # or the r, regardless of register or immediate value
    operand2++;


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
        if (immediate_value <= 0xFF) {
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

/*same as assemble_dpi, but for sdt instructions*/
void assemble_sdt_to(tokenised_line *tokenised_line, int line, char *binary_string, int current_instruction,
                     int final_instruction, int *value,
                     int offset, char **byte_to_add) {

    uint32_t binary = 0;
    char *opcode = tokenised_line->opcode[line];
    char *address = tokenised_line->operands[line][1];
    char *address_offset = tokenised_line->operands[line][2];


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

    int expression_value;

    if (!(strcmp(opcode, "ldr"))) {
        //LDR instruction
        set_L = 1;
        if (address[0] == '=') {
            //numeric constant

            address++;
            //asssume always hex
            expression_value = (int) strtol(address, NULL, 16);
            if (expression_value <= 0xFF) {
                //convert to mov instruction
                opcode[0] = 'm';
                opcode[1] = 'o';
                opcode[2] = 'v';

                address -= sizeof(char);
                address[0] = '#';
                tokenised_line->operands[line][1] = address;
                assemble_dpi_to(tokenised_line, line, binary_string);
                return;
            } else {

                //in general case we need to output the value of expression, store it in 4 bytes at end of program
                //TODO: output expression, stick it onto end of binary
                //TODO: get address of new thing added, calculate offset, set last bits to offset
                /* In the general case, the assembler should put the value of <expression> in four bytes at the end
    of the assembled program, and use the address of this value, with the PC as the base register and
    a calculated offset, in the assembled ldr instruction. It will be always a pre-indexed address.*/

//pipeline offset subtracted

                int destination_address = ((final_instruction - current_instruction) * 4) + offset - 8;
                *value = 1;
                set_I = 0;
                set_P = 1;
                set_U = 1;
                set_L = 1;
                set_rn = 15;
                //rd alreaday set
                //offset is destination_address
                set_offset = destination_address;
                //value to add to the byte
                char res[33];
                toBinaryString(expression_value, res);
                strcat(*byte_to_add, res);

                set_sdt_bits(&binary, set_I, set_P, set_U, set_L, set_rn, set_rd, set_offset);

                toBinaryString(binary, binary_string);
                return;
            }
        }

    } else {
        //STR instruction
        set_L = 0;
    }

    // ldr Rd, [rn] = pre
    // ldr Rd, [rn, #exp] = pre
    // ldr Rd, [rn], #exp = post
    address += (2 * sizeof(char));
    set_rn = (int) strtol(address, NULL, 10);
    bool isNegative = false;

    int lastargumentindex = tokenised_line->num_of_operands[line] - 1;
    if (tokenised_line->num_of_operands[line] == 2 ||
        containsChar(']', tokenised_line->operands[line][lastargumentindex])) {
        //pre indexed
        set_P = 1;

        if (address_offset[0] == 'r') {
            set_I = 1;
        }

        //either type [rn,<#exp>] or type [rn], TODO: identify type, set offset based on it
        if (address_offset[0] == '#') {
            //type is [rn,<#expression>]

            address_offset++;


            if (address_offset[0] == '-') {
                isNegative = true;
                address_offset++;
            } else {
                set_U = 1;
            }
            //get base of number and set offset
            int base;
            set_base(address_offset, &base);
            set_offset = (int) strtol(address_offset, NULL, base);


            //set U based on +ve or -ve
            if (isNegative) {
                set_U = 0;
            }

        } else if (containsChar(']', address_offset)){
            // the entire offset is just a reg value

            if (address_offset[0] == '-') {
                isNegative = true;
                address_offset++;
            }
            //set U based on +ve or -ve
            if (isNegative) {
                set_U = 0;
            }

            address_offset++;
            int rm = (int) strtol(address_offset, NULL, 10);
            set_offset = rm;
        } else {
            // contains another 4th argument that specifies the shift
            char *address_offset_shift = tokenised_line->operands[line][3];
            if (address_offset[0] == '-') {
                isNegative = true;
                address_offset++;
            }

            //set U based on +ve or -ve
            if (isNegative) {
                set_U = 0;
            }

            int shift_type = 0;
            if (strstr(address_offset_shift, "lsr")) {
                //shift is an lsr
                shift_type = 1;
            } else if (strstr(address_offset_shift, "asr")){
                //shift is an asr
                shift_type = 2;
            } else if (strstr(address_offset_shift, "ror")){
                //shift is a ror
                shift_type = 3;
            }
            //otheriwse is an lsl, no need to change the shift_type since it is already 0

            bool shift_by_register = false;
            if (address_offset_shift[4] == 'r'){
                shift_by_register = true;
            }

            //move past "lsr #" in "lsr #exp"
            address_offset_shift += (5 * sizeof(char));
            int base;
            set_base(address_offset_shift, &base);
            int shift = (int) strtol(address_offset_shift, NULL, base);

            address_offset++;
            int rm = (int) strtol(address_offset, NULL, 10);

            //combine rm, shift_type and shift appropriately (page 7 of spec)
            if(shift_by_register){
                shift = shift << 8;
            } else {
                shift = shift << 7;
            }
            shift_type = shift_type << 5;
            set_offset = shift + shift_type +rm;
            if (shift_by_register){
                set_offset += (0x1 << 4);
            }

        }
        //otherwise is of type [rn], offset need not be set
    } else {
        //post indexed

        if (address_offset[0] == 'r') {
            set_I = 1;
        }

        if (address_offset[0] == '#') {
            // numeric offset

            address_offset++;

            if (address_offset[0] == '-') {
                isNegative = true;
                address_offset++;
            }
            int base;
            set_base(address_offset, &base);
            set_offset = (int) strtol(address_offset, NULL, base);

            //set U based on +ve or -ve
            if (isNegative) {
                set_U = 0;
            }

        } else {
            //shifted register offset
            char *address_offset_shift = tokenised_line->operands[line][2];
            if (address_offset[0] == '-') {
                isNegative = true;
                address_offset++;
            }

            //set U based on +ve or -ve
            if (isNegative) {
                set_U = 0;
            }
            int base;
            set_base(address_offset_shift, &base);
            int shift = (int) strtol(address_offset_shift, NULL, base);

            address_offset++;
            int rm = (int) strtol(address_offset, NULL, 10);

            //combine rm and shift appropriately (page 7 of spec)
            shift = shift << 7;
            set_offset = shift + rm;
        }
    }

    set_sdt_bits(&binary, set_I, set_P, set_U, set_L, set_rn, set_rd, set_offset);

    toBinaryString(binary, binary_string);
    printf("%d\n", binary);
    printf("%x\n", binary);
}

/*same as assemble_dpi, but for mul instructions*/
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

/*same as assemble_dpi, but for branch instructions*/
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
        char res[33];
        toBinaryString(offset, res);
        printf("Offset: %s\n", res);
        set_n_bits(&binary, 0, offset);
        toBinaryString(binary, binary_string);
    }
}

/*same as assemble_dpi, but for special instructions*/
void assemble_special_to(tokenised_line *tokenised_line, int line, char *binary_string) {
    uint32_t binary = 0;
    char *opcode = tokenised_line->opcode[line];

    if (!(strcmp("andeq", opcode))) {
        //ALL 0 HALT INSTRUCTION
        toBinaryString(binary, binary_string);
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