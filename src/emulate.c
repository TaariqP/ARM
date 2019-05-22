#include <stdlib.h>
#include <stdio.h>
#include <printf.h>
#include <stdint.h>
#include "emulate_utils/utils.h"
#include "emulate_utils/defs.h"


int main(int argc, char **argv) {

    if (argc != 2) {
        printf("Provide an argument!");
        return EXIT_FAILURE;
    }

    char* filename = argv[1];
    FILE *input = fopen(filename, "rb");

    if (input == NULL) {
        printf("Invalid File");
        return EXIT_FAILURE;
    }
    uint8_t memory[NUM_ADDRESSES];
    binary_file_loader(filename, (char *) memory);
    print_binary(memory);

    return EXIT_SUCCESS;
}



