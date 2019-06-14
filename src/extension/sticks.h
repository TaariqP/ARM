//
// Created by Satadru on 12/06/19.
//
#include <curses.h>
#include <unistd.h>
#include "ball.h"

#ifndef ARM11_11_STICKS_H
#define ARM11_11_STICKS_H

#define STICK_WINDOW_WIDTH 98
#define STICK_WINDOW_HEIGHT 28
#define HEIGHT_OF_STICK 4

void display_left_stick(WINDOW *, int);

void display_right_stick(WINDOW *, int);

void stick_move(ball *, int , int , int *, int *, int *);

#endif //ARM11_11_STICKS_H
