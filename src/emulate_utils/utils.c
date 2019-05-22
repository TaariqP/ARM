//
// Created by taariq on 5/22/19.
//


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "defs.h"

//Reads the binary file

void binary_file_loader(char *filename, char *memory) {


    FILE *binaryFile;
    FILE *fp = fopen(filename, "rb");
    fseek(fp, 0, SEEK_END);
    int size = (int) ftell(fp);
    fclose(fp);
    binaryFile = fopen(filename, "rb");
    if (binaryFile) {
        fread(memory, (size_t) size, 1, binaryFile);
    } else {
        printf("%s", "Could not read\n");
    }
    fclose(binaryFile);

}

void print_binary(uint8_t *memory) {

    printf("%s\n", "Non-zero memory:");
    for (int i = 0; i < 32; i = i + 4) {
        uint8_t value[4];
        value[3] = (memory[i]);
        value[2] = (memory[i + 1]);
        value[1] = (memory[i + 2]);
        value[0] = (memory[i + 3]);
        uint32_t instruction = (value[3] << 24) | (value[2] << 16) | (value[1] << 8) | value[0];
        if (instruction != 0) {
            printf("0x%08x: 0x%08x\n", i, instruction);
        }
    }

}
