//
// Created by ss13718 on 13/06/19.
//

#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>

#ifndef ARM11_11_UTILS_H
#define ARM11_11_UTILS_H


void printA(WINDOW *window, int start_x, int start_y);
void printB(WINDOW *window, int start_x, int start_y);
void printC(WINDOW *window, int start_x, int start_y);
void printD(WINDOW *window, int start_x, int start_y);
void printE(WINDOW *window, int start_x, int start_y);
void printF(WINDOW *window, int start_x, int start_y);
void printG(WINDOW *window, int start_x, int start_y);


void print_number(WINDOW *, int, int, int);

#endif //ARM11_11_UTILS_H
