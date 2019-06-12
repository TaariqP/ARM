//
// Created by Satadru on 12/06/19.
//
#include <ncurses.h>
#include <unistd.h>

#ifndef ARM11_11_STICKS_H
#define ARM11_11_STICKS_H

#define LEFT_X 0

void display_left_stick(WINDOW *, int);
void display_right_stick(WINDOW *, int);

#endif //ARM11_11_STICKS_H
