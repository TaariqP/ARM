#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

#include "assemble_utils/utils.h"
#include "assemble_utils/defs.h"
/*
int main(int argc, char **argv) {

    if (argc != 2) {
        printf("Provide an argument!");
        return EXIT_FAILURE;
    }


    FILE *file;
    char **code = (char **) malloc(sizeof(char *) * LINES);
    file = fopen(argv[1], "r");

    if (file) {
        int line = 0;
        code[line] = (char *) malloc(sizeof(char) * LINE_LENGTH);
        while (fgets(code[line], LINE_LENGTH, file) != NULL) {
            //Add null terminator at the end of every line
            code[line][strlen(code[line]) - 1] = '\0';
            printf("Code line %d: %s\n", line, code[line]);
            line++;
            code[line] = (char *) malloc(sizeof(char) * LINE_LENGTH);
        }
        fclose(file);
        char *binary = two_pass_assembly(code, line);
        //binary_file_writer(binary);
    } else {
        printf("Could not open file");
    }



    return EXIT_SUCCESS;
}
*/
