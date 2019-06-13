CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_DEFAULT_SOURCE -std=c99 -pedantic 

.SUFFIXES: .c .o .h

.PHONY: clean



all: clean main

main:
	gcc $(CFLAGS) main.c sticks.c ball.c -o main -lncurses


clean:
	rm -f $(wildcard *.o)
	rm -f main
