//
// Created by ss13718 on 13/06/19.
//

#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include "ball.h"

#ifndef ARM11_11_UTILS_H
#define ARM11_11_UTILS_H

#define SCREEN_WIDTH 100
#define SCREEN_HEIGHT 45
#define SCORE_HEIGHT 13
#define SCORE_WIDTH (STICK_WINDOW_WIDTH - 20)
#define MAXSCORE 1
#define max(x,y) ((x) >= (y)) ? (x) : (y)

void printA(WINDOW *window, int start_x, int start_y);
void printB(WINDOW *window, int start_x, int start_y);
void printC(WINDOW *window, int start_x, int start_y);
void printD(WINDOW *window, int start_x, int start_y);
void printE(WINDOW *window, int start_x, int start_y);
void printF(WINDOW *window, int start_x, int start_y);
void printG(WINDOW *window, int start_x, int start_y);


void print_number(WINDOW *, int, int, int);
void initialise_game(WINDOW *, ball *, int *, int *, int *, int *);
void display_game_state(WINDOW *, WINDOW *, ball *, int , int , int , int);
void print_message_center(WINDOW *, char *, int );

#endif //ARM11_11_UTILS_H
