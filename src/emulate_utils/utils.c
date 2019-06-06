//
// Created by taariq on 5/22/19.
//


#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "defs.h"

/*returns the value of the exact bit of the value inputted that we want (0 indexed)*/
uint8_t mask_1_bit(int value, int bit) {
    return (uint8_t) ((value >> bit) & 0x1);
}

/*returns the 4 bits required from the value (given the end bit) (0 indexed)*/
uint8_t mask_4_bit(int value, int end_bit) {
    return (uint8_t) ((value >> end_bit) & 0xF);
}

/*returns the byte required from the value (given the end bit) (0 indexed)*/
uint8_t mask_8_bit(int value, int end_bit) {
    return (uint8_t) ((value >> end_bit) & 0xFF);
}

//Reads the binary file
void binary_file_loader(char *filename, char *memory) {

    FILE *binaryFile = fopen(filename, "rb");
    fseek(binaryFile, 0, SEEK_END);
    int size = (int) ftell(binaryFile);
    rewind(binaryFile);
    if (binaryFile) {
        fread(memory, (size_t) size, 1, binaryFile);
    } else {
        printf("%s", "Could not read\n");
    }
    fclose(binaryFile);

}
/*prints all non zero memories as required by emulate test outputs*/
void print_binary(uint8_t *memory) {

    printf("%s\n", "Non-zero memory:");
    for (int i = 0; i < NUM_ADDRESSES; i = i + 4) {
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
/*prints out the value of all registers as required by emulate test outputs*/
void print_registers(int32_t *registers) {
    printf("%s\n", "Registers:");
    for (int i = 0; i < 13; i++) {
        char reg[4];
        reg[0] = '$';
        sprintf(&reg[1], "%d", i);
        if (i <10){
            sprintf(&reg[2], " ");
        }
        printf("%s : %10d (0x%08x)\n",reg, registers[i], registers[i]);
    }
    printf("PC  : %10d (0x%08x)\n", registers[PC], registers[PC]);
    printf("CPSR: %10d (0x%08x)\n", registers[CPSR], registers[CPSR]);
}

/*checks the CPSR register to see whether condition is valid*/
bool check_condition(current_state *state) {
    int32_t value = state->registers[CPSR];
    uint8_t n = mask_1_bit(value, N);
    uint8_t z = mask_1_bit(value, Z);
    uint8_t c = mask_1_bit(value, C);
    uint8_t v = mask_1_bit(value, V);
    uint8_t cond = state->decoded_instruction.cond;

    switch (cond) {
        case 0:
            return z;
        case 1:
            return !z;
        case 10:
            return n == v;
        case 11:
            return (n != v);
        case 12:
            return !z && (n == v);
        case 13:
            return z || (n != v);
        case 14:
            return 1;
        default:
            printf("Failed CPSR Check\n");
            return 0;
    }
}

int get_file_size(char *filename) {

    FILE *fp = fopen(filename, "rb");
    fseek(fp, 0, SEEK_END);
    int lengthOfFile = (int) ftell(fp);
    fclose(fp);
    return lengthOfFile;
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

void pc_increment(current_state *state) {
    state->registers[PC] += 4;
}

void set_register(current_state *state, int reg, int value) {
    state->registers[reg] = value;
}

void set_CPSR_bit(current_state *state, int bit_number, int val) {
    int32_t cpsr = state->registers[CPSR];
    if (!val) {
        if (mask_1_bit(cpsr, bit_number)) {
            cpsr ^= (0x1 << bit_number);
        }
    } else {
        cpsr |= (0x1 << bit_number);
    }

    state->registers[CPSR] = cpsr;

}

int32_t sign_extend_26_to_32(int32_t value) {
    uint8_t most_significant_bit = mask_1_bit(value, 25);
    if (most_significant_bit) {
        return value | 0xFC000000;
    }
    return value;
}

int ror(int32_t val, uint32_t num) {
    return (val >> num) | val << (32 - num);
}

int lsr(int x, int n) {
    return (int) ((unsigned int) x >> n);
}

//Preserves sign bit
int asr(int x, int n) {
    return x >> n;
}

int lsl(int x, int n) {
    return (int) ((unsigned int) x << n);
}