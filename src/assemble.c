#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv) {

    if (argc != 2) {
        printf("Provide an argument!");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
