//
// Created by Satadru on 5/25/19.
//

#include <bits/types/FILE.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "defs.h"


void binary_file_writer(char *filename, const char *binary_string){
    FILE *binary_file = fopen(filename, "wb");
    if (binary_file == NULL) {
        printf("Error opening file");
    }
    fwrite(filename, 1, sizeof(binary_string),binary_file);
    fclose(binary_file);
}

void extract_branch_cond(char *string, char result[3]){
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

uint32_t set_4_bits(uint32_t binary, int end_bit, int value){
    binary |= (value << end_bit);
    return binary;
}

void two_pass_assembly(){

    //First pass
    symbol_table *symbol_table = malloc(sizeof(mapping) * LINES);

    /* Second Pass */

}

void tokeniser(char* line){
    char* label;
    char* rest;
    label = strtok(line, ":",);

}
