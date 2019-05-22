//
// Created by taariq on 5/22/19.
//


#include <stdio.h>

//Reads the binary file

void binary_file_loader(char *filename, char *memory) {

    FILE *binaryFile;
    fseek(binaryFile, 0, SEEK_END);
    int lengthOfFile = (int) ftell(fp);
    binaryFile = fopen(filename, "rb");
    if (binaryFile) {
        fread(memory, (size_t) lengthOfFile, 1, binaryFile);
    } else {
        printf("Could not load binary file.\n");
    }
    fclose(binaryFile);

}
