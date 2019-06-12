CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_DEFAULT_SOURCE -std=c99 -pedantic 
NAME = main
SRC = main.c
OB = main.o

.SUFFIXES: .c .o .h

.PHONY: clean



all: $(NAME)

$(NAME):
	gcc $(CFLAGS) $(SRC) -o $(NAME) -lncurses


clean:
	rm -f $(wildcard *.o)
	rm -f main
