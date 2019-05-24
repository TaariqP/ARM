//
// Created by taariq on 5/22/19.
//


#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
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
    for (int i = 0; i < INSTRUCTION_SIZE; i = i + 4) {
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

void print_registers(int32_t *registers) {
    printf("%s\n", "Registers:");
    printf("$0  : %10d (0x%08x)\n", registers[0], registers[0]);
    printf("$1  : %10d (0x%08x)\n", registers[1], registers[1]);
    printf("$2  : %10d (0x%08x)\n", registers[2], registers[2]);
    printf("$3  : %10d (0x%08x)\n", registers[3], registers[3]);
    printf("$4  : %10d (0x%08x)\n", registers[4], registers[4]);
    printf("$5  : %10d (0x%08x)\n", registers[5], registers[5]);
    printf("$6  : %10d (0x%08x)\n", registers[6], registers[6]);
    printf("$7  : %10d (0x%08x)\n", registers[7], registers[7]);
    printf("$8  : %10d (0x%08x)\n", registers[8], registers[8]);
    printf("$9  : %10d (0x%08x)\n", registers[9], registers[9]);
    printf("$10 : %10d (0x%08x)\n", registers[10], registers[10]);
    printf("$11 : %10d (0x%08x)\n", registers[11], registers[11]);
    printf("$12 : %10d (0x%08x)\n", registers[12], registers[12]);
    printf("PC  : %10d (0x%08x)\n", registers[PC], registers[PC]);
    printf("CPSR: %10d (0x%08x)\n", registers[CPSR], registers[CPSR]);
}



bool check_condition(current_state* state){
    int32_t value = state->registers[CPSR];
    int8_t n = mask_1_bit(value, 31);
    int8_t z = mask_1_bit(value, 30);
    int8_t c = mask_1_bit(value, 29);
    int8_t v = mask_1_bit(value, 28);
    int8_t cond = state->decoded_instruction.cond;

    switch (cond){
        case 0: return z;
        case 1: return !z;
        case 10: return n == v;
        case 11: return !(n == v);
        case 12: return !z & (n == v);
        case 13: return z | !(n == v);
        case 14: return 1;
        default: printf("Failed CPSR Check");
    }
}

uint32_t get_instruct(current_state *state, int address) {

    uint8_t value[4];
    value[0] = (state->memory[address]);
    value[1] = (state->memory[address + 1]);
    value[2] = (state->memory[address + 2]);
    value[3] = (state->memory[address + 3]);
    uint32_t instruction = (value[3] << 24) | (value[2] << 16) | (value[1] << 8) | value[0];
    return instruction;

}

uint8_t mask_1_bit(int value, int bit) {
    return (value >>  bit) & 0x1;
}


uint8_t mask_4_bit(int value, int end_bit) {
    return (value >> end_bit) & 0xF;
}

