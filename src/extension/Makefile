CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_DEFAULT_SOURCE -std=c99 -pedantic 

.SUFFIXES: .c .o .h

.PHONY: clean




main:
	gcc $(CFLAGS) main.c sticks.c ball.c utils.c -o main -lncurses


clean:
	rm -f $(wildcard *.o)
	rm -f $(wildcard *.s)
	rm -f main
