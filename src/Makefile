CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_DEFAULT_SOURCE -std=c99 -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

assemble: assemble.o assemble_utils/instruction_assembler.c assemble_utils/utils.c

emulate: emulate.o emulate_utils/utils.c emulate_utils/decode.c emulate_utils/execute.c


clean:
	rm -f $(wildcard *.o)
	rm -f assemble
	rm -f emulate
