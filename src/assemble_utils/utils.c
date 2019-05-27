//
// Created by taariq on 5/25/19.
//

#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "defs.h"


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
