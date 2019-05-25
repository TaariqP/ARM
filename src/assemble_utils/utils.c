//
// Created by Satadru on 5/25/19.
//

#include <bits/types/FILE.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"


void binary_file_writer(char *filename, const char *binary_string){
    FILE *binary_file = fopen(filename, "wb");
    if (binary_file == NULL) {
        printf("Error opening file");
    }
    fwrite(filename, 1, sizeof(binary_string),binary_file);
    fclose(binary_file);
}
